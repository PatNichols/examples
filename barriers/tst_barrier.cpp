#include <pthread.h>
#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <thread>
#include <functional>
#include <atomic>
using std::size_t;


constexpr auto seed = 335;

namespace putils {
struct barrier {
  barrier(uint32_t no_threads):mutex(),cond(),cnt(new std::atomic<uint32_t>(no_threads)),
    m_gen(new std::atomic<uint32_t>(0)),reset_cnt(no_threads),owns(true) {
    mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
    cond = (pthread_cond_t*)malloc(sizeof(pthread_cond_t));
    pthread_mutex_init(mutex,0x0);
    pthread_cond_init(cond,0x0);
  }
  
  explicit barrier( barrier& b):mutex(b.mutex),cond(b.cond),cnt(b.cnt),m_gen(b.m_gen),reset_cnt(b.reset_cnt),
    owns(false)
  {
  }
  barrier(const barrier& b)=delete;
  barrier(barrier&& b)=delete;  
  ~barrier() {
    if (owns) {
      pthread_cond_destroy(cond);
      pthread_mutex_destroy(mutex);
      free(cond);
      free(mutex);
    }
  }
  void wait()
  {
      uint32_t gen = *m_gen;
      pthread_mutex_lock(mutex);
      if ( --*cnt) {
        while (gen==*m_gen) pthread_cond_wait(cond,mutex);
      }else{
        *cnt = reset_cnt;
        ++*m_gen;
        pthread_cond_broadcast(cond);
      }
      pthread_mutex_unlock(mutex);
  }
  void reduce( void (*f)(void *), void * args) {
      uint32_t gen = *m_gen;
      pthread_mutex_lock(mutex);
      if ( --*cnt) {
        while (gen==*m_gen) pthread_cond_wait(cond,mutex);
      }else{
        f(args);
        *cnt = reset_cnt;
        ++*m_gen;
        pthread_cond_broadcast(cond);
      }
      pthread_mutex_unlock(mutex);   
  }
  void wait_no_release()
  {
      uint32_t gen = *m_gen;
      pthread_mutex_lock(mutex);
      if ( --*cnt) {
        while (gen==*m_gen) pthread_cond_wait(cond,mutex);
      }else{
        return;
      }
      pthread_mutex_unlock(mutex);     
  }
  void release()
  {
      *cnt = reset_cnt;
      ++*m_gen;
      pthread_cond_broadcast(cond);
      pthread_mutex_unlock(mutex);     
  }

   pthread_mutex_t *mutex;
   pthread_cond_t *cond;
   std::atomic<uint32_t> *cnt;
   std::atomic<uint32_t> *m_gen;
   uint32_t reset_cnt;
   bool owns;
};

double drand() {
  double x = rand();
  x = x / RAND_MAX;
  x = x + x - 1.;
  return x;
}

}  // namespace putils


void init(size_t n, double *arr) {
  srand(335);
  for (size_t i = 0; i < n; ++i) {
    arr[i] = putils::drand();
  }
  std::cerr << "init " << n << "\n";
}

struct reduce_info {
  double *ptr;
  uint32_t nsum;
};

void reduce(void *args) {
  reduce_info *info = reinterpret_cast<reduce_info*>(args);
  double *p = info->ptr;
  uint32_t np= info->nsum;
  double res{0.};
  for (uint32_t k = 0; k < np; ++k) res += p[k];
  p[0] = res;
}

struct summ_it_t {
  uint32_t id;
  uint32_t nth;
  size_t n;
  double *p;
  double *arr;
  putils::barrier *barr;

  summ_it_t(int id_, uint32_t nth_, size_t n_, double **p_,double **arr_,putils::barrier *b_)
      : id(id_),
        nth(nth_),
        n(n_),
        p(*p_),
        arr(*arr_),
        barr(b_) {
  }

  summ_it_t(int id_, uint32_t nth_, size_t n_, double **p_,double **arr_)
      : id(id_),
        nth(nth_),
        n(n_),
        p(*p_),
        arr(*arr_),
        barr(new putils::barrier(nth_)) {
  }

  ~summ_it_t() {
    if (id==0) delete barr;
  }

  void summ_it() {
    reduce_info info;
    size_t sz0 = n / nth;
    size_t xsz = n % nth;
    size_t toff = 0;
    size_t my_off = 0;
    size_t my_end = 0;
    size_t sz = 0;
    for (uint32_t j = 0; j < nth; ++j) {
      sz = sz0;
      if (xsz) { 
        ++sz;
        --xsz;
      }
      if (j == id) {
        my_off = toff;
        my_end = my_off + sz;    
        break;
      }
      toff += sz;
    }
    double sum{0.};
    for (size_t k = my_off; k < my_end; ++k) {
      sum += arr[k];
    }
    p[id] = sum;
    info.ptr = p;
    info.nsum = nth;
    barr->reduce(reduce,(void*)&info);
  }
};

void summ_test(size_t n, size_t nthd) {
  double *arr;
  double *p;
  arr = new double[n];
  p = new double[nthd];
  init(n, arr);
    
  double sum{0.};
  for (size_t k=0;k<n;++k) {
    sum+=arr[k];
  }
  std::cerr << "ans = " << sum << "\n";
  std::cerr << " n = " << n << " nth " << nthd <<" \n";

  std::thread ** tptr= new std::thread*[nthd];
  summ_it_t **sum_x = new summ_it_t *[nthd];
  std::chrono::high_resolution_clock::time_point start =
    std::chrono::high_resolution_clock::now();
    sum_x[0] = new summ_it_t(0, nthd, n, &p, &arr);
    tptr[0]=new std::thread(&summ_it_t::summ_it, *sum_x[0]);  
  for (size_t i = 1; i < nthd; ++i) {  
    sum_x[i] = new summ_it_t(i, nthd, n, &p, &arr, sum_x[0]->barr);
    tptr[i]=new std::thread(&summ_it_t::summ_it, *sum_x[i]);
  }
  size_t i = nthd;
  while (i) {
    i--;
    tptr[i]->join();
    delete tptr[i];
    delete sum_x[i];
  }
  std::chrono::high_resolution_clock::time_point end =
    std::chrono::high_resolution_clock::now();
  std::cerr << "time  = " <<std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
  std::cerr << "\n";  
  std::cerr << "thd ans = " << p[0] << " diff = " << (sum-p[0]) << "\n";
  delete [] sum_x;
  delete [] tptr;  
  delete [] p;
  delete [] arr;
}

int main() {
  size_t thd_szs[] = {2, 4, 8, 0};

  size_t arr_sz = (size_t)1e5;
  for (size_t k = 0; k < 3; ++k) {
    size_t j = 0;
    while (thd_szs[j] != 0) {
      size_t tsz = thd_szs[j];
      summ_test(arr_sz, tsz);
      ++j;
    }
    arr_sz = arr_sz * 100;
  }
}

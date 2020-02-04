#include <pthread.h>
#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <thread>
#include <functional>
#include <atomic>
#include <boost/thread.hpp>
using std::size_t;

constexpr auto seed = 335;

namespace putils {

double drand() {
  double x = rand();
  x = x / RAND_MAX;
  x = x + x - 1.;
  return x;
}

}  // namespace putils

struct reduce_info {
  boost::barrier *barr;
  double *ptr;
  uint32_t nsum;
};

void reduce(void *args) {
  reduce_info *info = reinterpret_cast<reduce_info*>(args);
  boost::barrier *barr = info -> barr;
  double *p = info->ptr;
  uint32_t np= info->nsum;
  if (barr->wait()) {
    double res{0.};
    for (uint32_t k = 0; k < np; ++k) res += p[k];
    p[0] = res;
  }
  barr->wait();
}


void init(size_t n, double *arr) {
  srand(335);
  for (size_t i = 0; i < n; ++i) {
    arr[i] = putils::drand();
  }
  std::cerr << "init " << n << "\n";
}


struct summ_it_t {
  uint32_t id;
  uint32_t nth;
  size_t n;
  double *p;
  double *arr;
  boost::barrier *barr;

  summ_it_t(int id_, uint32_t nth_, size_t n_, double **p_,double **arr_,boost::barrier *b_)
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
        barr(new boost::barrier(nth_)) {
  }

  ~summ_it_t() {
    barr->wait();
    if (id_==0) delete barr;
  }

/*
  void reduce() {
    double res = 0;
    for (uint32_t k = 0; k < nth; ++k) res += p[k];
    arr[0] = res;
    std::cerr << "reduced\n";
  }
*/

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
    info.barr = barr;
    info.ptr = p;
    info.nsum = nth;
    reduce((void*)&info);
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

#include <boost/thread.hpp>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <thread>
#include <vector>

template < class barrier_t >
struct sum_info_t {
  int id;
  barrier_t *b;
  double *a;
  double *psum;
  std::size_t n;
  std::size_t nth;

  explicit sum_info_t(int id_,barrier_t *b_,double *a_,double *psum_,std::size_t n_,std::size_t nth_):
    id(id_),b(b_),a(a_),psum(psum_),n(n_),nth(nth_) {}
    
  explicit sum_info_t( sum_info_t& s):
    id(s.id),b(s.b),a(s.a),psum(s.psum),n(s.n),nth(s.nth) {} 
    
  void set_id(int new_id) { id=new_id;};  

  ~sum_info_t() { 
     a=0x0;
     b=0x0;
     psum = 0x0;
  }
};


void reducer(double *part_sum, std::size_t nth) {
  double sum = 0.;
  for (std::size_t i = 0; i < nth; ++i) {
    sum += part_sum[i];
  }
  part_sum[0] = sum;
}

template <class barrier_t>
void do_sum1(void *args_in) {
  sum_info_t<barrier_t> * info = reinterpret_cast< sum_info_t<barrier_t> * >(args_in);
  int id = info->id;
  double *a = info->a;
  barrier_t *b = info->b;
  double *psum = info->psum;
  std::size_t n = info->n;
  std::size_t nth = info->nth; 

  std::size_t n0 = n / nth;
  std::size_t x0 = n % nth;
  std::size_t off = 0;
  std::size_t my_hi, my_lo;
  for (int i = 0; i < nth; ++i) {
    std::size_t lo = off;
    std::size_t hi = off + n0;
    if (x0) ++hi;
    off = hi;
    if (i == id) {
      my_lo = lo;
      my_hi = hi;
      break;
    }
  }
  double sum = 0.;
  for (std::size_t i = my_lo; i < my_hi; ++i) {
    sum += a[i];
  }
  psum[id] = sum;
  if (b->wait()) {
    sum = 0.;
    for (std::size_t k = 0; k < nth; ++k) sum += psum[k];
    psum[0] = sum;
  }
}

template <class barrier_t>
void do_sum2(void *args_in) {
  sum_info_t<barrier_t> * info = reinterpret_cast<sum_info_t<barrier_t>*>(args_in);
  int id = info->id;
  double *a = info->a;
  barrier_t *b = info->b;
  double *psum = info->psum;
  std::size_t n = info->n;
  std::size_t nth = info->nth; 

  std::size_t n0 = n / nth;
  std::size_t x0 = n % nth;
  std::size_t off = 0;
  std::size_t my_hi, my_lo;
  for (int i = 0; i < nth; ++i) {
    std::size_t lo = off;
    std::size_t hi = off + n0;
    if (x0) ++hi;
    off = hi;
    if (i == id) {
      my_lo = lo;
      my_hi = hi;
      break;
    }
  }
  double sum = 0.;
  for (std::size_t i = my_lo; i < my_hi; ++i) {
    sum += a[i];
  }
  psum[id] = sum;
  b->reduce(reducer, nth, psum);
}

namespace putils {
template <class barrier_t>
struct summer {
  summer() = delete;
  summer(const summer &s) = delete;
  summer &operator=(const summer &) = delete;
  explicit summer(std::size_t n_, std::size_t nth_)
      : arr(new double[n_]),
        part_sum(new double[nth_]),
        n(n_),
        nth(nth_) {
    srand(time(0));
    for (std::size_t i = 0; i < n_; ++i) {
      double x = double(rand()) / RAND_MAX;
      x = x + x - 1.;
      arr[i] = x;
    }
  }
  ~summer() {
    delete[] part_sum;
    delete[] arr;
  }

  void sum_it() {
    barrier_t *b=new barrier_t(nth);
    std::thread **thds = new std::thread *[nth];
    sum_info_t<barrier_t> **infos = new sum_info_t<barrier_t>*[nth];
    infos[0] = new sum_info_t<barrier_t>(0,b,arr,part_sum,n,nth);
    thds[0] =
        new std::thread(do_sum1<barrier_t>, reinterpret_cast<void*>(infos[0]));
    for (std::size_t i = 1; i < nth; ++i) {
      infos[i] = new sum_info_t<barrier_t>(*infos[0]);
      infos[i]->set_id(i);
      thds[i] =
          new std::thread(do_sum1<barrier_t>,reinterpret_cast<void*>(infos[i])); 
    }
    std::size_t j = nth;
    for (std::size_t i = 0; i < nth; ++i) {
      --j;
      thds[j]->join();
      delete thds[j];
      delete infos[j];
    }
    delete[] infos;
    delete[] thds;
    delete b;
  }

  double expected() const noexcept {
    double sum = 0;
    for (std::size_t i = 0; i < n; ++i) sum += arr[i];
    return sum;
  }

  double answer() const noexcept { return part_sum[0]; }

  double *arr;
  double *part_sum;
  std::size_t n;
  std::size_t nth;
};
}  // namespace putils

template <class barrier_t>
void test_it(std::size_t n, std::size_t nthreads) {
  putils::summer<barrier_t> s(n, nthreads);
  auto st = std::chrono::high_resolution_clock::now();
  s.sum_it();
  auto fn = std::chrono::high_resolution_clock::now();
  auto t =
      std::chrono::duration_cast<std::chrono::microseconds>(fn - st).count();
  double tx = double(t)*0.001;
  double ans = s.answer();
  double expected = s.expected();
  std::cerr << std::scientific << std::setprecision(3) << std::setw(15) << tx
            << " n = " << std::setw(12) << n << " nth = " << std::setw(2) << nthreads << " ans = " 
            << std::scientific << std::setprecision(5) << std::setw(15) << ans
            << " expected = " 
            << std::scientific << std::setprecision(5) << std::setw(15) << expected            
            << " diff " 
            << std::scientific << std::setprecision(5) << std::setw(15) << (ans - expected) 
            << "\n";
}

int main() {
  std::vector<std::size_t> nthds = {1, 2, 4, 8};
  std::vector<std::size_t> nsize = {100,     1000,      10000,
                                    1000000, 100000000, 1000000000};

  for (auto n : nsize) {
    for (auto nth : nthds) {
      test_it<boost::barrier>(n, nth);
    }
  }
  return EXIT_SUCCESS;
}

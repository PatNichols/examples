#ifndef TEST_FFI_H
#define TEST_FFI_H

#include <cstdint>
#include <cstdlib>
#include <array>
#include <iostream>

struct cpp_test {
    std::array<double,24> a;
    std::array<double,12> b;
    std::array<double,4> c;
    double *d;
    
    cpp_test() {
        double acnt =0.;
        double bcnt =0.;
        double ccnt =0.;
        for (std::int64_t i=0;i<4;++i) {
          for (std::int64_t j=0;j<3;++j) {
            for (std::int64_t k=0;k<2;++k) {
              a[k+j*2+i*6]= acnt;
              acnt += 1.0;
            }
          }
        }
        for (std::int64_t i=0;i<4;++i) {
          for (std::int64_t j=0;j<3;++j) {
            b[j+i*3] = bcnt;
            bcnt += 1.0;
          }
        }
        for (std::int64_t i=0;i<4;++i) {
          c[i] = ccnt;
          ccnt += 1.0;
        }
        d = new double(2.4);
    }

    ~cpp_test() {
        delete d;
    }
    
    void set_a_value(double x,std::int64_t i,std::int64_t j,std::int64_t k) {
        std::size_t index = i + j * 2 + k * 6;
        a[index] = x; 
    }
    void set_b_value(double x,std::int64_t i,std::int64_t j) {
        b[i + j*3 ] = x; 
    }
    void set_c_value(double x,std::int64_t i) {
        c[i] = x; 
    }
    void set_d_value(double x) {
        *d = x; 
    }
    double get_a_value(std::int64_t i,std::int64_t j,std::int64_t k) const {
        std::size_t index = i + j * 2 + k * 6;
        return a[index];
    }
    double get_b_value(std::int64_t i,std::int64_t j) const {
        return b[i+j*3];
    }
    double get_c_value(std::int64_t i) const { return c[i];}
    double get_d_value() const { return *d;}

    auto get_a_pointer() { return a.data();}
    auto get_b_pointer() { return b.data();}
    auto get_c_pointer() { return c.data();}
    auto get_d_pointer() { return d;}
};


extern "C" {

typedef struct cpp_test_impl_t {
  double *a;
  double *b;
  double *c;
  double *d;
}  cpp_test_impl_t;

void  *cpp_test_impl_t_init();
void cpp_test_impl_t_free(cpp_test_impl_t **obj);
void cpp_test_impl_t_set_a_value(cpp_test_impl_t *obj,double x,int64_t i,int64_t j,int64_t k);
void cpp_test_impl_t_set_b_value(cpp_test_impl_t *obj,double x,int64_t i,int64_t j);
void cpp_test_impl_t_set_c_value(cpp_test_impl_t *obj,double x,int64_t i);
void cpp_test_impl_t_set_d_value(cpp_test_impl_t *obj,double x);
double cpp_test_impl_t_get_a_value(cpp_test_impl_t *obj,int64_t i,int64_t j,int64_t k);
double cpp_test_impl_t_get_b_value(cpp_test_impl_t *obj,int64_t i,int64_t j);
double cpp_test_impl_t_get_c_value(cpp_test_impl_t *obj,int64_t i);
double cpp_test_impl_t_get_d_value(cpp_test_impl_t *obj);
double * cpp_test_impl_t_get_a_pointer(cpp_test_impl_t *obj);
double * cpp_test_impl_t_get_b_pointer(cpp_test_impl_t *obj);
double * cpp_test_impl_t_get_c_pointer(cpp_test_impl_t *obj);
double * cpp_test_impl_t_get_d_pointer(cpp_test_impl_t *obj);

}
#endif

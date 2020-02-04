#include "test_ffi.h"

extern "C" {

void  *cpp_test_impl_t_init()
{
    return reinterpret_cast<cpp_test_impl_t*>(new cpp_test());    
}

void cpp_test_impl_t_free(cpp_test_impl_t **obj)
{
    reinterpret_cast<cpp_test*>(*obj)->~cpp_test();
    *obj = 0;
}

void cpp_test_impl_t_set_a_value(cpp_test_impl_t *obj,double x,int64_t i,int64_t j,int64_t k)
{
    cpp_test * cl = reinterpret_cast< cpp_test * >( obj);
    cl -> set_a_value(x,i,j,k);
}

void cpp_test_impl_t_set_b_value(cpp_test_impl_t *obj,double x,int64_t i,int64_t j)
{
    cpp_test * cl = reinterpret_cast< cpp_test * >( obj);
    cl -> set_b_value(x,i,j);
}

void cpp_test_impl_t_set_c_value(cpp_test_impl_t *obj,double x,int64_t i)
{
    cpp_test * cl = reinterpret_cast< cpp_test * >( obj);
    cl -> set_c_value(x,i);
}

void cpp_test_impl_t_set_d_value(cpp_test_impl_t *obj,double x)
{
    cpp_test * cl = reinterpret_cast< cpp_test * >( obj);
    cl -> set_d_value(x);
}

double cpp_test_impl_t_get_a_value(cpp_test_impl_t *obj,int64_t i,int64_t j,int64_t k)
{
    cpp_test * cl = reinterpret_cast< cpp_test * >( obj);
    return  cl -> get_a_value(i,j,k);
}

double cpp_test_impl_t_get_b_value(cpp_test_impl_t *obj,int64_t i,int64_t j)
{
    cpp_test * cl = reinterpret_cast< cpp_test * >( obj);
    return  cl -> get_b_value(i,j);
}

double cpp_test_impl_t_get_c_value(cpp_test_impl_t *obj,int64_t i)
{
    cpp_test * cl = reinterpret_cast< cpp_test * >( obj);
    return  cl -> get_c_value(i);
}

double cpp_test_impl_t_get_d_value(cpp_test_impl_t *obj)
{
    cpp_test * cl = reinterpret_cast< cpp_test * >( obj);
    return  cl -> get_d_value();
}

double * cpp_test_impl_t_get_a_pointer(cpp_test_impl_t *obj)
{
    cpp_test * cl = reinterpret_cast< cpp_test * >( obj);
    return  cl -> get_a_pointer();
}

double * cpp_test_impl_t_get_b_pointer(cpp_test_impl_t *obj)
{
    cpp_test * cl = reinterpret_cast< cpp_test * >( obj);
    return  cl -> get_b_pointer();
}

double * cpp_test_impl_t_get_c_pointer(cpp_test_impl_t *obj)
{
    cpp_test * cl = reinterpret_cast< cpp_test * >( obj );
    return  cl -> get_c_pointer();
}

double * cpp_test_impl_t_get_d_pointer(cpp_test_impl_t *obj)
{
    cpp_test * cl = reinterpret_cast< cpp_test * >( obj);
    return  cl -> get_d_pointer();
}

}


#ifndef PETLIB_OPS_HPP
#define PETLIB_OPS_HPP
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include "petlib_iterator.hpp"

namespace petlib {

template < class array_t,  typename T >
struct array_base
{
    typedef T value_type;
    typedef T* pointer;
    typedef std::size_t size_type;
     
    const array_t* cast_down() const noexcept {
        return static_cast< const array_t * >(this);
    }
    constexpr value_type operator[](size_type i) const noexcept {
        return cast_down()->operator[](i);
    }
    constexpr size_type size() const noexcept { return cast_down()->size();}
    constexpr size_type stride() const noexcept { return cast_down()->stride();} 
    constexpr pointer base() const noexcept { return cast_down()->base();}
};


template < class array_t, typename T >
struct array_ref
{
    typedef T value_type;
    typedef std::size_t size_type;

    const array_t& a;
     
    array_ref(const array_base<array_t,T>& a0):a(*a0.cast_down()) {}
    
    value_type operator[](size_type i) const noexcept {
        return a[i];
    }
    size_type size() const noexcept { return a.size();}
};

template < class xpr_t , typename T >
struct array_xpr {
    typedef T value_type;
    typedef std::size_t size_type;
    const xpr_t a;

    array_xpr(const xpr_t& x):a(x) {}
    
    value_type operator[](size_type i) const noexcept {
        return a[i];
    }
    size_type size() const noexcept { return a.size();}
};

template < class Op, class A, class B >
struct array_bin_xpr {
    typedef typename Op::value_type value_type;
    typedef std::size_t size_type;

    const A a;
    const B b;
    
    array_bin_xpr(const A& a0,const B& b0):a(a0),b(b0) {}
    
     value_type operator[](size_type i) const noexcept {
        return Op::eval(a[i],b[i]);
    }
     size_type size() const noexcept { return a.size();}
};


template < class Op, class A >
struct array_un_xpr {
    typedef typename Op::value_type value_type;
    typedef std::size_t size_type;

    const A a;
    
    array_un_xpr(const A& a0):a(a0) {}
    
    value_type operator[](size_type i) const noexcept {
        return Op::eval(a[i]);
    }
    size_type size() const noexcept { return a.size();}
};

template < typename T> struct scalar_ref {
    typedef T value_type;
    typedef std::size_t size_type;
    const T x;
    
    scalar_ref(const T& x0):x(x0) {}
    value_type operator[](size_type i) const noexcept { return x;}
    value_type operator()(size_type i,size_type j) const noexcept { return x;}   
};

#define make_op(name_,sym_)\
template < class A, class B, typename T > \
constexpr array_xpr< array_bin_xpr < name_##Op<T>, array_ref<A,T>, array_ref<B,T> >, T > \
operator sym_ ( const array_base<A,T>& a, const array_base<B,T>& b) noexcept {\
  typedef array_bin_xpr< name_##Op<T>, array_ref<A,T>, array_ref<B,T> > xpr_t;\
  return array_xpr< xpr_t, T>(xpr_t(array_ref<A,T>(a),array_ref<B,T>(b))); \
} \
 \
template < class A, class B, typename T > \
constexpr array_xpr< array_bin_xpr < name_##Op<T>, array_ref<A,T>, array_xpr<B,T> >, T > \
operator sym_ ( const array_base<A,T>& a, const array_xpr<B,T>& b) noexcept {\
  typedef array_bin_xpr< name_##Op<T>, array_ref<A,T>, array_xpr<B,T> > xpr_t;\
  return array_xpr< xpr_t, T>(xpr_t(array_ref<A,T>(a),b)); \
} \
 \
template < class A, class B, typename T > \
constexpr array_xpr< array_bin_xpr < name_##Op<T>, array_xpr<A,T>, array_ref<B,T> >, T > \
operator sym_ ( const array_xpr<A,T>& a, const array_base<B,T>& b) noexcept {\
  typedef array_bin_xpr< name_##Op<T>, array_xpr<A,T>, array_ref<B,T> > xpr_t;\
  return array_xpr< xpr_t, T>(xpr_t(a,array_ref<B,T>(b))); \
} \
 \
template < class A, class B, typename T > \
constexpr array_xpr< array_bin_xpr < name_##Op<T>, array_xpr<A,T>, array_xpr<B,T> >, T > \
operator sym_ ( const array_xpr<A,T>& a, const array_xpr<B,T>& b) noexcept {\
  typedef array_bin_xpr< name_##Op<T>, array_xpr<A,T>, array_xpr<B,T> > xpr_t;\
  return array_xpr< xpr_t, T>(xpr_t(a,b)); \
} \
 \
template < class A, typename T > \
constexpr array_xpr< array_bin_xpr < name_##Op<T>, array_ref<A,T>, scalar_ref<T> >, T > \
operator sym_ ( const array_base<A,T>& a, const T b) noexcept {\
  typedef array_bin_xpr< name_##Op<T>, array_ref<A,T>, scalar_ref<T> > xpr_t;\
  return array_xpr< xpr_t, T>(xpr_t(array_ref<A,T>(a),scalar_ref<T>(b))); \
} \
 \
template < class A, typename T > \
constexpr array_xpr< array_bin_xpr < name_##Op<T>, array_xpr<A,T>, scalar_ref<T> >, T > \
operator sym_ ( const array_xpr<A,T>& a, const T b) noexcept {\
  typedef array_bin_xpr< name_##Op<T>, array_xpr<A,T>, scalar_ref<T> > xpr_t;\
  return array_xpr< xpr_t, T>(xpr_t(a,scalar_ref<T>(b))); \
} \
 \
template < class A, typename T > \
constexpr array_xpr< array_bin_xpr < name_##RevOp<T>, array_ref<A,T>, scalar_ref<T> >, T > \
operator sym_ ( const T b,const array_base<A,T>& a) noexcept {\
  typedef array_bin_xpr< name_##RevOp<T>, array_ref<A,T>, scalar_ref<T> > xpr_t;\
  return array_xpr< xpr_t, T>(xpr_t(array_ref<A,T>(a),scalar_ref<T>(b))); \
} \
 \
template < class A, typename T > \
constexpr array_xpr< array_bin_xpr < name_##RevOp<T>, array_xpr<A,T>, scalar_ref<T> >, T > \
operator sym_ ( const T b, const array_xpr<A,T>& a) noexcept {\
  typedef array_bin_xpr< name_##RevOp<T>, array_xpr<A,T>, scalar_ref<T> > xpr_t;\
  return array_xpr< xpr_t, T>(xpr_t(a,scalar_ref<T>(b))); \
} 
 
make_op(Add,+)
make_op(Sub,-)
make_op(Mul,*)
make_op(Div,/)
make_op(Xor,^)
make_op(Ior,|)
make_op(And,&)
make_op(Lsh,<<)
make_op(Rsh,>>)
make_op(Eq,==)
make_op(Neq,!=)
make_op(LtEq,<=)
make_op(GtEq,>=)
make_op(Gt,>)
make_op(Lt,<)
make_op(LogAnd,&&)
make_op(LogOr,||)
#undef make_op

#define make_op(name_,sym_)\
template < class A, typename T > \
constexpr array_xpr < array_un_xpr < name_##Op<T>, array_ref<A,T> >, T > \
operator sym_ (const array_base<A,T>& a) {\
 typedef array_un_xpr< name_##Op<T>, array_ref<A,T> > xpr_t;\
 return array_xpr< xpr_t , T>(xpr_t(array_ref<A,T>(a))); \
} \
 \
template < class A, typename T > \
constexpr array_xpr < array_un_xpr < name_##Op<T>, array_xpr<A,T> >, T > \
operator sym_ (const array_xpr<A,T>& a) {\
 typedef array_un_xpr< name_##Op<T>, array_xpr<A,T> > xpr_t;\
 return array_xpr< xpr_t , T>(xpr_t(a)); \
} 
make_op(Com,~)
make_op(Neg,-)
make_op(Pos,+)
make_op(Not,!)
#undef make_op
  
template < class A, typename T > 
std::ostream& operator << ( std::ostream& os, const array_base<A,T>& a) { 
   std::size_t asize = a.size();
   
   if (asize<6) {
      os << "( " << a[0];
      for (std::size_t i=1;i<asize;++i) {
          os << ", " << a[i];
      }
      os << " )\n";
      return os;
   }else{
      os << "----------------------------\n";
      os << "index           value \n"; 
      os << "----------------------------\n";
      for (std::size_t i=0;i<asize;++i) {
          os << std::setw(12) << i << " " << a[i] << "\n";
      }
      os << "----------------------------\n";
      return os;
   }
}

template < class A, typename T > 
std::ostream& operator << ( std::ostream& os, const array_xpr<A,T>& a) { 
   std::size_t asize = a.size();
   
   if (asize<6) {
      os << "( " << a[0];
      for (std::size_t i=1;i<asize;++i) {
          os << ", " << a[i];
      }
      os << " )\n";
      return os;
   }else{
      os << "----------------------------\n";
      os << "index           value \n"; 
      os << "----------------------------\n";
      for (std::size_t i=0;i<asize;++i) {
          os << std::setw(12) << i << " " << a[i] << "\n";
      }
      os << "----------------------------\n";
      return os;
   }
}
 
} 
#endif
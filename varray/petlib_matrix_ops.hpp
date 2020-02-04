#ifndef PETLIB_MATRIX_OPS_HPP
#define PETLIB_MATRIX_OPS_HPP
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include "petlib_iterator.hpp"
#include "petlib_ops.hpp"

namespace petlib {

template < class matrix_t,  typename T >
struct matrix_base
{
    typedef T value_type;
    typedef T* pointer;
    typedef std::size_t size_type;
     
    const matrix_t* cast_down() const noexcept {
        return static_cast< const matrix_t * >(this);
    }
    constexpr value_type operator()(size_type i,size_t j) const noexcept {
        return cast_down()->operator()(i,j);
    }
    constexpr size_type extent1() const noexcept { return cast_down()->extent1();};
    constexpr size_type extent2() const noexcept { return cast_down()->extent2();};
    constexpr size_type size() const noexcept { return cast_down()->size();}
    constexpr size_type stride() const noexcept { return cast_down()->stride();} 
    constexpr pointer base() const noexcept { return cast_down()->base();}
};


template < class matrix_t, typename T >
struct matrix_ref
{
    typedef T value_type;
    typedef std::size_t size_type;

    const matrix_t& a;
     
    matrix_ref(const matrix_base<matrix_t,T>& a0):a(*a0.cast_down()) {}
    

    constexpr value_type operator()(size_type i,size_t j) const noexcept {
        return a(i,j);
    }
    constexpr size_type extent1() const noexcept { return a.extent1();};
    constexpr size_type extent2() const noexcept { return a.extent2();};
    constexpr size_type size() const noexcept { return a.size();}
};

template < class xpr_t , typename T >
struct matrix_xpr {
    typedef T value_type;
    typedef std::size_t size_type;
    const xpr_t a;

    matrix_xpr(const xpr_t& x):a(x) {}

    constexpr value_type operator()(size_type i,size_t j) const noexcept {
        return a(i,j);
    }
    constexpr size_type extent1() const noexcept { return a.extent1();};
    constexpr size_type extent2() const noexcept { return a.extent2();};
    constexpr size_type size() const noexcept { return a.size();}
};

template < class Op, class A, class B >
struct matrix_bin_xpr {
    typedef typename Op::value_type value_type;
    typedef std::size_t size_type;

    const A a;
    const B b;
    
    matrix_bin_xpr(const A& a0,const B& b0):a(a0),b(b0) {}
    
    constexpr value_type operator()(size_type i,size_t j) const noexcept {
        return Op::eval(a(i,j),b(i,j));
    }
    constexpr size_type extent1() const noexcept { return a.extent1();};
    constexpr size_type extent2() const noexcept { return a.extent2();};
    constexpr size_type size() const noexcept { return a.size();}
};


template < class Op, class A >
struct matrix_un_xpr {
    typedef typename Op::value_type value_type;
    typedef std::size_t size_type;

    const A a;
    
    matrix_un_xpr(const A& a0):a(a0) {}
    
    constexpr value_type operator()(size_type i,size_t j) const noexcept {
        return Op::eval(a(i,j));
    }
    constexpr size_type extent1() const noexcept { return a.extent1();};
    constexpr size_type extent2() const noexcept { return a.extent2();};
    constexpr size_type size() const noexcept { return a.size();}
};


#define make_op(name_,sym_)\
template < class A, class B, typename T > \
constexpr matrix_xpr< matrix_bin_xpr < name_##Op<T>, matrix_ref<A,T>, matrix_ref<B,T> >, T > \
operator sym_ ( const matrix_base<A,T>& a, const matrix_base<B,T>& b) noexcept {\
  typedef matrix_bin_xpr< name_##Op<T>, matrix_ref<A,T>, matrix_ref<B,T> > xpr_t;\
  return matrix_xpr< xpr_t, T>(xpr_t(matrix_ref<A,T>(a),matrix_ref<B,T>(b))); \
} \
 \
template < class A, class B, typename T > \
constexpr matrix_xpr< matrix_bin_xpr < name_##Op<T>, matrix_ref<A,T>, matrix_xpr<B,T> >, T > \
operator sym_ ( const matrix_base<A,T>& a, const matrix_xpr<B,T>& b) noexcept {\
  typedef matrix_bin_xpr< name_##Op<T>, matrix_ref<A,T>, matrix_xpr<B,T> > xpr_t;\
  return matrix_xpr< xpr_t, T>(xpr_t(matrix_ref<A,T>(a),b)); \
} \
 \
template < class A, class B, typename T > \
constexpr matrix_xpr< matrix_bin_xpr < name_##Op<T>, matrix_xpr<A,T>, matrix_ref<B,T> >, T > \
operator sym_ ( const matrix_xpr<A,T>& a, const matrix_base<B,T>& b) noexcept {\
  typedef matrix_bin_xpr< name_##Op<T>, matrix_xpr<A,T>, matrix_ref<B,T> > xpr_t;\
  return matrix_xpr< xpr_t, T>(xpr_t(a,matrix_ref<B,T>(b))); \
} \
 \
template < class A, class B, typename T > \
constexpr matrix_xpr< matrix_bin_xpr < name_##Op<T>, matrix_xpr<A,T>, matrix_xpr<B,T> >, T > \
operator sym_ ( const matrix_xpr<A,T>& a, const matrix_xpr<B,T>& b) noexcept {\
  typedef matrix_bin_xpr< name_##Op<T>, matrix_xpr<A,T>, matrix_xpr<B,T> > xpr_t;\
  return matrix_xpr< xpr_t, T>(xpr_t(a,b)); \
} \
 \
template < class A, typename T > \
constexpr matrix_xpr< matrix_bin_xpr < name_##Op<T>, matrix_ref<A,T>, scalar_ref<T> >, T > \
operator sym_ ( const matrix_base<A,T>& a, const T b) noexcept {\
  typedef matrix_bin_xpr< name_##Op<T>, matrix_ref<A,T>, scalar_ref<T> > xpr_t;\
  return matrix_xpr< xpr_t, T>(xpr_t(matrix_ref<A,T>(a),scalar_ref<T>(b))); \
} \
 \
template < class A, typename T > \
constexpr matrix_xpr< matrix_bin_xpr < name_##Op<T>, matrix_xpr<A,T>, scalar_ref<T> >, T > \
operator sym_ ( const matrix_xpr<A,T>& a, const T b) noexcept {\
  typedef matrix_bin_xpr< name_##Op<T>, matrix_xpr<A,T>, scalar_ref<T> > xpr_t;\
  return matrix_xpr< xpr_t, T>(xpr_t(a,scalar_ref<T>(b))); \
} \
 \
template < class A, typename T > \
constexpr matrix_xpr< matrix_bin_xpr < name_##RevOp<T>, matrix_ref<A,T>, scalar_ref<T> >, T > \
operator sym_ ( const T b,const matrix_base<A,T>& a) noexcept {\
  typedef matrix_bin_xpr< name_##RevOp<T>, matrix_ref<A,T>, scalar_ref<T> > xpr_t;\
  return matrix_xpr< xpr_t, T>(xpr_t(matrix_ref<A,T>(a),scalar_ref<T>(b))); \
} \
 \
template < class A, typename T > \
constexpr matrix_xpr< matrix_bin_xpr < name_##RevOp<T>, matrix_xpr<A,T>, scalar_ref<T> >, T > \
operator sym_ ( const T b, const matrix_xpr<A,T>& a) noexcept {\
  typedef matrix_bin_xpr< name_##RevOp<T>, matrix_xpr<A,T>, scalar_ref<T> > xpr_t;\
  return matrix_xpr< xpr_t, T>(xpr_t(a,scalar_ref<T>(b))); \
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
constexpr matrix_xpr < matrix_un_xpr < name_##Op<T>, matrix_ref<A,T> >, T > \
operator sym_ (const matrix_base<A,T>& a) {\
 typedef matrix_un_xpr< name_##Op<T>, matrix_ref<A,T> > xpr_t;\
 return matrix_xpr< xpr_t , T>(xpr_t(matrix_ref<A,T>(a))); \
} \
 \
template < class A, typename T > \
constexpr matrix_xpr < matrix_un_xpr < name_##Op<T>, matrix_xpr<A,T> >, T > \
operator sym_ (const matrix_xpr<A,T>& a) {\
 typedef matrix_un_xpr< name_##Op<T>, matrix_xpr<A,T> > xpr_t;\
 return matrix_xpr< xpr_t , T>(xpr_t(a)); \
} 
make_op(Com,~)
make_op(Neg,-)
make_op(Pos,+)
make_op(Not,!)
#undef make_op
  
template < class A, typename T > 
std::ostream& operator << ( std::ostream& os, const matrix_base<A,T>& a) { 
   std::size_t rsize = a.extent1();
   std::size_t csize = a.extent2();   
   if (csize<6) {
      for (std::size_t j=0;j<rsize;++j) {
      os << "| " << a(j,0);
      for (std::size_t i=1;i<csize;++i) {
          os << " " << a(j,i);
      }
      os << " |\n";
      }
      return os;
   }else{
      os << "----------------------------\n";
      os << "index-1      index-2           value \n"; 
      os << "----------------------------\n";
      for (std::size_t i=0;i<rsize;++i) {
      for (std::size_t j=0;j<csize;++j) {
          os << std::setw(12) << i << " " 
             << std::setw(12) << j << " "
             << a(i,j) << "\n";
      }
      }
      os << "----------------------------\n";
      return os;
   }
}

template < class A, typename T > 
std::ostream& operator << ( std::ostream& os, const matrix_xpr<A,T>& a) { 
   std::size_t rsize = a.extent1();
   std::size_t csize = a.extent2();   
   if (csize<6) {
      for (std::size_t j=0;j<rsize;++j) {
      os << "| " << a(j,0);
      for (std::size_t i=1;i<csize;++i) {
          os << " " << a(j,i);
      }
      os << " |\n";
      }
      return os;
   }else{
      os << "----------------------------\n";
      os << "index-1      index-2           value \n"; 
      os << "----------------------------\n";
      for (std::size_t i=0;i<rsize;++i) {
      for (std::size_t j=0;j<csize;++j) {
          os << std::setw(12) << i << " " 
             << std::setw(12) << j << " "
             << a(i,j) << "\n";
      }
      }
      os << "----------------------------\n";
      return os;
   }
}
 
} 
#endif
#ifndef PETLIB_ITERATOR_HPP
#define PETLIB_ITERATOR_HPP
#include <cstdlib>
#include <cstdint>
#include <iterator>
#include <cmath>
#include <random>
#include <ctime>
namespace petlib {

template < typename T >
struct slice_iterator {
   typedef T& reference;
   typedef T* pointer;
   typedef std::size_t size_type;
   typedef std::ptrdiff_t difference_type;
   typedef T value_type;
   typedef std::random_access_iterator_tag iterator_category;

   pointer ptr_;
   size_type str_;
   
   slice_iterator(pointer p,size_type stride_=size_type(1)):ptr_(p),str_(stride_) {}
   
   slice_iterator(slice_iterator<T>&& iter):ptr_(iter.ptr_),str_(iter.str_) {
       iter.ptr_ = nullptr;
       iter.str_ = 0;
   }
   slice_iterator(const slice_iterator<T>& iter):ptr_(iter.ptr_),str_(iter.str_) {
   }
   slice_iterator():ptr_(nullptr),str_(0) {}

   ~slice_iterator() { ptr_ = nullptr; str_=0; }
   
   slice_iterator& operator=(const slice_iterator<T>& iter) {
       ptr_=iter.ptr_;
       str_=iter.str_;
       return *this;
   }
   slice_iterator& operator=(slice_iterator<T>&& iter) {
       ptr_=iter.ptr_;
       str_=iter.str_;
       iter.ptr_ = nullptr;
       iter.str_ = 0;
       return *this;
   }
   slice_iterator& operator=(slice_iterator<T>& iter) {
       ptr_=iter.ptr_;
       str_=iter.str_;
       return *this;      
   }
   constexpr slice_iterator<T>& operator+=(difference_type n) {
       ptr_+=n*str_;
       return *this;
   }
   constexpr slice_iterator<T>& operator-=(difference_type n) {
       ptr_-=n*str_;
       return *this;
   }
   constexpr slice_iterator<T>& operator+=(size_type n) {
       ptr_+=n*str_;
       return *this;
   }
   constexpr slice_iterator<T>& operator-=(size_type n) {
       ptr_-=n*str_;
       return *this;
   }
   constexpr value_type operator[](size_type n) const noexcept { return ptr_[n*str_];}
   constexpr reference operator[](size_type n) noexcept { return ptr_[n*str_];}
   constexpr reference operator*() { return *ptr_;}
   constexpr reference operator->() { return *ptr_;}
   constexpr pointer base() noexcept { return ptr_;}
   constexpr pointer base() const noexcept { return ptr_;}
   constexpr size_type stride() const noexcept { return str_;}
        
   constexpr slice_iterator<T>& operator++() noexcept { ptr_+=str_; return *this;}     
   constexpr slice_iterator<T>& operator--() noexcept { ptr_-=str_; return *this;}     
   constexpr slice_iterator<T>& operator++(int) noexcept { 
       slice_iterator<T> tmp=(*this);
       ptr_+=str_; 
       return tmp;
   }     
   constexpr slice_iterator<T>& operator--(int) noexcept {  
       slice_iterator<T> tmp=*this;
       ptr_-=str_; 
       return tmp;
   }     
   constexpr bool operator==(const slice_iterator<T>& iter) const noexcept {
       return (ptr_==iter.ptr_);
   } 
   constexpr bool operator!=(const slice_iterator<T>& iter) const noexcept {
       return (ptr_!=iter.ptr_);
   } 
   constexpr bool operator>=(const slice_iterator<T>& iter) const noexcept {
       return (ptr_>=iter.ptr_);
   } 
   constexpr bool operator<=(const slice_iterator<T>& iter) const noexcept {
       return (ptr_<=iter.ptr_);
   } 
   constexpr bool operator>(const slice_iterator<T>& iter) const noexcept {
       return (ptr_>iter.ptr_);
   } 
   constexpr bool operator<(const slice_iterator<T>& iter) const noexcept {
       return (ptr_<iter.ptr_);
   } 
};

template < typename T > 
constexpr slice_iterator<T> operator+(const slice_iterator<T>& iter,std::ptrdiff_t n) noexcept
{
    return slice_iterator<T>(iter.base()+n*iter.stride(),iter.stride());
}

template < typename T > 
constexpr slice_iterator<T> operator+(std::ptrdiff_t n,const slice_iterator<T>& iter) noexcept
{
    return slice_iterator<T>(iter.base()+n*iter.stride(),iter.stride());
}


template < typename T > 
constexpr slice_iterator<T> operator+(const slice_iterator<T>& iter,std::size_t n) noexcept
{
    return slice_iterator<T>(iter.base()+n*iter.stride(),iter.stride());
}

template < typename T > 
constexpr slice_iterator<T> operator+(std::size_t n,const slice_iterator<T>& iter) noexcept
{
    return slice_iterator<T>(iter.base()+n*iter.stride(),iter.stride());
}

template < typename T >
constexpr std::ptrdiff_t operator-(const slice_iterator<T>& a,const slice_iterator<T>& b) noexcept
{
    return std::ptrdiff_t(a.base()-b.base())/std::ptrdiff_t(a.stride()*sizeof(T));
}

template < typename T >
constexpr bool operator==(const slice_iterator<T>& a,const slice_iterator<T>& b) noexcept
{
    return (a.base()==b.base());
}

template < typename T >
constexpr bool operator!=(const slice_iterator<T>& a,const slice_iterator<T>& b) noexcept
{
    return (a.base()!=b.base());
}

template < typename T >
constexpr bool operator>=(const slice_iterator<T>& a,const slice_iterator<T>& b) noexcept
{
    return (a.base()>=b.base());
}

template < typename T >
constexpr bool operator<=(const slice_iterator<T>& a,const slice_iterator<T>& b) noexcept
{
    return (a.base()<=b.base());
}

template < typename T >
constexpr bool operator>(const slice_iterator<T>& a,const slice_iterator<T>& b) noexcept
{
    return (a.base()>b.base());
}

template < typename T >
constexpr bool operator<(const slice_iterator<T>& a,const slice_iterator<T>& b) noexcept
{
    return (a.base()<b.base());
}

template < typename T >
struct array_iterator {
   typedef T& reference;
   typedef T* pointer;
   typedef std::size_t size_type;
   typedef std::ptrdiff_t difference_type;
   typedef T value_type;
   typedef std::random_access_iterator_tag iterator_category;
   
   pointer ptr_;
   
   array_iterator(pointer p):ptr_(p) {}
   
   array_iterator(array_iterator<T>&& iter):ptr_(iter.ptr_) {
       iter.ptr_ = nullptr;
   }
   array_iterator(const array_iterator<T>& iter):ptr_(iter.ptr_) {
   }
   array_iterator():ptr_(nullptr) {}

   ~array_iterator() { ptr_ = nullptr;}
   
   
   array_iterator& operator=(const array_iterator<T>& iter) {
       ptr_=iter.ptr_;
       return *this;
   }
   array_iterator& operator=(array_iterator<T>&& iter) {
       ptr_=iter.ptr_;
       iter.ptr_ = nullptr;
       return *this;
   }
   array_iterator& operator=(array_iterator<T>& iter) {
       ptr_=iter.ptr_;
       return *this;      
   }
   constexpr array_iterator<T>& operator+=(difference_type n) {
       ptr_+=n;
       return *this;
   }
   constexpr array_iterator<T>& operator-=(difference_type n) {
       ptr_-=n;
       return *this;
   }
   constexpr value_type operator[](size_type n) const noexcept { return ptr_[n];}
   constexpr reference operator[](size_type n) noexcept { return ptr_[n];}
   constexpr reference operator*() { return *ptr_;}
   constexpr reference operator->() { return *ptr_;}
   constexpr pointer base() noexcept { return ptr_;}
   constexpr pointer base() const noexcept { return ptr_;}
   constexpr size_type stride() const noexcept { return size_type(1);}
        
   constexpr array_iterator<T>& operator++() noexcept { ++ptr_; return *this;}     
   constexpr array_iterator<T>& operator--() noexcept { --ptr_; return *this;}     
   constexpr array_iterator<T>& operator++(int) noexcept { 
       array_iterator<T> tmp=(*this);
       ++ptr_; 
       return tmp;
   }     
   constexpr array_iterator<T>& operator--(int) noexcept {  
       array_iterator<T> tmp=*this;
       --ptr_; 
       return tmp;
   }     
   constexpr bool operator==(const array_iterator<T>& iter) const noexcept {
       return (ptr_==iter.ptr_);
   } 
   constexpr bool operator!=(const array_iterator<T>& iter) const noexcept {
       return (ptr_!=iter.ptr_);
   } 
   constexpr bool operator>=(const array_iterator<T>& iter) const noexcept {
       return (ptr_>=iter.ptr_);
   } 
   constexpr bool operator<=(const array_iterator<T>& iter) const noexcept {
       return (ptr_<=iter.ptr_);
   } 
   constexpr bool operator>(const array_iterator<T>& iter) const noexcept {
       return (ptr_>iter.ptr_);
   } 
   constexpr bool operator<(const array_iterator<T>& iter) const noexcept {
       return (ptr_<iter.ptr_);
   } 
};

template < typename T > 
constexpr array_iterator<T> operator+(const array_iterator<T>& iter,std::ptrdiff_t n) noexcept
{
    return array_iterator<T>(iter.base()+n,iter.stride());
}

template < typename T > 
constexpr array_iterator<T> operator+(std::ptrdiff_t n,const array_iterator<T>& iter) noexcept
{
    return array_iterator<T>(iter.base()+n,iter.stride());
}

template < typename T >
constexpr std::ptrdiff_t operator-(const array_iterator<T>& a,const array_iterator<T>& b) noexcept
{
    return std::ptrdiff_t(a.base()-b.base());
}

template < typename T >
constexpr bool operator==(const array_iterator<T>& a,const array_iterator<T>& b) noexcept
{
    return (a.base()==b.base());
}

template < typename T >
constexpr bool operator!=(const array_iterator<T>& a,const array_iterator<T>& b) noexcept
{
    return (a.base()!=b.base());
}

template < typename T >
constexpr bool operator>=(const array_iterator<T>& a,const array_iterator<T>& b) noexcept
{
    return (a.base()>=b.base());
}

template < typename T >
constexpr bool operator<=(const array_iterator<T>& a,const array_iterator<T>& b) noexcept
{
    return (a.base()<=b.base());
}

template < typename T >
constexpr bool operator>(const array_iterator<T>& a,const array_iterator<T>& b) noexcept
{
    return (a.base()>b.base());
}

template < typename T >
constexpr bool operator<(const array_iterator<T>& a,const array_iterator<T>& b) noexcept
{
    return (a.base()<b.base());
}

template < class src_iter_t, class dst_iter_t>
dst_iter_t copy(dst_iter_t dst_iter, dst_iter_t last, src_iter_t src_iter)
{
    while (dst_iter!=last) {
        *dst_iter = *src_iter;
        ++src_iter;
        ++dst_iter;
    }
    return dst_iter;
}

#define PETLIB_MAKE_OP(name_,sym_)\
template < class src_iter_t, class dst_iter_t > \
dst_iter_t name_##Eq(dst_iter_t dst_iter,dst_iter_t last,src_iter_t src_iter) {\
  while (dst_iter!=last) { \
    *dst_iter sym_ *src_iter;\
    ++dst_iter;\
    ++src_iter;\
  } \
  return dst_iter;\
}

PETLIB_MAKE_OP(add,+=)
PETLIB_MAKE_OP(sub,-=)
PETLIB_MAKE_OP(mul,*=)
PETLIB_MAKE_OP(div,/=)
PETLIB_MAKE_OP(xor,^=)
PETLIB_MAKE_OP(ior,|=)
PETLIB_MAKE_OP(and,&=)
PETLIB_MAKE_OP(lsh,<<=)
PETLIB_MAKE_OP(rsh,>>=)
#undef PETLIB_MAKE_OP

template < class dst_iter_t, typename T >
dst_iter_t fill( dst_iter_t dst_iter, dst_iter_t last, const T src)
{
    while (dst_iter!=last) {
        *dst_iter = src;
        ++dst_iter;
    }
    return dst_iter;
}

#define PETLIB_MAKE_OP(name_,sym_)\
template < class dst_iter_t, typename T > \
dst_iter_t name_##Fill(dst_iter_t dst_iter,dst_iter_t last, const T src) {\
  while (dst_iter!=last) { \
    *dst_iter sym_ src;\
    ++dst_iter;\
  } \
  return dst_iter;\
}

PETLIB_MAKE_OP(add,+=)
PETLIB_MAKE_OP(sub,-=)
PETLIB_MAKE_OP(mul,*=)
PETLIB_MAKE_OP(div,/=)
PETLIB_MAKE_OP(xor,^=)
PETLIB_MAKE_OP(ior,|=)
PETLIB_MAKE_OP(and,&=)
PETLIB_MAKE_OP(lsh,<<=)
PETLIB_MAKE_OP(rsh,>>=)
#undef PETLIB_MAKE_OP


template < typename T > constexpr T abs(T x) noexcept {
    return (x>=T(0))?x:-x;
}

template < typename T > constexpr bool abs_cmp( T x, T y) noexcept
{
    return abs<T>(x)<abs<T>(y);
}

template < class iter_t >
constexpr typename iter_t::value_type amax_element(iter_t iter,const iter_t last) noexcept{
    typedef typename iter_t::value_type value_type; 
    value_type cmax(0);
    while (iter!=last) {
        value_type curr_val = abs<value_type>(*iter);
        if (curr_val>cmax) {
            cmax=curr_val;
        }
        ++iter;
    }
    return cmax;
}

template < class iter_t >
constexpr typename iter_t::value_type amin_element(iter_t iter,const iter_t last) noexcept {
    typedef typename iter_t::value_type value_type;
    value_type cmin(0);
    while (iter!=last) {
        value_type curr_val = abs<value_type>(*iter);
        if (curr_val<cmin) {
            cmin=curr_val;
        }
        ++iter;
    }
    return cmin;
}

template < class iter_t > 
constexpr std::size_t amax_index(iter_t iter,const iter_t last) noexcept {
    typedef typename iter_t::value_type value_type;
    value_type c{value_type(0)};
    std::size_t ix{0};
    std::size_t ic{0};
    while (iter!=last) {
        value_type cval = abs<value_type>(*iter);
        if (cval>c) {
            c=cval;
            ic = ix;
        }
        ++ix; 
        ++iter;
    }
    return ic;
}

template < class iter_t > 
constexpr std::size_t amin_index(iter_t iter,const iter_t last) noexcept {
    typedef typename iter_t::value_type value_type;
    value_type c{value_type(0)};
    std::size_t ix{0};
    std::size_t ic{0};
    while (iter!=last) {
        value_type cval = abs<value_type>(*iter);
        if (cval<c) {
            c=cval;
            ic = ix;
        }
        ++ix; 
        ++iter;
    }
    return ic;
}

template < class iter_t >
constexpr typename iter_t::value_type max_element(iter_t iter,const iter_t last) noexcept{
    typedef typename iter_t::value_type value_type; 
    value_type cmax(*iter);
    ++iter;
    while (iter!=last) {
        value_type curr_val = *iter;
        if (curr_val>cmax) {
            cmax=curr_val;
        }
        ++iter;
    }
    return cmax;
}

template < class iter_t >
constexpr typename iter_t::value_type min_element(iter_t iter,const iter_t last) noexcept {
    typedef typename iter_t::value_type value_type;
    value_type cmin(*iter++);
    while (iter!=last) {
        value_type curr_val = *iter;
        if (curr_val<cmin) {
            cmin=curr_val;
        }
        ++iter;
    }
    return cmin;
}

template < class iter_t > 
constexpr std::size_t max_index(iter_t iter,const iter_t last) noexcept {
    typedef typename iter_t::value_type value_type;
    value_type c{*iter++};
    std::size_t ix{0};
    std::size_t ic{0};
    while (iter!=last) {
        value_type cval = *iter;
        if (cval>c) {
            c=cval;
            ic = ix;
        }
        ++ix; 
        ++iter;
    }
    return ic;
}

template < class iter_t > 
constexpr std::size_t min_index(iter_t iter,const iter_t last) noexcept {
    typedef typename iter_t::value_type value_type;
    value_type c{*iter++};
    std::size_t ix{0};
    std::size_t ic{0};
    while (iter!=last) {
        value_type cval = *iter;
        if (cval<c) {
            c=cval;
            ic = ix;
        }
        ++ix; 
        ++iter;
    }
    return ic;
}

template < class r_iter_t , class l_iter_t, typename T >
constexpr l_iter_t rotate(l_iter_t x,l_iter_t last,r_iter_t y,T angle) noexcept {
  typedef typename l_iter_t::value_type value_type;
  T sn = ::sin(angle);
  T cs = ::cos(angle);
  while (x!=last) {
    value_type tmp = *x;
    *x = cs * (*x) - sn * *y;
    *y = cs * (*y) + sn * tmp;
    ++x;
    ++y;
  }
  return x;
}

template < class l_iter_t , class r_iter_t >
constexpr void swap(l_iter_t l_iter,l_iter_t last,r_iter_t r_iter) noexcept
{
    typedef typename r_iter_t::value_type r_value_t;
    while (l_iter!=last) {
        r_value_t tmp = *r_iter;
        *r_iter = *l_iter;
        *l_iter = tmp;
        ++r_iter;
        ++l_iter;
    }
}

template < class iter_t >
constexpr auto norm2  = [] ( iter_t x, iter_t xlast) noexcept {
    typedef typename iter_t::value_type value_type;
    value_type sum(0);
    while (x!=xlast) {
        sum += *x * *x;
        ++x;
    }
    return sum;
};

template < class l_iter_t, class r_iter_t >
constexpr auto dot  = [] ( l_iter_t x, l_iter_t xlast,
  r_iter_t y) noexcept {
    typedef typename l_iter_t::value_type value_type;
    value_type sum(0);
    while (x!=xlast) {
        sum += *x * *y;
        ++x;
        ++y;
    }
    return sum;
};


template < class l_iter_t, class r_iter_t >
constexpr void elim(l_iter_t x,l_iter_t last,r_iter_t y,r_iter_t y_last) {
   auto dotp = dot<l_iter_t,r_iter_t>(y,y_last,x);
   auto nrm2 = norm2<r_iter_t>(y,y_last);
   auto fact = dotp/nrm2;
   while (y!=y_last) {
       *x = *x - fact * (*y);
       ++x;
       ++y;
   }
} 

template < class iter_t >
constexpr iter_t not_fill(iter_t x,iter_t last) noexcept
{
    while (x!=last) {
        *x = !*x; 
        ++x;
    }
    return x;
}

template < class iter_t >
constexpr iter_t neg_fill(iter_t x,iter_t last) noexcept
{
    while (x!=last) {
        *x = -*x; 
        ++x;
    }
    return x;
}

template < class iter_t >
constexpr iter_t com_fill(iter_t x,iter_t last) noexcept
{
    while (x!=last) {
        *x = ~*x; 
        ++x;
    }
    return x;
}

template < class iter_t >
constexpr iter_t pos_fill(iter_t x,iter_t last) noexcept {
    while (x!=last) {
        *x = +*x; 
        ++x;
    }
    return x;
}

template < class iter_t , class fun >
constexpr iter_t func_fill(iter_t x,iter_t last,iter_t y, fun f) noexcept {
    while (x!=last) {
        *x = fun(*y); 
        ++x;
        ++y;
    }
    return x;
}

template < class iter_t , class fun >
constexpr iter_t func_fill(iter_t x,iter_t last,fun f) noexcept {
    while (x!=last) {
        *x = fun(*x); 
        ++x;
    }
    return x;
}

#define make_op(name_,sym_)\
constexpr auto name_##OpLam = [] (auto x, auto y) noexcept { return x sym_ y;};\

make_op(Add,+)
make_op(Sub,-)
make_op(Mul,*)
make_op(Div,/)
make_op(Xor,^)
make_op(Ior,|)
make_op(And,&)
make_op(Lsh,<<)
make_op(Rsh,>>)
#undef make_op

#define make_op(name_,sym_)\
constexpr auto name_##OpLam = [] (auto x) noexcept { return sym_ x;};\

make_op(Com,~)
make_op(Neg,-)
make_op(Pos,+)
#undef make_op

/*
#define make_op(name_,sym_) constexpr bool name_##OpLam = [] (const auto x, const auto y) noexcept { return (x sym_ y);};

make_op(Eq,==)
make_op(Neq,!=)
make_op(LtEq,<=)
make_op(GtEq,>=)
make_op(Gt,>)
make_op(Lt,<)
make_op(LogAnd,&&)
make_op(LogOr,||)
#undef make_op

constexpr bool NotOpLam = [] (auto x) noexcept { return !x;};
*/

#define make_op(name_,sym_)\
template < typename T > \
struct name_##Op {  \
    typedef T value_type; \
    static value_type eval(T x,T y) noexcept { return (x sym_ y);} \
}; \
 \
template < typename T > \
struct name_##RevOp {  \
    typedef T value_type; \
    static value_type eval(T x,T y) noexcept { return (y sym_ x);} \
}; 

make_op(Add,+)
make_op(Sub,-)
make_op(Mul,*)
make_op(Div,/)
make_op(Xor,^)
make_op(Ior,|)
make_op(And,&)
make_op(Lsh,<<)
make_op(Rsh,>>)
#undef make_op

#define make_op(name_,sym_)\
template < typename T > \
struct name_##Op {  \
    typedef bool value_type; \
    static value_type eval(T x,T y) noexcept { return (x sym_ y);} \
}; \
 \
template < typename T > \
struct name_##RevOp {  \
    typedef bool value_type; \
    static value_type eval(T x,T y) noexcept { return (y sym_ x);} \
}; 

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
template < typename T > struct name_##Op {\
  typedef T value_type;\
  static value_type eval(T x) noexcept { return ( sym_ x);}\
};

make_op(Com,~)
make_op(Neg,-)
make_op(Pos,+)
#undef make_op
  
template < typename T > struct NotOp {
  typedef bool value_type;
  static value_type eval(T x) noexcept { return !x;}
};

template < class iter_t >
void fill_int(iter_t x,iter_t last)
{
  auto s = std::clock();
  std::mt19937 rgen((s+s+1));
  while (x!=last) { *x = rgen(); ++x; }
}

template < class iter_t >
void fill_uint(iter_t x,iter_t last)
{
  auto s = std::clock();
  std::mt19937 rgen(s+s+1);
  while (x!=last) { *x = rgen(); ++x; }
}

template < class iter_t >
void fill_dbl(iter_t x,iter_t last)
{
  auto s = std::clock();
  std::mt19937 rgen(s+s+1);
  std::uniform_real_distribution dist(-1.,1.);
  while (x!=last) { *x = dist(rgen); ++x; }
}

}
#endif

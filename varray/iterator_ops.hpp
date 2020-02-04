#ifndef PETLIB_ITERATOR_OPS_HPP
#define PETLIB_ITERATOR_OPS_HPP
#include <cstdlib>
#include <iterator>
#include <cmath>

namespace petlib {

auto add_op = [] (auto x, auto y) { return (x+y);};
auto sub_op = [] (auto x, auto y) { return (x-y);};
auto mul_op = [] (auto x, auto y) { return (x*y);};
auto div_op = [] (auto x, auto y) { return (x/y);};
auto xor_op = [] (auto x, auto y) { return (x^y);};
auto ior_op = [] (auto x, auto y) { return (x|y);};
auto bi_and_op = [] (auto x, auto y) { return (x&y);};
auto rsh_op = [] (auto x, auto y) { return (x>>y);};
auto lsh_op = [] (auto x, auto y) { return (x<<y);};

auto neg_op = [] ( auto x ) { return -x;};
auto pos_op = [] ( auto x ) { return +x;};
auto com_op = [] ( auto x)  { return ~x;}

bool not_op = [] ( auto x)  { return !x;};
bool eq_op = [] (auto x, auto y) { return (x==y);};
bool neq_op = [] (auto x, auto y) { return (x!=y);};
bool gt_eq_op = [] (auto x, auto y) { return (x>=y);};
bool lt_eq_op = [] (auto x, auto y) { return (x<=y);};
bool gt_op = [] (auto x, auto y) { return (x>y);};
bool lt_op = [] (auto x, auto y) { return (x<y);};

bool is_null = [] ( auto * x ) { return (x==nullptr);};

template < class src_iterator, class dst_iterator >
dst_iterator copy( src_iterator src, const src_iterator last,  dst_iterator dst)
{
    while (src!=last) {
        *dst = *src;
        ++dst;
        ++src;
    }
    return dst;
}

template < class src_iterator >
dst_iterator swap( src_iterator src, const src_iterator last,  src_iterator dst)
{
    while (src!=last) {
        src_iterator tmp(dst);
        *dst = *src;
        *src = *tmp;
        ++dst;
        ++src;
    }
    return dst;
}

template < class src_iterator , typename T >
dst_iterator elim( src_iterator src, const src_iterator last,  src_iterator dst, T x)
{
    while (src!=last) {
        *dst = *src - x * (*dst);
        ++dst;
        ++src;
    }
    return dst;
}

template < class src_iterator , typename T >
auto norm2 -> decltype(typename src_iterator::value_type) = []  
( src_iterator src, const src_iterator last,  src_iterator dst)
{
    typename src_iterator::value_type value_type;
    value_type sum(0);
    while (src!=last) {
        sum+=*dst * *src;
        ++dst;
        ++src;
    }
    return sum;
}

template < class src_iterator, typename T >
std::size_t amax_index( src_iterator src, src_iterator last)
{
    typename src_iterator::value_type value_type;
    value_type sum( *src );
    std::size_t s=0;
    std::size_t curr=0;
    ++src;
    while (src!=last) {
        value_type curr_val = abs<T>(*src); 
        if ( val > sum) {
             s=curr;
             sum = curr_val;
        }
        ++curr;
        ++src;
    }
    return s;
}

template < class src_iterator, typename T >
std::size_t amin_index( src_iterator src, src_iterator last)
{
    typename src_iterator::value_type value_type;
    value_type sum( *src );
    std::size_t s=0;
    std::size_t curr=0;
    ++src;
    while (src!=last) {
        value_type curr_val = abs<T>(*src); 
        if ( val < sum) {
             s=curr;
             sum = curr_val;
        }
        ++curr;
        ++src;
    }
    return s;
}

template < class src_iterator >
auto amax_element->decltype(typename src_iterator::value_type) = [] 
( src_iterator src, src_iterator last)
{
    typename src_iterator::value_type value_type;
    value_type sum( *src );
    ++src;
    while (src!=last) {
        value_type curr_val = abs<T>(*src); 
        if ( curr_val > sum) {
             sum = curr_val;
        }
        ++src;
    }
    return sum
}

template < class src_iterator, typename T >
auto amin_element->decltype(typename src_iterator::value_type) = [] 
( src_iterator src, src_iterator last)
{
    typename src_iterator::value_type value_type;
    value_type sum( *src );
    ++src;
    while (src!=last) {
        value_type curr_val = abs<T>(*src); 
        if ( val < sum) {
             sum = val;
        }
        ++src;
    }
    return sum;
}

template < class src_iterator, class dst_iterator, class unary_op >
dst_iterator apply_un_op( src_iterator src, const src_iterator last,  dst_iterator dst,
   unary_op op)
{
    while (src!=last) {
        *dst = unary_op(*src);
        ++dst;
        ++src;
    }
    return dst;
}

template < class lhs_iterator, class rhs_iterator, class dst_iterator,class binary_op >
dst_iterator apply_bin_op( lhs_iterator lhs, const lhs_iterator lhs,  rhs_iterator rhs,
   dst_iterator dst,
   binary_op op)
{
    while (src!=last) {
        *dst = binary_op(*lhs,*rhs);
        ++dst;
        ++lhs;
        ++rhs;
    }
    return dst;
}

template < class alloc, typename T >
T * get_storage(size_t n) { return alloc.allocate(n);};
template < class alloc, typename T > 
void free_storage(T *p) { return alloc.deallocate(p);};

template < class iterator_t, typename T >
iterator_t fill_elements(iterator_t iter,iterator_t last,T x=T(0)) {
    while (iter!=last) {
        *iter= x;
        ++iter;
    }
    return iter;
} 

template < class iter1_t, class iter2_t >
iter1 rotate(iter1_t iter,iter1_t last, iter2_t iter2, typename iter1_t::value_type angle)
{
    typedef typename iter1_t::value_type value_type;
    value_type c = std::cos(angle);
    value_type s = std::sin(angle);
    while (iter1!=iter2) {
        iter1 tmp(iter2);
        *iter1 = c * *iter1 - s * *iter2;
        *iter2 = c * *iter2 + s * *tmp;
        ++iter1;
        ++iter2; 
    }
    return iter1;
}


template < typename T >
struct slice_iterator {
    typedef T value_type;
    typedef std::size_t size_type;
    typedef std::difference_type difference_type;
    typedef T* pointer;
    typedef T& reference;
    
    slice_iterator(pointer p_base,size_type stride_):p(p_base),str(stride_) {}
        
    constexpr pointer base() noexcept { return p;}
    constexpr const pointer base() const noexcept { return p;}
    constexpr size_type stride() const noexcept { return str;};
    
    constexpr const reference operator*() const noexcept { return *p;}
    constexpr const reference operator->() const noexcept { return *p);}
    constexpr const reference operator[](size_type n) const noexcept {
      return p[n*str];
    };  
    constexpr  reference operator*()  noexcept { return *p;}
    constexpr  reference operator->() noexcept { return *p);}
    constexpr  reference operator[](size_type n)  noexcept {
      return p[n*str];
    };  
    
    constexpr slice_iterator& operator++() const noexcept { p+=str; return *this;}
    constexpr slice_iterator& operator--() const noexcept { p-=str; return *this;}
    constexpr slice_iterator& operator++(int) const noexcept { 
        slice_iterator tmp(*this);
        p+=str;; 
        return tmp;
    }
    constexpr slice_iterator& operator--(int) const noexcept {
        slice_iterator tmp(*this);
        p-=str; 
        return tmp;
    }
};

template < typename T >
constexpr bool operator==(const slice_iterator<T>& x,const slice_iterator<T>& y)
{
    return (x.base() == y.base());
}   
    
template < typename T >
constexpr bool operator!=(const slice_iterator<T>& x,const slice_iterator<T>& y)
{
    return (x.base() != y.base());
}   
    
template < typename T >
constexpr bool operator<=(const slice_iterator<T>& x,const slice_iterator<T>& y)
{
    return (x.base() <= y.base());
}   
    
template < typename T >
constexpr bool operator>=(const slice_iterator<T>& x,const slice_iterator<T>& y)
{
    return (x.base() >= y.base());
}   
    
template < typename T >
constexpr bool operator<(const slice_iterator<T>& x,const slice_iterator<T>& y)
{
    return (x.base() < y.base());
}   
    
template < typename T >
constexpr bool operator>(const slice_iterator<T>& x,const slice_iterator<T>& y)
{
    return (x.base() > y.base());
}   
    
template < typename T > 
constexpr slice_iterator<T> operator+(std::ptrdiff_t n,slice_iterator<T>& x)
{
    return slice_iterator<T>(x.base()+n*x.stride(),x.stride());
}    
    
template < typename T > 
constexpr slice_iterator<T> operator+(slice_iterator<T>& x,std::ptrdiff_t n)
{
    return slice_iterator<T>(x.base()+n*x.stride(),x.stride());
}    
    
template < typename T > 
constexpr slice_iterator<T> operator-(std::ptrdiff_t n,slice_iterator<T>& x)
{
    return slice_iterator<T>(x.base()-n*x.stride(),x.stride());
}    
    
template < typename T > 
constexpr slice_iterator<T> operator-(slice_iterator<T>& x,std::ptrdiff_t n)
{
    return slice_iterator<T>(x.base()-n*x.stride(),x.stride());
}    
    
template < typename T, class iter_t > 
std::ostream& print_index_value(iter_t iter,iter_t last,std::ostream& os)
{       
    std::size_t ix = 0 ;
    while (iter!=last) {
        os << setw(12) << ix << " " << *iter << "\n";
        ++iter;
        ++ix;
    }
    return os;
}    
    
template < class iter_t > 
std::ostream& print_array = [] (iter_t iter,iter_t last,std::ostream& os)
{       
    os << " ( " << *iter;
    while (iter!=last) {
        os << ", ";
        os << *iter;
        ++iter;
    }
    os << " )\n";
    return os;
}    

template < typename T >
class scalar_ref {
    typedef T value_type;
    typedef std::size_t size_type;
    T x;
    scalar_ref(T y):x(y) {}; 
    constexpr value_type operator[](size_type i) const noexcept { return x;}
    constexpr value_type operator()(size_type i,size_t j) const noexcept { return x;}
    constexpr value_t operator() { return x;}
};
    
    
template < class array_t, typename T >
struct array_ref {
    typedef T value_type;
    typedef std::size_t size_type;

    const array_t& a;
    array_ref(const array_t& a0):a(a0) {};
    constexpr value_type operator[](size_type i) const noexcept { return a[i];}
    constexpr size_type size() const noexcept { return a.size();}    
};    
    
template < class op_t , class lhs_t, class rhs_t >
struct array_binary_xpr
{     
    typedef typename op_t::value_type value_type;
    typedef std::size_t size_type;
    op_t op;
    lhs_t lhs;
    rhs_t rhs;
    
    array_binary_xpr(lhs_t& lhs0,rhs_t& rhs0):op(),lhs(lhs0),rhs(rhs0) {}
    constexpr value_t operator[](size_type i) const noexcept { return op.eval(a[i],b[i]);}
    constexpr size_type size() const noexcept { return a.size();}    
};

template < class op_t , class xpr_t >
struct array_unary_xpr
{     
    typedef typename op_t::value_type value_t;
    typedef std::size_t size_type;
    op_t op;
    xpr_t a;

    array_unary_xpr(xpr_t& a0):op(),a(a0) {}
    constexpr value_t operator[](size_type i) const noexcept { return op.eval(a[i]);}
    constexpr size_type size() const noexcept { return a.size();}    
};

template < class xpr_t, typename T >
struct array_xpr {
    typedef typename xpr_t::value_type value_type;
    typedef std::size_t size_type;
    xpr_t a;
    array_xpr(xpr_t& a0):a(a0) {}
    constexpr value_type operator[](size_type i) const noexcept { return a[i];}
    constexpr size_type size() const noexcept { return a.size();}    
};



template < class array_t , typename T >
struct array_base {
    typedef T value_type;
    typedef std::size_t size_type;
    typedef std::difference_type difference_type;
    typedef T* pointer;
    typedef T& reference;
    
    constexpr const array_t * leaf() {
        return static_cast<const array_t*>(this);
    }
    constexpr value_type operator[](size_type i) const noexcept { 
        return leaf()->operator[](i);
    }
    constexpr size_type size() const noexcept { 
        return leaf()->size();
    }
};

#define PETLIB_MAKE_OP_(name_,sym_)\
template < typename T > struct name_##Op {\
    typedef T value_type;       \
    static value_type eval(T x, T y) noexcept { x sym_ y;}\
}; 
PETLIB_MAKE_OP(add,+)
PETLIB_MAKE_OP(sub,-)
PETLIB_MAKE_OP(mul,*)
PETLIB_MAKE_OP(div,/)
PETLIB_MAKE_OP(bit_xor,^)
PETLIB_MAKE_OP(bit_ior,|)
PETLIB_MAKE_OP(bit_and,&)
PETLIB_MAKE_OP(lshift,<<)
PETLIB_MAKE_OP(rshift,>>)
#undef PETLIB_MAKE_OP

#define PETLIB_MAKE_OP_(name_,sym_)\
template < typename T > struct name_##RevOp {\
    typedef T value_type;       \
    static value_type eval(T x, T y) noexcept { y sym_ x;}\
}; 
PETLIB_MAKE_OP(add,+)
PETLIB_MAKE_OP(sub,-)
PETLIB_MAKE_OP(mul,*)
PETLIB_MAKE_OP(div,/)
PETLIB_MAKE_OP(bit_xor,^)
PETLIB_MAKE_OP(bit_ior,|)
PETLIB_MAKE_OP(bit_and,&)
PETLIB_MAKE_OP(lshift,<<)
PETLIB_MAKE_OP(rshift,>>)
#undef PETLIB_MAKE_OP

#define PETLIB_MAKE_OP_(name_,sym_)\
template < typename T > struct name_##Op {\
    typedef bool value_type;       \
    static value_type eval(T x, T y) noexcept { x sym_ y;}\
}; 
PETLIB_MAKE_OP(eq,==)
PETLIB_MAKE_OP(neq,!=)
PETLIB_MAKE_OP(gt,>)
PETLIB_MAKE_OP(lt,<)
PETLIB_MAKE_OP(gte,>=)
PETLIB_MAKE_OP(lte,<=)
PETLIB_MAKE_OP(log_or,||)
PETLIB_MAKE_OP(log_and,&&)
#undef PETLIB_MAKE_OP
 
#define PETLIB_MAKE_OP_(name_,sym_)\
template < typename T > struct name_##Op {\
    typedef T value_type;       \
    static value_type eval(T x) noexcept { sym_ x;}\
}; 
PETLIB_MAKE_OP(com,~)
PETLIB_MAKE_OP(pos,+)
PETLIB_MAKE_OP(neg,-)
#undef PETLIB_MAKE_OP
 
 
#define PETLIB_MAKE_OP_(name_,sym_)\
template < typename T , class a_t, class b_t> \
constexpr array_xpr< \
array_binary_xpr < name_##Op<T>, array_ref<a_t,T> , array_ref<b_t,T> >, T > \
operator sym_ ( const array_base<a_t,T>& a, const array_base<b_t,T>& b) {\
    typedef xpr_t array_binary_xpr < name_##Op<T>, array_ref<a_t,T> , array_ref<b_t,T> >; \
    array_xpr< xpr_t, T >( xpr_t ( array_ref<a_t,T>(a), array_ref<b_t,T>(b) ) );\
}  \
 \
template < typename T , class a_t, class b_t> \
constexpr array_xpr< \
array_binary_xpr < name_##Op<T>, array_ref<a_t,T> , array_xpr<b_t,T> >, T > \
operator sym_ ( const array_base<a_t,T>& a, const array_xpr<b_t,T>& b) {\
    typedef array_binary_xpr < name_##Op<T>, array_ref<a_t,T> , array_xpr<b_t,T> > xpr_t; \
    array_xpr< xpr_t, T >( xpr_t ( array_ref<a_t,T>(a), b ) );\
}  \
 \
template < typename T , class a_t, class b_t> \
constexpr array_xpr< \
array_binary_xpr < name_##Op<T>, array_xpr<a_t,T> , array_ref<b_t,T> >, T > \
operator sym_ ( const array_xpr<a_t,T>& a, const array_base<b_t,T>& b) {\
    typedef xpr_t array_binary_xpr < name_##Op<T>, array_xpr<a_t,T> , array_ref<b_t,T> >; \
    array_xpr< xpr_t, T >( xpr_t ( a, array_ref<b_t,T>(b) ) );\
}  \
 \
template < typename T , class a_t, class b_t> \
constexpr array_xpr< \
array_binary_xpr < name_##Op<T>, array_xpr<a_t,T> , array_xpr<b_t,T> >, T > \
operator sym_ ( const array_xpr<a_t,T>& a, const array_xpr<b_t,T>& b) {\
    typedef array_binary_xpr < name_##Op<T>, array_xpr<a_t,T> , array_xpr<b_t,T> > xpr_t; \
    array_xpr< xpr_t, T >( xpr_t ( a , b ) );\
}   \
 \
template < typename T , class a_t > \
constexpr array_xpr< \
array_binary_xpr < name_##Op<T>, array_ref<a_t,T> , scalar_ref<T> >, T > \
operator sym_ ( const array_base<a_t,T>& a, const T b) {\
    typedef xpr_t array_binary_xpr < name_##Op<T>, array_ref<a_t,T> , scalar_ref<T> >; \   
    array_xpr< xpr_t, T >( xpr_t ( array_ref<a_t,T>(a), scalar_ref<T>(b) ) );\
}  \
 \
template < typename T , class a_t > \
constexpr array_xpr< \
array_binary_xpr < name_##Op<T>, array_ref<a_t,T> , scalar_ref<T> >, T > \
operator sym_ ( const T b, const array_base<a_t,T>& a) {\
    typedef xpr_t array_binary_xpr < name_##RevOp<T>, array_ref<a_t,T> , scalar_ref<T> >; \
    array_xpr< xpr_t, T >( xpr_t ( array_ref<a_t,T>(a), scalar_ref<T>(b) ) );\
}  \
 \
template < typename T , class a_t > \
constexpr array_xpr< \
array_binary_xpr < name_##Op<T>, array_xpr<a_t,T> , scalar_ref<T> >, T > \
operator sym_ ( const array_xpr<a_t,T>& a, const T b) {\
    typedef xpr_t array_binary_xpr < name_##Op<T>, array_xpr<a_t,T> , scalar_ref<T> >; \
    array_xpr< xpr_t, T >( xpr_t ( a, scalar_ref<T>(b) ) );\
}  \
 \
template < typename T , class a_t > \
constexpr array_xpr< \
array_binary_xpr < name_##Op<T>, array_xpr<a_t,T> , scalar_ref<T> >, T > \
operator sym_ ( const T b, const array_xpr<a_t,T>& a) {\
    typedef xpr_t array_binary_xpr < name_##RevOp<T>, array_xpr<a_t,T> , scalar_ref<T> >; \
    array_xpr< xpr_t, T >( xpr_t ( a, scalar_ref<T>(b) ) );\
} \
 \
template < typename T , class a_t, class b_t> \
constexpr array_xpr< \
array_binary_xpr < name_##Op<T>, array_ref<a_t,T> , array_xpr<b_t,T> >, T > \
operator sym_ ( const array_base<a_t,T>& a, const array_xpr<b_t,T>& b) {\
    typedef array_binary_xpr < name_##Op<T>, array_ref<a_t,T> , array_xpr<b_t,T> > xpr_t; \
    array_xpr< xpr_t, T >( xpr_t ( array_ref<a_t,T>(a), b ) );\
}  \
 \
template < typename T , class a_t, class b_t> \
constexpr array_xpr< \
array_binary_xpr < name_##Op<T>, array_xpr<a_t,T> , array_ref<b_t,T> >, T > \
operator sym_ ( const array_xpr<a_t,T>& a, const array_base<b_t,T>& b) {\
    typedef xpr_t array_binary_xpr < name_##Op<T>, array_xpr<a_t,T> , array_ref<b_t,T> >; \
    array_xpr< xpr_t, T >( xpr_t ( a, array_ref<b_t,T>(b) ) );\
}  \
 \
template < typename T , class a_t, class b_t> \
constexpr array_xpr< \
array_binary_xpr < name_##Op<T>, array_xpr<a_t,T> , array_xpr<b_t,T> >, T > \
operator sym_ ( const array_xpr<a_t,T>& a, const array_xpr<b_t,T>& b) {\
    typedef array_binary_xpr < name_##Op<T>, array_xpr<a_t,T> , array_xpr<b_t,T> > xpr_t; \
    array_xpr< xpr_t, T >( xpr_t ( a , b ) );\
}   
 
PETLIB_MAKE_OP(add,+)
PETLIB_MAKE_OP(sub,-)
PETLIB_MAKE_OP(mul,*)
PETLIB_MAKE_OP(div,/)
PETLIB_MAKE_OP(bit_xor,^)
PETLIB_MAKE_OP(bit_ior,|)
PETLIB_MAKE_OP(bit_and,&)
PETLIB_MAKE_OP(lshift,<<)
PETLIB_MAKE_OP(rshift,>>)
PETLIB_MAKE_OP(eq,==)
PETLIB_MAKE_OP(neq,!=)
PETLIB_MAKE_OP(gt,>)
PETLIB_MAKE_OP(lt,<)
PETLIB_MAKE_OP(gte,>=)
PETLIB_MAKE_OP(lte,<=)
PETLIB_MAKE_OP(log_or,||)
PETLIB_MAKE_OP(log_and,&&)
#undef PETLIB_MAKE_OP

#define PETLIB_MAKE_OP_(name_,sym_)\
template < typename T , class a_t > \
array_xpr < array_unary_xpr < name_##Op<T>, array_ref<a_t, T > >, T > \
operator sym_ ( const array_base<a_t,T>& a) { \
  typedef array_unary_xpr < name_##Op<T>, array_ref<a_t, T > > xpr_t;\
  return array_xpr<xpr_t,T>(xpr_t(array_ref<a_t,T>(a)));\
}\
 \
template < typename T , class a_t > \
array_xpr < array_unary_xpr < name_##Op<T>, array_xpr<a_t, T > >, T > \
operator sym_ ( const array_xpr<a_t,T>& a) { \
  typedef array_unary_xpr < name_##Op<T>, array_xpr<a_t, T > > xpr_t;\
  return array_xpr<xpr_t,T>(xpr_t(array_xpr<a_t,T>(a)));\
}

PETLIB_MAKE_OP(com,~)
PETLIB_MAKE_OP(pos,+)
PETLIB_MAKE_OP(neg,-)
#undef PETLIB_MAKE_OP


}
#endif



namespace petlib {

template < typename T > struct type_traits {
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef T * pointer;
    typedef const T * const_pointer;
    typedef T & reference;
    typedef const T & const_reference;
};



template < typename T > struct matrix : public array_base< matrix<T>, T >
{
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef T * pointer;
    typedef const T * const_pointer;
    typedef T & reference;
    typedef const T & const_reference;

    T* data_;
    size_type n1,n2,sz;

    matrix():data_(nullptr),n1(0),n2(0),sz(0) {}
    matrix(const matrix<T>& m):data_(get_storage<T>(m.sz)),n1(m.n1),n2(m.n2),sz(m.sz) { 
        copy(data_,data_+sz,m.data_);
    }
    matrix(matrix<T>&& m):data_(m.data_),n1(m.n1),n2(m.n2),sz(m.sz) {}
    
    template < class a_t >
    matrix(const matrix_xpr<a_t,T>& m):data_(get_storage<T>(a.size()),n1(a.extent1()),n2(a.extent2()),
        sz(a.size()) {
       array_iterator<T> iter(data_);
       for (size_type i = 0; i < n1; ++i) {
           for (size_t j = 0; j < n2; ++j) {
               *iter = a(i,j);
               ++iter; 
           }
       } 
    }
    
    template < class a_t >
    matrix(const matrix_base<a_t,T>& m):data_(get_storage<T>(a.size()),n1(a.extent1()),n2(a.extent2()),
        sz(a.size()) {
       array_iterator<T> iter(data_);
       for (size_type i = 0; i < n1; ++i) {
           for (size_t j = 0; j < n2; ++j) {
               *iter = a(i,j);
               ++iter; 
           }
       } 
    }
    
    matrix(size_type nrow,size_type ncol):data_(get_storage<T>(nrow*ncol)),n1(nrow),
        n2(ncol),sz(nrow*ncol) {
    }
    
    matrix(const pointer p,size_type nrow,size_type ncol):data_(get_storage<T>(nrow*ncol)),n1(nrow),
        n2(ncol),sz(nrow*ncol) {
        copy(data_,data_+sz,p);
    }
    
    matrix(const initializer_list< initializer_list < T > >& mlist) {
    
    
    }
    
    
    ~matrix() {
        free_storage<T>(data_);
        n1=n2=0;
        sz=0;
    }
    
    matrix& operator=(matrix&& m) {
        if (data_) free_storage<T>(data_);
        data_ = m.data_;
        n1 = m.n1;
        n2 = m.n2;
        sz = m.sz;
        m.data_ = nullptr;
        m.n1=m.n2=m.sz=0;
        return *this;
    }
    matrix& operator=(const matrix& m) {
        copy(data_,data_+sz,m.data_);
        return *this;
    }
    
    size_type extent1() const noexcept { return n1;}
    size_type extent2() const noexcept { return n2;}
    size_type size() const noexcept { return sz;}
    size_type stride() const noexcept { return n2;}
    size_type extent(size_type i) const noexcept {
        switch (i) {
        case 0:
            return n1;
        case 1:
            return n2;
        default:
            return sz;        
        }
    } 
    
    constexpr value_type operator()(size_type i, size_type j) const noexcept {
        return data_[i*n2+j];
    }
    constexpr const_pointer operator[](size_type i) const noexcept {
        return data_+i*n2;
    }
    constexpr reference operator()(size_type i, size_type j) noexcept {
        return data_[i*n2+j];
    }
    constexpr pointer operator[](size_type i) noexcept {
        return data_+i*n2;
    }
     
    constexpr iterator begin() noexcept { return data_;}
    constexpr iterator end() noexcept { return data_+sz;}
    constexpr reverse_iterator rbegin() noexcept { return reverse_iterator(end());}
    constexpr reverse_iterator rend() noexcept { return reverse_iterator(begin());}
    constexpr const_iterator begin() const noexcept { return data_;}
    constexpr const_iterator end() const noexcept { return data_+sz;}
    constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end());}
    constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin());}
   
    constexpr array_iterator<T> row_begin(size_type i) noexcept {
        return array_iterator<T>(data_+i*n2);
    }
    constexpr array_iterator<T> row_end(size_type i) noexcept{
        return array_iterator<T>(data_+i*n2+n2);
    }
    constexpr slice_iterator<T> col_begin(size_type i) noexcept {
        return slice_iterator<T>(data_+i,n2);
    }
    constexpr slice_iterator<T> col_end(size_type i) noexcept {
        return slice_iterator<T>(data_+i+n2*n1,n2);
    }
    constexpr slice_iterator<T> diag_begin(size_type i) noexcept {
        return slice_iterator<T>(data_,(n2+1));
    }
    constexpr slice_iterator<T> diag_end(size_type i) noexcept {
        return slice_iterator<T>(data_+(n2+1)*n1,(n2+1));
    }
    constexpr bool empty() const noexcept { return (data_==nullptr);}
    constexpr bool valid_size() const noexcept {
        return (n1>0) && (n2>0) && (n1*n2<=sz);
    }
    constexpr bool is_row_major() const noexcept { return true;}
    constexpr bool is_col_major() const noexcept { return false;}
    constexpr bool is_zero_offset() const noexcept { return true;}
    
    



    constexpr value_type amax_col_element(size_type i) const noexcept {
        return amax_element(col_begin(i),col_end(i));
    }
    constexpr value_type amin_col_element(size_type i) const noexcept {
        return amin_element(col_begin(i),col_end(i));    
    }
    constexpr size_type amax_col_index(size_type i) const noexcept {
        return amax_index(col_begin(i),col_end(i));    
    }
    constexpr size_type amin_col_index(size_type i) const noexcept {
        return amin_index(col_begin(i),col_end(i));    
    }
    constexpr value_type max_col_element(size_type i) const noexcept {
        return max_element(col_begin(i),col_end(i));    
    }
    constexpr value_type min_col_element(size_type i) const noexcept {
        return min_element(col_begin(i),col_end(i));    
    }
    constexpr size_type max_col_index(size_type i) const noexcept {
        return max_element(col_begin(i),col_end(i));    
    }
    constexpr size_type min_col_index(size_type i) const noexcept {
        return min_element(col_begin(i),col_end(i));    
    }

    constexpr value_type amax_row_element(size_type i) const noexcept {
        return amax_element(row_begin(i),row_end(i));
    }
    constexpr value_type amin_row_element(size_type i) const noexcept {
        return amin_element(row_begin(i),row_end(i));    
    }
    constexpr size_type amax_row_index(size_type i) const noexcept {
        return amax_index(row_begin(i),row_end(i));    
    }
    constexpr size_type amin_row_index(size_type i) const noexcept {
        return amin_index(row_begin(i),row_end(i));    
    }
    constexpr value_type max_row_element(size_type i) const noexcept {
        return max_element(row_begin(i),row_end(i));    
    }
    constexpr value_type min_row_element(size_type i) const noexcept {
        return min_element(row_begin(i),row_end(i));    
    }
    constexpr size_type max_row_index(size_type i) const noexcept {
        return max_element(row_begin(i),row_end(i));    
    }
    constexpr size_type min_row_index(size_type i) const noexcept {
        return min_element(row_begin(i),row_end(i));    
    }
    
    constexpr value_type amax_element(size_type i) const noexcept {
        return amax_element(begin(i),end(i));
    }
    constexpr value_type amin_element(size_type i) const noexcept {
        return amin_element(begin(i),end(i));    
    }
    constexpr size_type amax_index(size_type i) const noexcept {
        return amax_index(begin(i),end(i));    
    }
    constexpr size_type amin_index(size_type i) const noexcept {
        return amin_index(begin(i),end(i));    
    }
    constexpr value_type max_element(size_type i) const noexcept {
        return max_element(begin(i),end(i));    
    }
    constexpr value_type min_element(size_type i) const noexcept {
        return min_element(begin(i),end(i));    
    }
    constexpr size_type max_index(size_type i) const noexcept {
        return max_element(begin(i),end(i));    
    }
    constexpr size_type min_index(size_type i) const noexcept {
        return min_element(begin(i),end(i));    
    }
    
    constexpr void swap_rows(size_type i,size_type j) noexcept {
        swap(row_begin(i),row_end(i),row_begin(j));
    }    
    constexpr void swap_cols(size_type i,size_type j) noexcept {
        swap(col_begin(i),col_end(i),col_begin(j));
    }    
    constexpr void rotate_rows(size_type i,size_type j, T angle) noexcept {
        rotate(row_begin(i),row_end(i),row_begin(j),angle);
    }    
    constexpr void rotate_cols(size_type i,size_type j, T angle) noexcept {
        rotate(col_begin(i),col_end(i),col_begin(j),angle);
    }    
    constexpr void elim_rows(size_type i,size_type j) noexcept {
        elim(row_begin(i),row_end(i),row_begin(j));
    }    
    constexpr void elim_cols(size_type i,size_type j) noexcept {
        elim(col_begin(i),col_end(i),col_begin(j));
    }        
    constexpr value_type row_norm(size_type i) const noexcept {
        return norm2(row_begin(i),row_end(i));
    }
    constexpr value_type col_norm(size_type i) const noexcept {
        return norm2(col_begin(i),col_end(i));
    }
    constexpr value_type norm() const noexcept {
        return norm2(begin(),end());
    }
   
    constexpr varray<T> row(size_type i) const noexcept {
        return varray<T>(data_[i],n2);
    }
    constexpr sub_array<T> row(size_type i) noexcept {
        return sub_array<T>(data_[i],n2);
    }
    constexpr varray<T> col(size_type i) const noexcept {
        varray<T> c(n2);
        copy(c.begin(),c.end(),col_begin(i));
        return c;
    }
    constexpr slice_array<T> col(size_type i) noexcept {
        return slice_array<T>(data_+i,n1,n2);
    }
    
    constexpr matrix<T> operator()(const range& r1,const range& r2) const noexcept {
        matrix<T> m(r1.size(),r2.size());
        size_type l_corner = r2.offset() + r1.offset()*n2;
        for (size_type i = 0; i < r1.size(); ++i) {
            copy(m.row_begin(i),m.row_end(i),data_+l_corner+i*n2;
        }        
        return m;
    }
    constexpr sub_matrix<T> operator()(const range& r1,const range& r2) noexcept {
        size_type l_corner = r2.offset() + r1.offset()*n2;
        return sub_matrix<T>(data_+l_corner,r1.size(),r2.size(),n2);
    } 
    constexpr matrix<T> transpose() const noexcept {
        matrix<T> m(n2,n1);
        for (size_t i=0;i<n2;++i) {
           copy(m.row_begin(i),m.row_end(i),this->col_begin(i));
        }
        return m;
    }
    constexpr make_identity() const noexcept {
        sub_array<T> s(data_,sz);
        s=T(0);
        fill(diag_begin(),diag_end(),T(1));
    }
    constexpr random_fill() const noexcept {
        for (size_type i=0;i<n1;++i) {
            rand_fill<T>(row_begin(i),row_end(i));
        }         
    }  
    constexpr matrix<T> apply( value_type fun(value_type) ) const noexcept {
        matrix<T> m(*this);
        func_fill(m.begin(),m.end(),this->begin(),fun);
        return m;
    }
    constexpr matrix<T>& apply( value_type fun(value_type) ) noexcept {
        func_fill(this->begin(),this->end(),this->begin(),fun);
        return *this;
    }
    constexpr matrix<T> apply( value_type fun(const value_type&) ) const noexcept {
        matrix<T> m(*this);
        func_fill(m.begin(),m.end(),this->begin(),fun);
        return m;
    }
    constexpr matrix<T>& apply( value_type fun(const value_type&) ) noexcept {
        func_fill(this->begin(),this->end(),this->begin(),fun);
        return *this;
    }
    
    void resize(size_type new_n1,size_type new_n2) {
        size_type new_sz = new_n1 * new_n2;
        if (new_sz > sz) {
           if (data_) free_storage<T>(data_);
           data_ = get_storage<T>(new_sz);
           n1= new_n1;
           n2= new_n2;
           sz = new_sz;
        }
        n1= new_n1;
        n2= new_n2;
        sz = new_sz;
    }
    
#define petlib_make_op(name_,sym_)\
    constexpr matrix<T>& operator sym_ (const matrix<T>& m) {\
       name_##Eq(begin(),end(),m.begin());\
       return *this;\
    }
    
petlib_make_op(Add,+=)
petlib_make_op(Sub,-=)
petlib_make_op(Mul,*=)
petlib_make_op(Div,/=)
petlib_make_op(Xor,^=)
petlib_make_op(Ior,|=)
petlib_make_op(And,&=)
petlib_make_op(Rsh,>>=)
petlib_make_op(Lsh,<<=)
#undef petlib_make_op

#define petlib_make_op(sym_)\
    template < class m_t > \
    constexpr matrix<T>& operator sym_ (const matrix_xpr<m_t,T>& m) {\
       for (size_type i=0;i<n1;++i) {\
           row_iterator iter(row_begin(i));
           for (size_type j=0;j<n2;++j,++iter) *iter sym_ m(i,j);\
       }\
       return *this;\
    }
    
petlib_make_op(+=)
petlib_make_op(-=)
petlib_make_op(*=)
petlib_make_op(/=)
petlib_make_op(^=)
petlib_make_op(|=)
petlib_make_op(&=)
petlib_make_op(>>=)
petlib_make_op(<<=)
#undef petlib_make_op

#define petlib_make_op(sym_)\
    template < class m_t > \
    constexpr matrix<T>& operator sym_ (const matrix_base<m_t,T>& m) {\
       pointer ptr = data_;\
       for (size_type i=0;i<n1;++i) {\
           for (size_type j=0;j<n2;++j,++ptr) *ptr sym_ m(i,j);\
       }\
       return *this;\
    }
    
petlib_make_op(+=)
petlib_make_op(-=)
petlib_make_op(*=)
petlib_make_op(/=)
petlib_make_op(^=)
petlib_make_op(|=)
petlib_make_op(&=)
petlib_make_op(>>=)
petlib_make_op(<<=)
#undef petlib_make_op
    
#define petlib_make_op(name_,sym_)\
    constexpr matrix<T>& operator sym_ (const sub_matrix<T>& m) {\
       for (size_type i=0;i<n1;++i) {\
           name_##Eq(this->row_begin(i),this->row_end(i),m.row_begin(i));\
       }\
       return *this;\
    }

petlib_make_op(Add,+=)
petlib_make_op(Sub,-=)
petlib_make_op(Mul,*=)
petlib_make_op(Div,/=)
petlib_make_op(Xor,^=)
petlib_make_op(Ior,|=)
petlib_make_op(And,&=)
petlib_make_op(Rsh,>>=)
petlib_make_op(Lsh,<<=)
#undef petlib_make_op

#define petlib_make_op(name_,sym_)\
    constexpr matrix<T>& operator sym_ (const T x) {\
       for (size_type i=0;i<n1;++i) {\
           name_##Eq(row_begin(i),row_end(i),x);\
       }\
       return *this;\
    }

petlib_make_op(Add,+=)
petlib_make_op(Sub,-=)
petlib_make_op(Mul,*=)
petlib_make_op(Div,/=)
petlib_make_op(Xor,^=)
petlib_make_op(Ior,|=)
petlib_make_op(And,&=)
petlib_make_op(Rsh,>>=)
petlib_make_op(Lsh,<<=)
#undef petlib_make_op

    std::ostream& serialize(std::ostream& os);
};

template < typename T >
std::ostream& matrix<T>::serialize(std::ostream& os) {
       os.write(reinterpret_cast<char*>(&n1),sizeof(size_type));
       os.write(reinterpret_cast<char*>(&n2),sizeof(size_type));
       os.write(reinterpret_cast<char*>(&sz),sizeof(size_type));
       size_type vsize = sizeof(value_type);
       os.write(reinterpret_cast<char*>(&vsize),sizeof(size_type));
       size_type is_float = 0;
       os.write(reinterpret_cast<char*>(&is_float),sizeof(size_type));
       os.write(reinterpret_cast<char*>(this->data_),sizeof(value_type)*sz);
       return os;
}

template <>
std::ostream& matrix<float>::serialize(std::ostream& os) {
       using matrix<float>::value_type;
       using matrix<float>::size_type;
       os.write(reinterpret_cast<char*>(&n1),sizeof(size_type));
       os.write(reinterpret_cast<char*>(&n2),sizeof(size_type));
       os.write(reinterpret_cast<char*>(&sz),sizeof(size_type));
       size_type vsize = sizeof(value_type);
       os.write(reinterpret_cast<char*>(&vsize),sizeof(size_type));
       size_type is_float = 1;
       os.write(reinterpret_cast<char*>(&is_float),sizeof(size_type));
       os.write(reinterpret_cast<char*>(this->data_),sizeof(value_type)*sz);
       return os;
}

template <>
std::ostream& matrix<double>::serialize(std::ostream& os) {
       using matrix<double>::value_type;
       using matrix<double>::size_type;
       os.write(reinterpret_cast<char*>(&n1),sizeof(size_type));
       os.write(reinterpret_cast<char*>(&n2),sizeof(size_type));
       os.write(reinterpret_cast<char*>(&sz),sizeof(size_type));
       size_type vsize = sizeof(value_type);
       os.write(reinterpret_cast<char*>(&vsize),sizeof(size_type));
       size_type is_double = 1;
       os.write(reinterpret_cast<char*>(&is_double),sizeof(size_type));
       os.write(reinterpret_cast<char*>(this->data_),sizeof(value_type)*sz);
       return os;
}

template <>
std::ostream& matrix<long double>::serialize(std::ostream& os) {
       using matrix<long double>::value_type;
       using matrix<long double>::size_type;
       os.write(reinterpret_cast<char*>(&n1),sizeof(size_type));
       os.write(reinterpret_cast<char*>(&n2),sizeof(size_type));
       os.write(reinterpret_cast<char*>(&sz),sizeof(size_type));
       size_type vsize = sizeof(value_type);
       os.write(reinterpret_cast<char*>(&vsize),sizeof(size_type));
       size_type is_long double = 1;
       os.write(reinterpret_cast<char*>(&is_long double),sizeof(size_type));
       os.write(reinterpret_cast<char*>(this->data_),sizeof(value_type)*sz);
       return os;
}




}



template < typename T >
T * get_storage(std::size_t n) {
    return static_cast<T*>(std::aligned_alloc(n*sizeof(T)));
}

template < typename T >
void free_storage(T* ptr) {
    std::aligned_free(ptr);
}


template < typename T >
struct varray: public petlib::array_base< varray<T>, T> {
   
    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef random_access_iterator_tag iterator_category;
    typedef array_iterator<T> iterator;
    typedef const array_iterator<T> const_iterator;
    typedef std::reverse_iterator<T> rev_iterator;
    typedef const std::reverse_iterator<T> const_rev_iterator;
        
    T * data_;
    size_type sz;
   
    varray():data_(nullptr),sz(0) {}
   
    explicit varray(size_type n):data_(get_storage<T>(n)),sz(n) {}
   
    template < typename v_t >
    varray(const array_base< v_t, T>& v):data_(get_storage<T>(v.size())),sz(v.size()) {
        for (size_type i = 0; i < sz; ++i) data_[i] = v[i];
    }
    
    varray(const T x,size_type n):data_(get_storage<T>(n)),sz(n) {
        petlib::array_fill(data_,data_+sz,x);
    }
    
    varray(T *p, size_type n):data_(get_storage<T>(n)),sz(n) {
        memcpy(p,data_,sizeof(T)*n);
    }
    
    varray(const varray<T>& v):data_(get_storage<T>(v.sz)),sz(v.sz) {
        memcpy(v.data_,data_,sizeof(T)*sz);
    }
    
    varray(varray<T>&& v):data_(v.data_),sz(v.sz) {
        v.data_ = nullptr;
        v.sz = 0;
    }
    
    ~varray() { free_storage<T>(data_); data_=nullptr; sz=0; }
    
    constexpr varray& operator=(const varray<T>& v) {
        memcpy(v.data_,data_,sizeof(T)*sz);
        return *this;
    };
    
    constexpr varray& operator=(varray&& v) {
        if (data_) {
            free_storage<T>(data_);
        }
        data_=v.data_;
        sz=v.sz;
        v.data_ = nullptr;
        v.sz = 0;
        return *this;
    }
    

#define PETLIB_MAKE_OPS_(sym_)\
    constexpr varray& operator sym_ (const varray<T>& v) noexcept {\
        for (size_type i = 0; i < sz ; ++i) data_[i] sym_ v.data[i];
        return *this;    
    }
PETLIB_MAKE_OPS(+=)    
PETLIB_MAKE_OPS(-=)
PETLIB_MAKE_OPS(*=)    
PETLIB_MAKE_OPS(/=)
PETLIB_MAKE_OPS(^=)    
PETLIB_MAKE_OPS(|=)
PETLIB_MAKE_OPS(&=)    
PETLIB_MAKE_OPS(>>=)
PETLIB_MAKE_OPS(<<=)    
#undef PETLIB_MAKE_OPS

#define PETLIB_MAKE_OPS_(sym_)\
    template < class v_t > \
    constexpr varray& operator sym_ (const slice_array<T>& v) noexcept {\
        slice_iterator<T> iter(v.begin()); \
        for (size_type i = 0; i < sz ; ++i, ++iter) data_[i] sym_ *iter;\
        return *this; \
    };
PETLIB_MAKE_OPS(=)
PETLIB_MAKE_OPS(+=)    
PETLIB_MAKE_OPS(-=)
PETLIB_MAKE_OPS(*=)    
PETLIB_MAKE_OPS(/=)
PETLIB_MAKE_OPS(^=)    
PETLIB_MAKE_OPS(|=)
PETLIB_MAKE_OPS(&=)    
PETLIB_MAKE_OPS(>>=)
PETLIB_MAKE_OPS(<<=)    
#undef PETLIB_MAKE_OPS

#define PETLIB_MAKE_OPS_(sym_)\
    template < class v_t > \
    constexpr varray& operator sym_ (const array_base<v_t,T>& v) noexcept {\
        for (size_type i = 0; i < sz ; ++i, ++iter) data_[i] sym_ v[i];\
        return *this; \
    };
PETLIB_MAKE_OPS(=)
PETLIB_MAKE_OPS(+=)    
PETLIB_MAKE_OPS(-=)
PETLIB_MAKE_OPS(*=)    
PETLIB_MAKE_OPS(/=)
PETLIB_MAKE_OPS(^=)    
PETLIB_MAKE_OPS(|=)
PETLIB_MAKE_OPS(&=)    
PETLIB_MAKE_OPS(>>=)
PETLIB_MAKE_OPS(<<=)    
#undef PETLIB_MAKE_OPS

#define PETLIB_MAKE_OPS_(sym_)\
    template < class v_t > \
    constexpr varray& operator sym_ (const array_xpr<v_t,T>& v) noexcept {\
        for (size_type i = 0; i < sz ; ++i) data_[i] sym_ v[i];
        return *this;    
    }
PETLIB_MAKE_OPS(=)    
PETLIB_MAKE_OPS(+=)    
PETLIB_MAKE_OPS(-=)
PETLIB_MAKE_OPS(*=)    
PETLIB_MAKE_OPS(/=)
PETLIB_MAKE_OPS(^=)    
PETLIB_MAKE_OPS(|=)
PETLIB_MAKE_OPS(&=)    
PETLIB_MAKE_OPS(>>=)
PETLIB_MAKE_OPS(<<=)    
#undef PETLIB_MAKE_OPS
   
    constexpr size_type size() const noexcept { return n;}
    
    constexpr reference operator[](size_type i) noexcept { return data_[i];}
    constexpr value_type operator[](size_type i) const noexcept { return data_[i];}
      
    constexpr pointer data() noexcept { return data_;}
    constexpr const pointer data() const noexcept { return data_;}
    
    constexpr bool empty() const noexcept { return (n!=0);}
    
    constexpr pointer begin() noexcept { return data_;}
    constexpr const pointer begin() const noexcept { return data_;}
    constexpr pointer end() noexcept { return data_+n;}
    constexpr const pointer end() const noexcept { return data_+n;}
    
    constexpr std::reverse_iterator<T> rbegin() noexcept { return reverse_iterator<T>(data_+n);}
    constexpr const std::reverse_iterator<T> rbegin() const noexcept { return reverse_iterator<T>(data_+n);}
    constexpr std::reverse_iterator<T> rend() noexcept { return std::reverse_iterator<T>(end());}
    constexpr const std::reverse_iterator<T> rend() const noexcept { 
        return std::reverse_iterator<T>(begin());
    }
    
    constexpr value_type amax() const noexcept {
        return std::max_element(begin(),end(),abs_cmp<T>);
    }
    constexpr value_type amin() const noexcept {
        return std::min_element(begin(),end(),abs_cmp<T>);
    }    
    constexpr size_type amax_index() const noexcept {
        return petlib::max_element_index(begin(),end(),abs_cmp<T>);
    }
    constexpr size_type amin_index() const noexcept {
        return petlib::min_element_index(begin(),end(),abs_cmp<T>);
    }    
    constexpr value_type max() const noexcept {
        return std::max_element(begin(),end());
    }
    constexpr value_type min() const noexcept {
        return std::min_element(begin(),end());
    }    
    constexpr size_type max_index() const noexcept {
        return petlib::max_element_index(begin(),end());
    }
    constexpr size_type min_index() const noexcept {
        return petlib::min_element_index(begin(),end());
    }    

    constexpr varray<T> apply( value_t fun( value_t ) ) const noexcept{
        varray<T> v(sz);
        for (size_type i = 0; i < sz; ++i) v.data_[i] = fun( data_[i] );
        return v;
    }
    constexpr varray<T> apply( value_t fun( const value_t& ) ) const noexcept{
        varray<T> v(sz);
        for (size_type i = 0; i < sz; ++i) v.data_[i] = fun( data_[i] );
        return v;
    }
    
    constexpr varray<T> shift(size_type n) const noexcept {
        varray<T> v(0,sz-n);
        const size_type sz_end = sz-n;
        for (size_type i=0;i<sz_end;++i) {
            v[i] = data_[i+n];    
        }
        return v;
    }

    constexpr varray<T> cshift(size_type n) const noexcept {
        varray<T> v(sz);
        const size_type sz_end = sz-n;
        for (size_type i=0;i<sz_end;++i) {
            v[i] = data_[i+n];    
        }
        for (size_type i=sz_end;i<n;++i) {
            v[i] = data_[sz_end+i];
        }
        return v;
    }
    
    constexpr void swap(varray<T>& v) noexcept {
        for (size_type i=0;i<sz;++i) {
            value_type tmp = data_[i];
            data_[i] = v.data_[i];
            v.data_[i] = tmp;
        }
    }

    constexpr swap_elements(size_type i, size_t j) noexcept {
        value_type tmp{data[i]};
        data_[i]= data_[j];
        data_[j]= tmp;
    }

    constexpr rotate_elements(size_type i, size_t j,T x) noexcept {
        value_type cs = std::cos(x);
        value_type sn = std::sin(x);
        value_type tmp{data[i]};
        data_[i]= cs*data_[i]-sn*data_[j];
        data_[j]= cs*data_[j]+sn*tmp;
    }

    constexpr void swap(varray<T>& v) noexcept {
        for (size_type i=0;i<sz;++i) {
            value_type tmp = data_[i];
            data_[i] = v.data_[i];
            v.data_[i] = tmp;
        }
    }

    constexpr void rotate(varray<T>& v,const T x) noexcept {
        value_type cs = std::cos(x);
        value_type sn = std::sin(x);
        for (size_type i=0;i<sz;++i) {
            value_type tmp = data_[i];
            data_[i] = cs*data_[i] - sn * v.data_[i];
            v.data_[i] = cs*v.data_[i] + sn*tmp;
        }
    }
    
    value_type norm2(const varray<T>& v) const noexcept {
        return petlib::norm2(begin(),end(),v.begin());
    }

    value_type norm2(const slice_array<T>& v) const noexcept {
        return petlib::norm2(begin(),end(),v.begin());
    }
    
    value_type norm2(const sub_array<T>& b) const noexcept {
        return petlib::norm2(begin(),end(),v.begin());        
    }
    
    varray<T> operator()(size_type offset,size_type len) const noexcept {
        return varray<T>(data_+offset,len);
    }  
    sub_array<T> operator()(size_type offset,size_type len) noexcept {
        return sub_array<T>(data_+offset,len);
    }
    varray<T> operator()( const slice& sl) const noexcept {
        varray<T> v(sl.size());
        pointer p = data_ + sl.offset();
        size_type str = sl.stride();
        for (size_t i=0;i<sl.size();++i,p+=str) {
            v[i] = *p;
        }
        return v;
    }  
    slice_array<T> operator()( const slice& sl) noexcept {
        return slice_array<T>(data_+sl.offset(),sl.size(),sl.stride());
    }
    varray<T> operator()( const varray<bool>& mask) const noexcept {
        size_type mask_size = 0;
        for (size_t i=0;i<sz;++i) {
            if (mask[i]) ++mask_size;
        }
        varray<T> v(mask_size);
        size_type curr(0);
        for (size_t i=0;i<sz;++i) {
            if (mask[i]) {
                v[curr] = data_[i];
                ++curr;
            }
        }
        return v;
    }  
    varray<T> operator()( const varray<size_type>& indx) const noexcept {
        varray<T> v(sz);
        for (size_t i=0;i<sz;++i) {
            v[i] = data_[indx[i]];
        }
        return v;
    }  
    
    void resize(size_type new_sz) {
        if (new_sz <= sz) {
            sz = new_sz;
            return;
        }
        if (data_) free_storage<T>(data_);
        data_= get_storage<T>(new_sz);
        sz = new_sz;
    }
    
    void grow(size_type new_sz) {
        if (new_sz <= sz) {
            sz = new_sz;
            return;
        }
        pointer tmp = get_storage<T>(new_sz);
        memcpy(data_,tmp,sizeof(T)*sz);
        if (data_) free_storage<T>(data_);
        data_= tmp;
        sz = new_sz;    
    }
};

template < typename T >
struct sub_array: public petlib::array_base< sub_array<T>, T> {
   
    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef random_access_iterator_tag iterator_category;
    typedef array_iterator<T> iterator;
    typedef const array_iterator<T> const_iterator;
    typedef std::reverse_iterator<T> rev_iterator;
    typedef const std::reverse_iterator<T> const_rev_iterator;
        
    T * data_;
    size_type sz;
   
    sub_array()=delete;

    explicit sub_array(pointer p,size_type n):data_(p),sz(n) {}

    sub_array(const sub_array<T>& v):data_(v.data_),sz(v.sz) {
    }
    
    sub_array(sub_array<T>&& v):data_(v.data_),sz(v.sz) {
        v.data_ = nullptr;
        v.sz = 0;
    }
    
    ~sub_array() { data_=nullptr; sz=0; }
    
    constexpr sub_array& operator=(const sub_array<T>& v) {
        memcpy(v.data_,data_,sizeof(T)*sz);
        return *this;
    };
    
    constexpr sub_array& operator=(sub_array&& v) {
        data_=v.data_;
        sz=v.sz;
        v.data_ = nullptr;
        v.sz = 0;
        return *this;
    }
    

#define PETLIB_MAKE_OPS_(sym_)\
    constexpr sub_array& operator sym_ (const sub_array<T>& v) noexcept {\
        for (size_type i = 0; i < sz ; ++i) data_[i] sym_ v.data[i];
        return *this;    
    }
PETLIB_MAKE_OPS(+=)    
PETLIB_MAKE_OPS(-=)
PETLIB_MAKE_OPS(*=)    
PETLIB_MAKE_OPS(/=)
PETLIB_MAKE_OPS(^=)    
PETLIB_MAKE_OPS(|=)
PETLIB_MAKE_OPS(&=)    
PETLIB_MAKE_OPS(>>=)
PETLIB_MAKE_OPS(<<=)    
#undef PETLIB_MAKE_OPS

#define PETLIB_MAKE_OPS_(sym_)\
    constexpr sub_array& operator sym_ (const varray<T>& v) noexcept {\
        for (size_type i = 0; i < sz ; ++i) data_[i] sym_ v[i];
        return *this;    
    }
PETLIB_MAKE_OPS(=)
PETLIB_MAKE_OPS(+=)    
PETLIB_MAKE_OPS(-=)
PETLIB_MAKE_OPS(*=)    
PETLIB_MAKE_OPS(/=)
PETLIB_MAKE_OPS(^=)    
PETLIB_MAKE_OPS(|=)
PETLIB_MAKE_OPS(&=)    
PETLIB_MAKE_OPS(>>=)
PETLIB_MAKE_OPS(<<=)    
#undef PETLIB_MAKE_OPS

#define PETLIB_MAKE_OPS_(sym_)\
    template < class v_t > \
    constexpr sub_array& operator sym_ (const slice_array<T>& v) noexcept {\
        slice_iterator<T> iter(v.begin()); \
        for (size_type i = 0; i < sz ; ++i, ++iter) data_[i] sym_ *iter;\
        return *this; \
    };
PETLIB_MAKE_OPS(=)
PETLIB_MAKE_OPS(+=)    
PETLIB_MAKE_OPS(-=)
PETLIB_MAKE_OPS(*=)    
PETLIB_MAKE_OPS(/=)
PETLIB_MAKE_OPS(^=)    
PETLIB_MAKE_OPS(|=)
PETLIB_MAKE_OPS(&=)    
PETLIB_MAKE_OPS(>>=)
PETLIB_MAKE_OPS(<<=)    
#undef PETLIB_MAKE_OPS

#define PETLIB_MAKE_OPS_(sym_)\
    template < class v_t > \
    constexpr sub_array& operator sym_ (const array_base<v_t,T>& v) noexcept {\
        for (size_type i = 0; i < sz ; ++i) data_[i] sym_ v[i];\
        return *this; \
    };
PETLIB_MAKE_OPS(=)
PETLIB_MAKE_OPS(+=)    
PETLIB_MAKE_OPS(-=)
PETLIB_MAKE_OPS(*=)    
PETLIB_MAKE_OPS(/=)
PETLIB_MAKE_OPS(^=)    
PETLIB_MAKE_OPS(|=)
PETLIB_MAKE_OPS(&=)    
PETLIB_MAKE_OPS(>>=)
PETLIB_MAKE_OPS(<<=)    
#undef PETLIB_MAKE_OPS

#define PETLIB_MAKE_OPS_(sym_)\
    template < class v_t > \
    constexpr sub_array& operator sym_ (const array_xpr<v_t,T>& v) noexcept {\
        for (size_type i = 0; i < sz ; ++i) data_[i] sym_ v[i];
        return *this;    
    }
PETLIB_MAKE_OPS(=)    
PETLIB_MAKE_OPS(+=)    
PETLIB_MAKE_OPS(-=)
PETLIB_MAKE_OPS(*=)    
PETLIB_MAKE_OPS(/=)
PETLIB_MAKE_OPS(^=)    
PETLIB_MAKE_OPS(|=)
PETLIB_MAKE_OPS(&=)    
PETLIB_MAKE_OPS(>>=)
PETLIB_MAKE_OPS(<<=)    
#undef PETLIB_MAKE_OPS
   
    constexpr size_type size() const noexcept { return sz;}
    
    constexpr reference operator[](size_type i) noexcept { return data_[i];}
    constexpr value_type operator[](size_type i) const noexcept { return data_[i];}
      
    constexpr pointer data() noexcept { return data_;}
    constexpr const pointer data() const noexcept { return data_;}
    
    constexpr bool empty() const noexcept { return (n!=0);}
    
    constexpr pointer begin() noexcept { return data_;}
    constexpr const pointer begin() const noexcept { return data_;}
    constexpr pointer end() noexcept { return data_+n;}
    constexpr const pointer end() const noexcept { return data_+n;}
    
    constexpr std::reverse_iterator<T> rbegin() noexcept { return reverse_iterator<T>(data_+n);}
    constexpr const std::reverse_iterator<T> rbegin() const noexcept { return reverse_iterator<T>(data_+n);}
    constexpr std::reverse_iterator<T> rend() noexcept { return std::reverse_iterator<T>(end());}
    constexpr const std::reverse_iterator<T> rend() const noexcept { 
        return std::reverse_iterator<T>(begin());
    }
    
    constexpr value_type amax() const noexcept {
        return std::max_element(begin(),end(),abs_cmp<T>);
    }
    constexpr value_type amin() const noexcept {
        return std::min_element(begin(),end(),abs_cmp<T>);
    }    
    constexpr size_type amax_index() const noexcept {
        return petlib::max_element_index(begin(),end(),abs_cmp<T>);
    }
    constexpr size_type amin_index() const noexcept {
        return petlib::min_element_index(begin(),end(),abs_cmp<T>);
    }    
    constexpr value_type max() const noexcept {
        return std::max_element(begin(),end());
    }
    constexpr value_type min() const noexcept {
        return std::min_element(begin(),end());
    }    
    constexpr size_type max_index() const noexcept {
        return petlib::max_element_index(begin(),end());
    }
    constexpr size_type min_index() const noexcept {
        return petlib::min_element_index(begin(),end());
    }    

    constexpr sub_array<T> apply( value_t fun( value_t ) ) const noexcept{
        varray<T> v(sz);
        for (size_type i = 0; i < sz; ++i) v.data_[i] = fun( data_[i] );
        return v;
    }
    constexpr varray<T> apply( value_t fun( const value_t& ) ) const noexcept{
        varray<T> v(sz);
        for (size_type i = 0; i < sz; ++i) v.data_[i] = fun( data_[i] );
        return v;
    }
    
    constexpr varray<T> shift(size_type n) const noexcept {
        varray<T> v(0,sz-n);
        const size_type sz_end = sz-n;
        for (size_type i=0;i<sz_end;++i) {
            v[i] = data_[i+n];    
        }
        return v;
    }

    constexpr varray<T> cshift(size_type n) const noexcept {
        varray<T> v(sz);
        const size_type sz_end = sz-n;
        for (size_type i=0;i<sz_end;++i) {
            v[i] = data_[i+n];    
        }
        for (size_type i=sz_end;i<n;++i) {
            v[i] = data_[sz_end+i];
        }
        return v;
    }
    
    constexpr void swap(sub_array<T>& v) noexcept {
        for (size_type i=0;i<sz;++i) {
            value_type tmp = data_[i];
            data_[i] = v.data_[i];
            v.data_[i] = tmp;
        }
    }

    constexpr swap_elements(size_type i, size_t j) noexcept {
        value_type tmp{data[i]};
        data_[i]= data_[j];
        data_[j]= tmp;
    }

    constexpr rotate_elements(size_type i, size_t j,T x) noexcept {
        value_type cs = std::cos(x);
        value_type sn = std::sin(x);
        value_type tmp{data[i]};
        data_[i]= cs*data_[i]-sn*data_[j];
        data_[j]= cs*data_[j]+sn*tmp;
    }

    constexpr void swap(varray<T>& v) noexcept {
        for (size_type i=0;i<sz;++i) {
            value_type tmp = data_[i];
            data_[i] = v.data_[i];
            v.data_[i] = tmp;
        }
    }

    constexpr void rotate(varray<T>& v,const T x) noexcept {
        value_type cs = std::cos(x);
        value_type sn = std::sin(x);
        for (size_type i=0;i<sz;++i) {
            value_type tmp = data_[i];
            data_[i] = cs*data_[i] - sn * v.data_[i];
            v.data_[i] = cs*v.data_[i] + sn*tmp;
        }
    }
    
    value_type norm2(const varray<T>& v) const noexcept {
        return petlib::norm2(begin(),end(),v.begin());
    }

    value_type norm2(const slice_array<T>& v) const noexcept {
        return petlib::norm2(begin(),end(),v.begin());
    }
    
    value_type norm2(const sub_array<T>& b) const noexcept {
        return petlib::norm2(begin(),end(),v.begin());        
    }
    
    sub_array<T> operator()(size_type offset,size_type len) const noexcept {
        return sub_array<T>(data_+offset,len);
    }  
    sub_array<T> operator()(size_type offset,size_type len) noexcept {
        return sub_array<T>(data_+offset,len);
    }
    varray<T> operator()( const slice& sl) const noexcept {
        varray<T> v(sl.size());
        pointer p = data_ + sl.offset();
        size_type str = sl.stride();
        for (size_t i=0;i<sl.size();++i,p+=str) {
            v[i] = *p;
        }
        return v;
    }  
    slice_array<T> operator()( const slice& sl) noexcept {
        return slice_array<T>(data_+sl.offset(),sl.size(),sl.stride());
    }
    varray<T> operator()( const varray<bool>& mask) const noexcept {
        size_type mask_size = 0;
        for (size_t i=0;i<sz;++i) {
            if (mask[i]) ++mask_size;
        }
        varray<T> v(mask_size);
        size_type curr(0);
        for (size_t i=0;i<sz;++i) {
            if (mask[i]) {
                v[curr] = data_[i];
                ++curr;
            }
        }
        return v;
    }  
    varray<T> operator()( const varray<size_type>& indx) const noexcept {
        varray<T> v(sz);
        for (size_t i=0;i<sz;++i) {
            v[i] = data_[indx[i]];
        }
        return v;
    }  
};

template < typename T >
struct slice_array: public petlib::array_base< slice_array<T>, T> {
   
    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef random_access_iterator_tag iterator_category;
    typedef slice_iterator<T> iterator;
    typedef const slice_iterator<T> const_iterator;
        
    T * data_;
    size_type sz;
    size_type str;
       
    slice_array()=delete;

    explicit slice_array(pointer p,size_type n,size_type stride_):data_(p),sz(n),str(stride_) {}

    slice_array(const slice_array<T>& v):data_(v.data_),sz(v.sz),str(v,str) {
    }
    
    slice_array(slice_array<T>&& v):data_(v.data_),sz(v.sz),str(v.str) {
        v.data_ = nullptr;
        v.sz = 0;
    }
    
    ~slice_array() { data_=nullptr; sz=0; str=0; }
    
    constexpr slice_array& operator=(const slice_array<T>& v) {
        petlib::copy(begin(),end(),v.begin());
        return *this;
    };
    
    constexpr slice_array& operator=(slice_array&& v) {
        data_=v.data_;
        sz=v.sz;
        str = v.str;
        v.data_ = nullptr;
        v.sz = 0;
        v.str = 0;
        return *this;
    }
    

#define PETLIB_MAKE_OPS_(sym_,name_)\
    constexpr slice_array& operator sym_ (const slice_array<T>& v) noexcept {\
        apply_op(begin(),end(),v.begin(), name_##Op<T> );
        return *this;    
    }
PETLIB_MAKE_OPS(+=,add_eq)    
PETLIB_MAKE_OPS(-=,sub_eq)
PETLIB_MAKE_OPS(*=,mul_eq)    
PETLIB_MAKE_OPS(/=,div_eq)
PETLIB_MAKE_OPS(^=,xor_eq)    
PETLIB_MAKE_OPS(|=,ior_eq)
PETLIB_MAKE_OPS(&=,and_eq)    
PETLIB_MAKE_OPS(>>=,rsh_eq)
PETLIB_MAKE_OPS(<<=,lsh_eq)    
#undef PETLIB_MAKE_OPS

#define PETLIB_MAKE_OPS_(sym_,name_)\
    constexpr slice_array& operator sym_ (const varray<T>& v) noexcept {\
        apply_op(begin(),end(),v.begin(), name_##Op<T> );
        return *this;    
    }
PETLIB_MAKE_OPS(=,eq)
PETLIB_MAKE_OPS(+=,add_eq)    
PETLIB_MAKE_OPS(-=,sub_eq)
PETLIB_MAKE_OPS(*=,mul_eq)    
PETLIB_MAKE_OPS(/=,div_eq)
PETLIB_MAKE_OPS(^=,xor_eq)    
PETLIB_MAKE_OPS(|=,ior_eq)
PETLIB_MAKE_OPS(&=,and_eq)    
PETLIB_MAKE_OPS(>>=,rsh_eq)
PETLIB_MAKE_OPS(<<=,lsh_eq)    
#undef PETLIB_MAKE_OPS

#define PETLIB_MAKE_OPS_(sym_,name_)\
    constexpr slice_array& operator sym_ (const sub_array<T>& v) noexcept {\
        apply_op(begin(),end(),v.begin(), name_##Op<T> );
        return *this;    
    }
PETLIB_MAKE_OPS(=,eq)
PETLIB_MAKE_OPS(+=,add_eq)    
PETLIB_MAKE_OPS(-=,sub_eq)
PETLIB_MAKE_OPS(*=,mul_eq)    
PETLIB_MAKE_OPS(/=,div_eq)
PETLIB_MAKE_OPS(^=,xor_eq)    
PETLIB_MAKE_OPS(|=,ior_eq)
PETLIB_MAKE_OPS(&=,and_eq)    
PETLIB_MAKE_OPS(>>=,rsh_eq)
PETLIB_MAKE_OPS(<<=,lsh_eq)    
#undef PETLIB_MAKE_OPS


#define PETLIB_MAKE_OPS_(sym_)\
    template < class v_t > \
    constexpr slice_array& operator sym_ (const array_base<v_t,T>& v) noexcept {\
        slice_iterator iter = data_; \
        for (size_type i = 0; i < sz ; ++i, iter+=str) *iter sym_ v[i];\
        return *this; \
    };
PETLIB_MAKE_OPS(=)
PETLIB_MAKE_OPS(+=)    
PETLIB_MAKE_OPS(-=)
PETLIB_MAKE_OPS(*=)    
PETLIB_MAKE_OPS(/=)
PETLIB_MAKE_OPS(^=)    
PETLIB_MAKE_OPS(|=)
PETLIB_MAKE_OPS(&=)    
PETLIB_MAKE_OPS(>>=)
PETLIB_MAKE_OPS(<<=)    
#undef PETLIB_MAKE_OPS

#define PETLIB_MAKE_OPS_(sym_)\
    template < class v_t > \
    constexpr slice_array& operator sym_ (const array_xpr<v_t,T>& v) noexcept {\
        slice_iterator iter = data_; \
        for (size_type i = 0; i < sz ; ++i, iter+=str) *iter sym_ v[i];\
        return *this;    
    }
PETLIB_MAKE_OPS(=)    
PETLIB_MAKE_OPS(+=)    
PETLIB_MAKE_OPS(-=)
PETLIB_MAKE_OPS(*=)    
PETLIB_MAKE_OPS(/=)
PETLIB_MAKE_OPS(^=)    
PETLIB_MAKE_OPS(|=)
PETLIB_MAKE_OPS(&=)    
PETLIB_MAKE_OPS(>>=)
PETLIB_MAKE_OPS(<<=)    
#undef PETLIB_MAKE_OPS
   
    constexpr size_type stride() const noexcept { return str;}
    constexpr size_type size() const noexcept { return sz;}
    
    constexpr reference operator[](size_type i) noexcept { return data_[i*str];}
    constexpr value_type operator[](size_type i) const noexcept { return data_[i*str];}
      
    constexpr pointer data() noexcept { return data_;}
    constexpr const pointer data() const noexcept { return data_;}
    
    constexpr bool empty() const noexcept { return (n!=0);}
    
    constexpr iterator begin() noexcept { return slice_iterator<T>(data_,str);}
    constexpr const iterator begin() const noexcept { return slice_iterator<T>(data_,str);}
    constexpr pointer end() noexcept { return data_+n;}
    constexpr const pointer end() const noexcept { return data_+n;}
    
    constexpr std::reverse_iterator<T> rbegin() noexcept { return reverse_iterator<T>(data_+n);}
    constexpr const std::reverse_iterator<T> rbegin() const noexcept { return reverse_iterator<T>(data_+n);}
    constexpr std::reverse_iterator<T> rend() noexcept { return std::reverse_iterator<T>(end());}
    constexpr const std::reverse_iterator<T> rend() const noexcept { 
        return std::reverse_iterator<T>(begin());
    }
    
    constexpr value_type amax() const noexcept {
        return std::max_element(begin(),end(),abs_cmp<T>);
    }
    constexpr value_type amin() const noexcept {
        return std::min_element(begin(),end(),abs_cmp<T>);
    }    
    constexpr size_type amax_index() const noexcept {
        return petlib::max_element_index(begin(),end(),abs_cmp<T>);
    }
    constexpr size_type amin_index() const noexcept {
        return petlib::min_element_index(begin(),end(),abs_cmp<T>);
    }    
    constexpr value_type max() const noexcept {
        return std::max_element(begin(),end());
    }
    constexpr value_type min() const noexcept {
        return std::min_element(begin(),end());
    }    
    constexpr size_type max_index() const noexcept {
        return petlib::max_element_index(begin(),end());
    }
    constexpr size_type min_index() const noexcept {
        return petlib::min_element_index(begin(),end());
    }    

    constexpr slice_array<T> apply( value_t fun( value_t ) ) const noexcept{
        varray<T> v(sz);
        for (size_type i = 0; i < sz; ++i) v.data_[i] = fun( data_[i] );
        return v;
    }
    constexpr varray<T> apply( value_t fun( const value_t& ) ) const noexcept{
        varray<T> v(sz);
        for (size_type i = 0; i < sz; ++i) v.data_[i] = fun( data_[i] );
        return v;
    }
    
    constexpr varray<T> shift(size_type n) const noexcept {
        varray<T> v(0,sz-n);
        const size_type sz_end = sz-n;
        for (size_type i=0;i<sz_end;++i) {
            v[i] = data_[i+n];    
        }
        return v;
    }

    constexpr varray<T> cshift(size_type n) const noexcept {
        varray<T> v(sz);
        const size_type sz_end = sz-n;
        for (size_type i=0;i<sz_end;++i) {
            v[i] = data_[i+n];    
        }
        for (size_type i=sz_end;i<n;++i) {
            v[i] = data_[sz_end+i];
        }
        return v;
    }
    
    constexpr void swap(slice_array<T>& v) noexcept {
        for (size_type i=0;i<sz;++i) {
            value_type tmp = data_[i];
            data_[i] = v.data_[i];
            v.data_[i] = tmp;
        }
    }

    constexpr swap_elements(size_type i, size_t j) noexcept {
        value_type tmp{data[i]};
        data_[i]= data_[j];
        data_[j]= tmp;
    }

    constexpr rotate_elements(size_type i, size_t j,T x) noexcept {
        value_type cs = std::cos(x);
        value_type sn = std::sin(x);
        value_type tmp{data[i]};
        data_[i]= cs*data_[i]-sn*data_[j];
        data_[j]= cs*data_[j]+sn*tmp;
    }

    constexpr void swap(varray<T>& v) noexcept {
        for (size_type i=0;i<sz;++i) {
            value_type tmp = data_[i];
            data_[i] = v.data_[i];
            v.data_[i] = tmp;
        }
    }

    constexpr void rotate(varray<T>& v,const T x) noexcept {
        value_type cs = std::cos(x);
        value_type sn = std::sin(x);
        for (size_type i=0;i<sz;++i) {
            value_type tmp = data_[i];
            data_[i] = cs*data_[i] - sn * v.data_[i];
            v.data_[i] = cs*v.data_[i] + sn*tmp;
        }
    }
    
    value_type norm2(const varray<T>& v) const noexcept {
        return petlib::norm2(begin(),end(),v.begin());
    }

    value_type norm2(const slice_array<T>& v) const noexcept {
        return petlib::norm2(begin(),end(),v.begin());
    }
    
    value_type norm2(const slice_array<T>& b) const noexcept {
        return petlib::norm2(begin(),end(),v.begin());        
    }
    
    slice_array<T> operator()(size_type offset,size_type len) const noexcept {
        return slice_array<T>(data_+offset,len);
    }  
    slice_array<T> operator()(size_type offset,size_type len) noexcept {
        return slice_array<T>(data_+offset,len);
    }
    varray<T> operator()( const slice& sl) const noexcept {
        varray<T> v(sl.size());
        pointer p = data_ + sl.offset();
        size_type str = sl.stride();
        for (size_t i=0;i<sl.size();++i,p+=str) {
            v[i] = *p;
        }
        return v;
    }  
    slice_array<T> operator()( const slice& sl) noexcept {
        return slice_array<T>(data_+sl.offset(),sl.size(),sl.stride());
    }
    varray<T> operator()( const varray<bool>& mask) const noexcept {
        size_type mask_size = 0;
        for (size_t i=0;i<sz;++i) {
            if (mask[i]) ++mask_size;
        }
        varray<T> v(mask_size);
        size_type curr(0);
        for (size_t i=0;i<sz;++i) {
            if (mask[i]) {
                v[curr] = data_[i];
                ++curr;
            }
        }
        return v;
    }  
    varray<T> operator()( const varray<size_type>& indx) const noexcept {
        varray<T> v(sz);
        for (size_t i=0;i<sz;++i) {
            v[i] = data_[indx[i]];
        }
        return v;
    }  
};


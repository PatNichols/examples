#ifndef PUTILS_VECTOR_HPP
#define PUTILS_VECTOR_HPP
#include <cstdlib>
#include <exception>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <memory>
#include <sstream>

namespace putils {

constexpr auto default_size = 32;

struct out_of_range_exception : public std::exception {
  std::size_t index_;
  std::size_t max_index_;
  out_of_range_exception(std::size_t index, std::size_t max_index)
      : index_(index), max_index_(max_index) {}
  const char* what() const noexcept {
    std::ostringstream os;
    os << "out of range access " << index_ << " in array of size " << max_index_
       << "\n";
    std::string err_str(os.str());
    return err_str.c_str();
  }
};

template <class iter>
iter move_forward(iter first, iter last, std::size_t count) {
  iter p = last;
  iter pdst = last + count;
  iter pend = first + count - 1;
  while (pdst != pend) {
    *pdst = *p;
    --p;
    --pdst;
  }
  return pdst;
}

template <class iter>
iter move_forward(iter first, iter last) {
  iter p = last;
  iter pdst = last + 1;
  while (pdst != first) {
    *pdst = *p;
    --p;
    --pdst;
  }
  return pdst;
}

template <class iter>
iter move_back(iter first, iter last, std::size_t count) {
  iter psrc = first + count;
  iter pdst = first;
  while (psrc != last) {
    *pdst = *psrc;
    ++pdst;
    ++psrc;
  }
  return pdst;
}

template <class dst_iterator, typename T>
dst_iterator fill(dst_iterator iter, dst_iterator last, const T& v) {
  while (iter != last) {
    *iter = v;
    ++iter;
  }
  return iter;
}

template <class dst_iterator, typename T>
dst_iterator fill(dst_iterator iter, dst_iterator last, const T&& v) {
  T x{v};
  while (iter != last) {
    *iter = x;
    ++iter;
  }
  return iter;
}

template <class dst_iterator, class src_iterator>
dst_iterator copy(dst_iterator dst, dst_iterator dst_last, src_iterator src) {
  while (dst != dst_last) {
    *dst = *src;
    ++src;
    ++dst;
  }
  return dst;
}

template <class dst_iterator, class src_iterator>
dst_iterator copy2(dst_iterator dst, src_iterator src, std::size_t cnt) {
  for (std::size_t k = 0; k < cnt; ++k, ++dst, ++src) {
    *dst = *src;
  }
  return dst;
}

template <class iter>
constexpr bool bool_cmp(iter x, iter xlast, iter y) {
  while (x != xlast) {
    if (*x != *y) return false;
    ++x;
    ++y;
  }
  return true;
}

template <class iter>
constexpr bool bool_abs_cmp(iter x, iter xlast, iter y) {
  bool abs_ = [](auto x) { return (x > 0) ? x : -x; };
  while (x != xlast) {
    if (abs_(*x) != abs_(*y)) return false;
    ++x;
    ++y;
  }
  return true;
}

template <typename T, class Allocator = std::allocator<T> >
struct vector {
  typedef T value_type;
  typedef T* pointer;
  typedef T& reference;
  typedef const T* const_pointer;
  typedef const T& const_reference;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;
  typedef std::reverse_iterator<pointer> reverse_iterator;
  typedef std::reverse_iterator<const_pointer> const_reverse_iterator;
  typedef Allocator allocator_type;
  typedef T* iterator;
  typedef const_pointer const_iterator;

  pointer data_;
  size_type sz, cap;
  Allocator alloc_;

  vector() : data_(), sz(0), cap(default_size), alloc_() {
    data_ = alloc_.allocate(cap);
  }
  explicit vector(const Allocator& alloc)
      : data_(alloc.allocate(default_size)),
        sz(0),
        cap(default_size),
        alloc_(alloc) {}
  vector(size_type n, const Allocator& alloc = Allocator())
      : data_(alloc.allocate(default_size)), sz(0), cap(n), alloc_(alloc) {}
  vector(size_type n, value_type t, const Allocator& alloc = Allocator())
      : data_(alloc.allocate(n)), sz(n), cap(n), alloc_(alloc) {
    fill(data_, data_ + sz, t);
  }
  vector(size_type n, const_reference t, const Allocator& alloc = Allocator())
      : data_(alloc.allocate(n)), sz(n), cap(n), alloc_(alloc) {
    fill(data_, data_ + sz, t);
  }
  vector(size_type n, pointer p, const Allocator& alloc = Allocator())
      : data_(alloc.allocate(n)), sz(n), cap(n), alloc_(alloc) {
    copy(data_, data_ + sz, p);
  }
  vector(vector&& v) : data_(v.data_), sz(v.sz), cap(v.cap), alloc_(v.alloc_) {
    v.data_ = nullptr;
    v.sz = 0;
    v.cap = 0;
  }
  vector(const vector& v, const Allocator& alloc = Allocator())
      : data_(alloc.allocate(v.sz)), sz(v.sz), cap(v.sz), alloc_(alloc) {
    copy(data_, data_ + sz, v.data_);
  }

  vector(const std::initializer_list<T>& vlist,
         const Allocator& alloc = Allocator())
      : data_(nullptr), sz(vlist.size()), cap(vlist.size()), alloc_(alloc) {
    data_ = alloc_.allocate(sz);
    copy(data_, data_ + sz, vlist.begin());
  }

  template <class InputIter>
  vector(InputIter iter, InputIter last, const Allocator& alloc = Allocator())
      : data_(alloc.allocate(size_type(last - iter))),
        sz(size_type(last - iter)),
        cap(size_type(last - iter)),
        alloc_(alloc) {
    copy(data_, data_ + sz, iter);
  }

  constexpr vector& operator=(vector&& v) {
    data_ = v.data_;
    sz = v.sz;
    cap = v.cap;
    alloc_ = v.alloc_;
    v.data_ = nullptr;
    v.sz = 0;
    v.cap = 0;
    return *this;
  }

  constexpr vector& operator=(const vector& v) {
    if (v.sz > cap) {
      alloc_.deallocate(data_, cap);
    }
    copy(data_, data_ + v.sz, v.data_);
    sz = v.sz;
    return *this;
  }

  constexpr vector& operator=(const std::initializer_list<value_type>& ilist) {
    size_type count = ilist.size();
    size_type new_sz = sz + count;
    if ((count) > cap) grow(count);
    typename std::initializer_list<value_type>::iterator iter = ilist.begin();
    sz = count;
    copy(data_, data_ + count, iter);
    return *this;
  }

  ~vector() {
    alloc_.deallocate(data_, cap);
    data_ = nullptr;
    sz = 0;
    cap = 0;
  }

  allocator_type get_allocator() { return alloc_; }

  template <class InputIter>
  void assign(InputIter iter, InputIter last) {
    size_type n = last - iter;
    if (n > cap) grow(n);
    copy(data_, data_ + n, iter);
    sz = n;
  }

  void assign(std::initializer_list<T> ilist) {
    size_type n = ilist.size();
    if (n > cap) grow(n);
    typename std::initializer_list<T>::iterator iter = ilist.begin();
    copy(data_, data_ + n, iter);
    sz = n;
  }

  constexpr size_type size() const noexcept { return sz; }
  constexpr size_type capacity() const noexcept { return cap; }
  constexpr bool empty() const noexcept { return sz == 0; }
  constexpr bool full() const noexcept { return sz == cap; }
  constexpr pointer data() noexcept { return data_; }
  constexpr const pointer data() const noexcept { return data_; }
  constexpr void clear() noexcept { sz = 0; }
  constexpr void shrink_to_fit() noexcept {
    if (sz == cap) return;
    pointer p = alloc_.allocate(sz * sizeof(value_type));
    copy2(p, data_, sz);
    alloc_.deallocate(data_, cap);
    data_ = p;
    cap = sz;
  }
  constexpr void grow(size_type new_sz = 0) noexcept {
    if (sz == new_sz) return;
    if (new_sz == 0) new_sz = sz + sz;
    pointer p = alloc_.allocate(new_sz * sizeof(value_type));
    copy2(p, data_, sz);
    alloc_.deallocate(data_, cap);
    data_ = p;
    cap = new_sz;
  }
  constexpr void reserve(size_type n) {
    if (sz <= n) return;
    grow(n);
  }

  void push_back(const_reference t) {
    if (full()) grow();
    data_[sz] = t;
    ++sz;
    return;
  }

  void push_back(T&& v) {
    if (full()) grow();
    data_[sz] = v;
    ++sz;
    return;
  }

  void push_front(const_reference t) {
    if ((sz + 1) > cap) grow();
    if (sz == 0) {
      data_[0] = t;
      sz = 1;
      return;
    }
    if (sz == 1) {
      data_[1] = data_[0];
      data_[0] = t;
      sz = 2;
      return;
    }
    if (sz == 2) {
      data_[2] = data_[1];
      data_[1] = data_[0];
      data_[0] = t;
      sz = 3;
      return;
    }
    move_forward(data_, data_ + sz - 1, 1);
    data_[0] = t;
    ++sz;
    return;
  }

  iterator insert(iterator pos, size_type count, const_reference v) {
    difference_type n = pos - data_;
    size_type new_sz = sz + count;
    if (new_sz > cap) grow();
    move_forward(data_ + n, end(), count);
    fill(data_ + n, data_ + n + count, v);
    sz += count;
    return data_ + n;
  }

  iterator insert(iterator pos, const_reference v) {
    difference_type n = pos - data_;
    if (sz == cap) grow();
    move_forward(data_ + n, end());
    data_[n] = v;
    ++sz;
    return data_ + n;
  }

  iterator insert(iterator pos, T&& x) {
    difference_type n = pos - data_;
    if (sz == cap) grow();
    move_forward(data_ + n, end());
    data_[n] = T{x};
    ++sz;
    return data_ + n;
  }

  iterator insert(iterator pos, std::initializer_list<value_type> ilist) {
    size_type count = ilist.size();
    size_type new_sz = sz + count;
    difference_type n = pos - data_;
    if (new_sz > cap) grow();
    move_forward(data_ + n, end(), count);
    sz = new_sz;
    typename std::initializer_list<value_type>::iterator iter = ilist.begin();
    copy(data_ + n, data_ + n + count, iter);
    return data_ + n;
  }

  iterator erase(iterator pos) {
    if (sz == 0) throw out_of_range_exception(1, 0);
    move_back(pos, end());
    --sz;
    return pos;
  }

  iterator erase(iterator pos, size_type count) {
    if (sz == 0) throw out_of_range_exception(count, 0);
    move_back(pos, end(), count);
    sz -= count;
    return pos;
  }

  iterator erase(iterator first, iterator last) {
    if (sz == 0) throw out_of_range_exception((last - first), 0);
    size_type count = last - first;
    return erase(first, count);
  }

  iterator erase(const_iterator first, const_iterator last) {
    if (sz == 0) throw out_of_range_exception((last - first), 0);
    size_type count = last - first;
    return erase(first, count);
  }

  void swap(vector& v) {
    pointer p = data_;
    data_ = v.data_;
    v.data_ = p;
    value_type tsz = sz;
    sz = v.sz;
    v.sz = tsz;
    value_type tcap = cap;
    cap = v.cap;
    v.cap = tcap;
  }

  void pop_back() { --sz; }

  constexpr const_reference front() const noexcept { return data_[0]; }
  constexpr const_reference back() const noexcept { return data_[sz - 1]; }
  constexpr const_reference operator[](size_type i) const noexcept {
    return data_[i];
  }
  constexpr const_reference at(size_type i) const noexcept {
    if (i < sz) return data_[i];
    throw out_of_range_exception(i, sz);
  }
  constexpr reference front() noexcept { return data_[0]; }
  constexpr reference back() noexcept { return data_[sz - 1]; }
  constexpr reference operator[](size_type i) noexcept { return data_[i]; }
  constexpr reference at(size_type i) noexcept {
    if (i < sz) return data_[i];
    throw out_of_range_exception(i, sz);
  }

  constexpr pointer begin() noexcept { return data_; }
  constexpr pointer end() noexcept { return (data_ + sz); }
  constexpr reverse_iterator rbegin() noexcept {
    return reverse_iterator(end());
  }
  constexpr reverse_iterator rend() noexcept {
    return reverse_iterator(begin());
  }
  constexpr const pointer begin() const noexcept { return data_; }
  constexpr const pointer end() const noexcept { return (data_ + sz); }
  constexpr const_reverse_iterator rbegin() const noexcept {
    return const_reverse_iterator(end());
  }
  constexpr const_reverse_iterator rend() const noexcept {
    return const_reverse_iterator(begin());
  }
};

template <typename allocator_1, typename allocator_2, typename T>
constexpr bool operator==(const vector<T, allocator_1>& lhs,
                          const vector<T, allocator_2>& rhs) {
  if (lhs.size() != rhs.size()) return false;
  return bool_cmp(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename allocator_1, typename allocator_2, typename T>
constexpr bool operator!=(const vector<T, allocator_1>& lhs,
                          const vector<T, allocator_2>& rhs) {
  if (lhs.size() != rhs.size()) return true;
  return !bool_cmp(lhs.begin(), lhs.end(), rhs.begin());
}

}

}  // namespace putils
#endif
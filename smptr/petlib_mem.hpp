#ifndef PETLIB_MEM_HPP
#define PETLIB_MEM_HPP

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <new>

namespace petlib {

#define PETLIB_ALIGN size_t(32)

#ifndef HAVE_ALIGNED_ALLOC

template <typename T>
T* get_storage(size_t n) {
  void* px = nullptr;
  int err = posix_memalign(&px, PETLIB_ALIGN, sizeof(T) * n);
  if (err || px == nullptr) {
    std::cerr << "get_storage(" << (n * sizeof(T)) << ") failed!\n";
    throw std::bad_alloc();
  }
  return reinterpret_cast<T*>(px);
}

#else

template <typename T>
T* get_storage(size_t n) {
  T* ptr = reinterpret_cast<T*>(posix_memalign(PETLIB_ALIGN, n * sizeof(T)));
  if (ptr != nullptr) {
    return ptr;
  }
  std::cerr << "get_storage(" << (n * sizeof(T)) << ") failed!\n";
  throw std::bad_alloc();
}

#endif

template <typename T>
constexpr void free_storage(T* ptr) {
  std::free(ptr);
}

/**
 * @class aligned_allocator
 * @brief Allocator for aligned memory
 *
 * The aligned_allocator class template is an allocator that
 * performs memory allocation aligned by the specified value.
 *
 * @tparam T type of objects to allocate.
 * @tparam Align alignment in bytes.
 */
template <class T, size_t Align = PETLIB_ALIGN>
class aligned_allocator {
 public:
  typedef T value_type;
  typedef T* pointer;
  typedef const T* const_pointer;
  typedef T& reference;
  typedef const T& const_reference;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;

  static constexpr size_t alignment = PETLIB_ALIGN;

  template <class U>
  struct rebind {
    using other = aligned_allocator<U, Align>;
  };

  aligned_allocator() noexcept;
  aligned_allocator(const aligned_allocator& rhs) noexcept;

  template <class U>
  aligned_allocator(const aligned_allocator<U, Align>& rhs) noexcept;

  ~aligned_allocator();

  pointer address(reference) noexcept;
  const_pointer address(const_reference) const noexcept;

  pointer allocate(size_type n, const void* hint = 0);
  void deallocate(pointer p, size_type n);

  size_type max_size() const noexcept;
  size_type size_max() const noexcept;

  template <class U, class... Args>
  void construct(U* p, Args&&... args);

  template <class U>
  void destroy(U* p);
};

template <class T1, size_t Align1, class T2, size_t Align2>
bool operator==(const aligned_allocator<T1, Align1>& lhs,
                const aligned_allocator<T2, Align2>& rhs) noexcept;

template <class T1, size_t Align1, class T2, size_t Align2>
bool operator!=(const aligned_allocator<T1, Align1>& lhs,
                const aligned_allocator<T2, Align2>& rhs) noexcept;

template <class T>
size_t get_alignment_offset(const T* p, size_t size, size_t block_size);

/************************************
 * aligned_allocator implementation *
 ************************************/

/**
 * Default constructor.
 */
template <class T, size_t A>
inline aligned_allocator<T, A>::aligned_allocator() noexcept {}

/**
 * Copy constructor.
 */
template <class T, size_t A>
inline aligned_allocator<T, A>::aligned_allocator(
    const aligned_allocator&) noexcept {}

/**
 * Extended copy constructor.
 */
template <class T, size_t A>
template <class U>
inline aligned_allocator<T, A>::aligned_allocator(
    const aligned_allocator<U, A>&) noexcept {}

/**
 * Destructor.
 */
template <class T, size_t A>
inline aligned_allocator<T, A>::~aligned_allocator() {}

/**
 * Returns the actual address of \c r even in presence of overloaded \c
 * operator&.
 * @param r the object to acquire address of.
 * @return the actual address of \c r.
 */
template <class T, size_t A>
inline auto aligned_allocator<T, A>::address(reference r) noexcept -> pointer {
  return &r;
}

/**
 * Returns the actual address of \c r even in presence of overloaded \c
 * operator&.
 * @param r the object to acquire address of.
 * @return the actual address of \c r.
 */
template <class T, size_t A>
inline auto aligned_allocator<T, A>::address(const_reference r) const noexcept
    -> const_pointer {
  return &r;
}

/**
 * Allocates <tt>n * sizeof(T)</tt> bytes of uninitialized memory, aligned by \c
 * A. The alignment may require some extra memory allocation.
 * @param n the number of objects to allocate storage for.
 * @param hint unused parameter provided for standard compliance.
 * @return a pointer to the first byte of a memory block suitably aligned and
 * sufficient to hold an array of \c n objects of type \c T.
 */
template <class T, size_t A>
inline auto aligned_allocator<T, A>::allocate(size_type n, const void*)
    -> pointer {
  pointer res = get_storage<T>(n);
  if (res == nullptr) throw std::bad_alloc();
  return res;
}

/**
 * Deallocates the storage referenced by the pointer p, which must be a pointer
 * obtained by an earlier call to allocate(). The argument \c n must be equal to
 * the first argument of the call to allocate() that originally produced \c p;
 * otherwise, the behavior is undefined.
 * @param p pointer obtained from allocate().
 * @param n number of objects earlier passed to allocate().
 */
template <class T, size_t A>
inline void aligned_allocator<T, A>::deallocate(pointer p, size_type) {
  free_storage<T>(p);
}

/**
 * Returns the maximum theoretically possible value of \c n, for which the
 * call allocate(n, 0) could succeed.
 * @return the maximum supported allocated size.
 */
template <class T, size_t A>
inline auto aligned_allocator<T, A>::max_size() const noexcept -> size_type {
  return size_type(-1) / sizeof(T);
}

/**
 * This method is deprecated, use max_size() instead
 */
template <class T, size_t A>
inline auto aligned_allocator<T, A>::size_max() const noexcept -> size_type {
  return size_type(-1) / sizeof(T);
}

/**
 * Constructs an object of type \c T in allocated uninitialized memory
 * pointed to by \c p, using placement-new.
 * @param p pointer to allocated uninitialized memory.
 * @param args the constructor arguments to use.
 */
template <class T, size_t A>
template <class U, class... Args>
inline void aligned_allocator<T, A>::construct(U* p, Args&&... args) {
  new ((void*)p) U(std::forward<Args>(args)...);
}

/**
 * Calls the destructor of the object pointed to by \c p.
 * @param p pointer to the object that is going to be destroyed.
 */
template <class T, size_t A>
template <class U>
inline void aligned_allocator<T, A>::destroy(U* p) {
  p->~U();
}

/**
 * @defgroup allocator_comparison Comparison operators
 */

/**
 * @ingroup allocator_comparison
 * Compares two aligned memory allocator for equality. Since allocators
 * are stateless, return \c true iff <tt>A1 == A2</tt>.
 * @param lhs aligned_allocator to compare.
 * @param rhs aligned_allocator to compare.
 * @return true if the allocators have the same alignment.
 */
template <class T1, size_t A1, class T2, size_t A2>
inline bool operator==(const aligned_allocator<T1, A1>& lhs,
                       const aligned_allocator<T2, A2>& rhs) noexcept {
  return lhs.alignment == rhs.alignment;
}

/**
 * @ingroup allocator_comparison
 * Compares two aligned memory allocator for inequality. Since allocators
 * are stateless, return \c true iff <tt>A1 != A2</tt>.
 * @param lhs aligned_allocator to compare.
 * @param rhs aligned_allocator to compare.
 * @return true if the allocators have different alignments.
 */
template <class T1, size_t A1, class T2, size_t A2>
inline bool operator!=(const aligned_allocator<T1, A1>& lhs,
                       const aligned_allocator<T2, A2>& rhs) noexcept {
  return !(lhs == rhs);
}

}  // namespace petlib
#endif
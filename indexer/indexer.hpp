#ifndef PUTILS_INDEXER_HPP
#define PUTILS_INDEXER_HPP
#include <cstddef>
#include <cstdlib>

namespace putils {

struct ColMajorIndexer {
  int64_t ndim;
  int64_t dims[7];
  int64_t strs[7];

  explicit ColMajorIndexer(int64_t ndims, int64_t *dim_size) {
    ndim = ndims;
    int64_t str = 1;
    for (auto k = 0; k < ndim; ++k) {
      dims[k] = dim_size[k];
      strs[k] = str;
      str *= dims[k];
    }
  }

  void set(int64_t ndims, int64_t *dim_size) {
    ndim = ndims;
    int64_t str = 1;
    for (auto k = 0; k < ndim; ++k) {
      dims[k] = dim_size[k];
      strs[k] = str;
      str *= dims[k];
    }
  }

  int64_t IndicesFromFlatIndex(const int64_t flat_index,
                                    int64_t *ind) const noexcept {
    int64_t f = flat_index;
    for (auto k = 0; k < ndim; ++k) {
      ind[k] = f % dims[k];
      f = (f - ind[k]) / dims[k];
    }
    return f;
  }

  int64_t FlatIndexFromIndices(const int64_t *ind) const noexcept {
    int64_t f = 0;
    for (auto k = 0; k < ndim; ++k) {
      f += ind[k] * strs[k];
    }
    return f;
  }
  constexpr int64_t extent(int64_t i) const noexcept {
    return dims[i];
  }
  constexpr int64_t numberOfDimensions() const noexcept { return ndim; }
  constexpr int64_t Stride(int64_t i) const noexcept {
    return strs[i];
  }
};

struct RowMajorIndexer {
  int64_t ndim;
  int64_t dims[7];
  int64_t strs[7];

  explicit RowMajorIndexer(int64_t ndims, int64_t *dim_size) {
    ndim = ndims;
    int64_t str = 1;
    for (auto k = 0; k < ndim; ++k) {
      dims[k] = dim_size[k];
    }
    for (auto k = ndim - 1; k >= 0; --k) {
      strs[k] = str;
      str *= dim_size[k];
    }
  }

  void set(int64_t ndims, int64_t *dim_size) {
    ndim = ndims;
    int64_t str = 1;
    for (auto k = 0; k < ndim; ++k) {
      dims[k] = dim_size[k];
    }
    for (auto k = ndim - 1; k >= 0; --k) {
      strs[k] = str;
      str *= dim_size[k];
    }
  }

  int64_t IndicesFromFlatIndex(const int64_t flat_index,
                                    int64_t *ind) const noexcept {
    int64_t f = flat_index;
    for (auto k = ndim - 1; k >= 0; --k) {
      ind[k] = f % dims[k];
      f = (f - ind[k]) / dims[k];
    }
    return f;
  }
  int64_t FlatIndexFromIndices(const int64_t *ind) const noexcept {
    int64_t f = 0;
    for (auto k = 0; k < ndim; ++k) {
      f += ind[k] * strs[k];
    }
    return f;
  }
  constexpr int64_t extent(int64_t i) const noexcept {
    return dims[i];
  }
  constexpr int64_t numberOfDimensions() const noexcept { return ndim; }
  constexpr int64_t Stride(int64_t i) const noexcept {
    return strs[i];
  }
};

struct FortranIndexer {
  int64_t ndim;
  int64_t dims[7];
  int64_t strs[7];

  explicit FortranIndexer(int64_t ndims, int64_t *dim_size) {
    ndim = ndims;
    int64_t str = 1;
    for (auto k = 0; k < ndim; ++k) {
      dims[k] = dim_size[k];
      strs[k] = str;
      str *= dims[k];
    }
  }

  void set(int64_t ndims, int64_t *dim_size) {
    ndim = ndims;
    int64_t str = 1;
    for (auto k = 0; k < ndim; ++k) {
      dims[k] = dim_size[k];
      strs[k] = str;
      str *= dims[k];
    }
  }

  int64_t IndicesFromFlatIndex(const int64_t flat_index,
                                    int64_t *ind) const noexcept {
    int64_t f = flat_index;
    for (auto k = 0; k < ndim; ++k) {
      auto cind = f % dims[k];
      ind[k] = cind + 1;
      f = (f - cind) / dims[k];
    }
    return f;
  }
  int64_t FlatIndexFromIndices(const int64_t *ind) const noexcept {
    int64_t f = 0;
    for (auto k = 0; k < ndim; ++k) {
      f += (ind[k] - 1) * strs[k];
    }
    return f;
  }
  constexpr int64_t extent(int64_t i) const noexcept {
    return dims[i];
  }
  constexpr int64_t numberOfDimensions() const noexcept { return ndim; }
  constexpr int64_t Stride(int64_t i) const noexcept {
    return strs[i];
  }
};

}  // namespace putils
#endif

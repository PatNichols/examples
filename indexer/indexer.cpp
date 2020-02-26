#include "indexer.hpp"
#include <cstddef>
#include <cstdlib>
#include <iostream>

void test_cm3(std::int64_t *szs) {
  putils::ColMajorIndexer indexer(3, szs);
  std::int64_t ndim = 3;
  std::int64_t cnt = 0;
  std::int64_t indx[7];
  std::int64_t cindx[7];
  for (auto i = 0; i < szs[2]; ++i) {
    for (auto j = 0; j < szs[1]; ++j) {
      for (auto k = 0; k < szs[0]; ++k, ++cnt) {
        indx[0] = k;
        indx[1] = j;
        indx[2] = i;
        std::int64_t f = indexer.FlatIndexFromIndices(indx);
        if (f - cnt) {
          std::cerr << "ERROR flat index failed " << f << " " << cnt << "\n";
        } else {
          std::cerr << "flat index = " << f << "\n";
        }
        std::cerr << " ind = ( " << indx[0];
        for (auto l = 1; l < ndim; ++l) {
          std::cerr << ", " << indx[l];
        }
        std::cerr << ")\n";
        f = indexer.IndicesFromFlatIndex(cnt, cindx);
        auto err = 0;
        std::cerr << " ind = ( " << cindx[0];
        err = (cindx[0] - indx[0]) * (cindx[0] - indx[0]);
        for (auto l = 1; l < ndim; ++l) {
          std::cerr << ", " << cindx[l];
          err += (cindx[l] - indx[l]) * (cindx[l] - indx[l]);
        }
        std::cerr << ")\n";
        if (err) {
          std::cerr << "ERROR INDICES FAILED\n";
        }
      }
    }
  }
}

void test_rm3(std::int64_t *szs) {
  putils::RowMajorIndexer indexer(3, szs);
  std::int64_t ndim = 3;
  std::int64_t cnt = 0;
  std::int64_t indx[7];
  std::int64_t cindx[7];
  for (auto i = 0; i < szs[0]; ++i) {
    for (auto j = 0; j < szs[1]; ++j) {
      for (auto k = 0; k < szs[2]; ++k, ++cnt) {
        indx[0] = i;
        indx[1] = j;
        indx[2] = k;
        std::int64_t f = indexer.FlatIndexFromIndices(indx);
        if (f - cnt) {
          std::cerr << "ERROR flat index failed " << f << " " << cnt << "\n";
        } else {
          std::cerr << "flat index = " << f << "\n";
        }
        std::cerr << " ind = ( " << indx[0];
        for (auto l = 1; l < ndim; ++l) {
          std::cerr << ", " << indx[l];
        }
        std::cerr << ")\n";
        f = indexer.IndicesFromFlatIndex(cnt, cindx);
        auto err = 0;
        std::cerr << " ind = ( " << cindx[0];
        err = (cindx[0] - indx[0]) * (cindx[0] - indx[0]);
        for (auto l = 1; l < ndim; ++l) {
          std::cerr << ", " << cindx[l];
          err += (cindx[l] - indx[l]) * (cindx[l] - indx[l]);
        }
        std::cerr << ")\n";
        if (err) {
          std::cerr << "ERROR INDICES FAILED\n";
        }
      }
    }
  }
}

void test_fm3(std::int64_t *szs) {
  putils::FortranIndexer indexer(3, szs);

  std::int64_t ndim = 3;
  std::int64_t cnt = 0;
  std::int64_t indx[7];
  std::int64_t cindx[7];
  for (auto i = 0; i < szs[2]; ++i) {
    for (auto j = 0; j < szs[1]; ++j) {
      for (auto k = 0; k < szs[0]; ++k, ++cnt) {
        indx[0] = k + 1;
        indx[1] = j + 1;
        indx[2] = i + 1;
        std::int64_t f = indexer.FlatIndexFromIndices(indx);
        if (f - cnt) {
          std::cerr << "ERROR flat index failed " << f << " " << cnt << "\n";
        } else {
          std::cerr << "flat index = " << f << "\n";
        }
        std::cerr << " ind = ( " << indx[0];
        for (auto l = 1; l < ndim; ++l) {
          std::cerr << ", " << indx[l];
        }
        std::cerr << ")\n";
        f = indexer.IndicesFromFlatIndex(cnt, cindx);
        auto err = 0;
        std::cerr << " ind = ( " << cindx[0];
        err = (cindx[0] - indx[0]) * (cindx[0] - indx[0]);
        for (auto l = 1; l < ndim; ++l) {
          std::cerr << ", " << cindx[l];
          err += (cindx[l] - indx[l]) * (cindx[l] - indx[l]);
        }
        std::cerr << ")\n";
        if (err) {
          std::cerr << "ERROR INDICES FAILED\n";
        }
      }
    }
  }
}

void test_cm4(std::int64_t *szs) {
  putils::ColMajorIndexer indexer(4, szs);

  std::int64_t ndim = 4;
  std::int64_t cnt = 0;
  std::int64_t indx[7];
  std::int64_t cindx[7];
  for (auto m = 0; m < szs[3]; ++m) {
    for (auto i = 0; i < szs[2]; ++i) {
      for (auto j = 0; j < szs[1]; ++j) {
        for (auto k = 0; k < szs[0]; ++k, ++cnt) {
          indx[0] = k;
          indx[1] = j;
          indx[2] = i;
          indx[3] = m;
          std::int64_t f = indexer.FlatIndexFromIndices(indx);
          if (f - cnt) {
            std::cerr << "ERROR flat index failed " << f << " " << cnt << "\n";
          } else {
            std::cerr << "flat index = " << f << "\n";
          }
          std::cerr << " ind = ( " << indx[0];
          for (auto l = 1; l < ndim; ++l) {
            std::cerr << ", " << indx[l];
          }
          std::cerr << ")\n";
          f = indexer.IndicesFromFlatIndex(cnt, cindx);
          auto err = 0;
          std::cerr << " ind = ( " << cindx[0];
          err = (cindx[0] - indx[0]) * (cindx[0] - indx[0]);
          for (auto l = 1; l < ndim; ++l) {
            std::cerr << ", " << cindx[l];
            err += (cindx[l] - indx[l]) * (cindx[l] - indx[l]);
          }
          std::cerr << ")\n";
          if (err) {
            std::cerr << "ERROR INDICES FAILED\n";
          }
        }
      }
    }
  }
}

void test_rm4(std::int64_t *szs) {
  putils::RowMajorIndexer indexer(4, szs);
  std::int64_t ndim = 4;
  std::int64_t cnt = 0;
  std::int64_t indx[7];
  std::int64_t cindx[7];
  for (auto i = 0; i < szs[0]; ++i) {
    for (auto j = 0; j < szs[1]; ++j) {
      for (auto k = 0; k < szs[2]; ++k) {
        for (auto m = 0; m < szs[3]; ++m, ++cnt) {
          indx[0] = i;
          indx[1] = j;
          indx[2] = k;
          indx[3] = m;
          std::int64_t f = indexer.FlatIndexFromIndices(indx);
          if (f - cnt) {
            std::cerr << "ERROR flat index failed " << f << " " << cnt << "\n";
          } else {
            std::cerr << "flat index = " << f << "\n";
          }
          std::cerr << " ind = ( " << indx[0];
          for (auto l = 1; l < ndim; ++l) {
            std::cerr << ", " << indx[l];
          }
          std::cerr << ")\n";
          f = indexer.IndicesFromFlatIndex(cnt, cindx);
          auto err = 0;
          std::cerr << " ind = ( " << cindx[0];
          err = (cindx[0] - indx[0]) * (cindx[0] - indx[0]);
          for (auto l = 1; l < ndim; ++l) {
            std::cerr << ", " << cindx[l];
            err += (cindx[l] - indx[l]) * (cindx[l] - indx[l]);
          }
          std::cerr << ")\n";
          if (err) {
            std::cerr << "ERROR INDICES FAILED\n";
          }
        }
      }
    }
  }
}

void test_fm4(std::int64_t *szs) {
  putils::FortranIndexer indexer(4, szs);

  std::int64_t ndim = 4;
  std::int64_t cnt = 0;
  std::int64_t indx[7];
  std::int64_t cindx[7];
  for (auto m = 0; m < szs[3]; ++m) {
    for (auto i = 0; i < szs[2]; ++i) {
      for (auto j = 0; j < szs[1]; ++j) {
        for (auto k = 0; k < szs[0]; ++k, ++cnt) {
          indx[0] = k + 1;
          indx[1] = j + 1;
          indx[2] = i + 1;
          indx[3] = m + 1;
          std::int64_t f = indexer.FlatIndexFromIndices(indx);
          if (f - cnt) {
            std::cerr << "ERROR flat index failed " << f << " " << cnt << "\n";
          } else {
            std::cerr << "flat index = " << f << "\n";
          }
          std::cerr << " ind = ( " << indx[0];
          for (auto l = 1; l < ndim; ++l) {
            std::cerr << ", " << indx[l];
          }
          std::cerr << ")\n";
          f = indexer.IndicesFromFlatIndex(cnt, cindx);
          auto err = 0;
          std::cerr << " ind = ( " << cindx[0];
          err = (cindx[0] - indx[0]) * (cindx[0] - indx[0]);
          for (auto l = 1; l < ndim; ++l) {
            std::cerr << ", " << cindx[l];
            err += (cindx[l] - indx[l]) * (cindx[l] - indx[l]);
          }
          std::cerr << ")\n";
          if (err) {
            std::cerr << "ERROR INDICES FAILED\n";
          }
        }
      }
    }
  }
}

int main() {
  int64_t szs[7];

  szs[0] = 6;
  szs[1] = 4;
  szs[2] = 5;
  szs[3] = 7;

  test_cm3(szs);
  test_rm3(szs);
  test_fm3(szs);
  test_cm4(szs);
  test_rm4(szs);
  test_fm4(szs);
}
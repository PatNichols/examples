#include <iostream>
#include <cstdlib>
#include <memory>
#include <new>
#include <type_traits>


/// below are operator overloads
/*
void * operator new(std::size_t sz) {
    void * ptr = std::aligned_alloc(ALIGN,sz);
    if (ptr) return ptr;
    throw std::bad_alloc();
}
void * operator new [] (std::size_t sz) {
    void * ptr = std::aligned_alloc(ALIGN,sz);
    return ptr;
}

void operator delete(void *ptr) {
    free(ptr);
}

void operator delete [] ( void *ptr) {
    free(ptr);
}

*/
//#__STDCPP_DEFAULT_NEW_ALIGNMENT__(32)




int main() {
    constexpr std::size_t n=10;

/// use new default with standard alignment
    double * z = new double[n];
    double *zp0 = z;
    for (auto i=0UL;i<n;++i) {
       z[i] = static_cast<double>(i);
       double * zpi = &(z[i]);
       std::ptrdiff_t dz= (zpi - zp0);
       std::cout << i << "  " << (&z[i]) << " " << z[i]
             << " " << dz << "\n";
    }

    std::cout << (std::ptrdiff_t(z)%64) << "\n";

    delete [] z;
/// use new with placement-like option 
    constexpr std::align_val_t al {64};
    double * x = new(al) double[n];

    double *xp0 = x;
    for (auto i=0;i<n;++i) {
       x[i] = static_cast<double>(i);
       double * xpi = &(x[i]);
       std::ptrdiff_t dx= (xpi - xp0);
       std::cout << i << "  " << (&x[i]) << " " << x[i]
             << " " << dx << "\n";
    }

    std::cout << (std::ptrdiff_t(x)%64) << "\n";

    delete [] x;
//////////////////
//// use std::aligned_alloc for allocation from C-11
////
    double * y = reinterpret_cast<double*>(std::aligned_alloc(std::size_t(64),sizeof(double)*10));

    for (auto i = 0 ; i < n ; ++i)
    {
        x[i]=static_cast<double>(i);
        double * xpi = &y[i];
        std::ptrdiff_t dx= xpi - y;
        std::cout << i << "  " << (&x[i]) << " " << x[i]
             << " " << dx << "\n";       
    }
    std::cout << (std::ptrdiff_t(y)%64) << "\n";
    free(y);
}
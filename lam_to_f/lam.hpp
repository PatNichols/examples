#ifndef LAM_HPP
#define LAM_HPP
#include <cstdlib>
#include <cstddef>
#include <iostream>

namespace lam_space {

inline auto flam = [] ( const int i, const double r) {
    std::cerr << "called flam with " << i << " and " << r << "\n";
    std::cerr << "should return " << (r+i) << "\n";
    return r+i;
};

inline auto glam = [] ( const int i, const double r) {
    std::cerr << "called glam with " << i << " and " << r << "\n";
    std::cerr << "should return " << (r+r) << "\n";
    return r+r;
};

struct lam_holder {

    static constexpr auto xlam = [] (const int i,const double r) noexcept {
        std::cerr << "called lam_holder::xlam with " << i << " and " << r << "\n";
        std::cerr << "should return " << (r+r+r) << "\n";
        return r+r+r;
    };

};

}
#endif
#ifndef LAM_H_
#define LAM_H_
#include <functional>
#include <utility>
#include "lam.hpp"

extern "C" {

typedef double (*ffun_t)(int,double);

ffun_t c_get_flam();

ffun_t c_get_glam();

ffun_t c_get_xlam();

}
#endif
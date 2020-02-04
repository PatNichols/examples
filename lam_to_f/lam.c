
#include "lam.h"

extern "C" {

ffun_t c_get_flam() {
    return lam_space::flam;
}

ffun_t c_get_glam() {
    return lam_space::glam;
}

ffun_t c_get_xlam() {
    return lam_space::lam_holder::xlam;
}

}
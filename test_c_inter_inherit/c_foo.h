#pragma once
#include <cstdlib>
#include <string>
#include "foo.hpp"

struct foo_wrap
{
    Foo f;
};

#define CPTR_TO_CLASS(p,TYPE) reinterpret_cast< TYPE *>((p))

typedef struct foo_wrap foo_wrap;

extern "C"
{

double foo_wrap_dval(void *ptr);

char * foo_wrap_sval(void *ptr);

foo_wrap * foo_wrap_ctor();

void foo_wrap_dtor(void *ptr);


}
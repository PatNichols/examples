#include "c_foo.h"

extern "C"
{

double foo_wrap_dval(void *ptr)
{
    foo_wrap * w =reinterpret_cast< foo_wrap *>(ptr);
    return (w->f).dvalue();
}

char * foo_wrap_sval(void *ptr)
{
    foo_wrap * w =reinterpret_cast< foo_wrap *>(ptr);
    std::string sw((w->f).svalue());
    size_t sz = sw.size();
    char * res = (char*)malloc(sz+1);
    for (size_t i=0;i<sz;++i) res[i] = sw[i];
    res[sz] = '\0';
    return res;
}

foo_wrap * foo_wrap_ctor()
{
    foo_wrap * w = new foo_wrap;
    return w;
}

void foo_wrap_dtor(void *ptr)
{
    foo_wrap * w = reinterpret_cast< foo_wrap *>(ptr);
    delete w;
    w = nullptr;
}

}

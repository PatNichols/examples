#pragma once
#include <cstdlib>
#include <iostream>

namespace FooSpace {
struct Foo {
    int i1;
//public:
    Foo():
        i1(-1)
    {}
    explicit Foo(int i1_):
        i1(i1_)
    {}

    Foo(const Foo& f):
        i1(f.i1)
    {}

    Foo& operator=(const Foo& f1) {
        i1 = f1.i1;
        return *this;
    }

    virtual void report()
    {
        std::cout << "Foo answers\n";
    }
    virtual int get_int()
    {
        return i1;
    }
};

class FooBar : public Foo
{
    int ix;
public:
    FooBar():
        Foo(-2),
        ix(1)
    {}
    explicit FooBar(int i1_,int ix1_=3):
        Foo(i1_),
        ix(ix1_)
    {}

    explicit FooBar(Foo& f1):
        Foo(f1.i1),
        ix(i1)
    {}

    FooBar& operator=(const Foo& f1) {
        i1 = f1.i1;
        return *this;
    }

    virtual void report()
    {
        std::cout << "FooBar answers\n";
    }
    virtual int get_int()
    {
        return Foo::i1;
    }
    virtual int get_int_x() { return ix;}
};

}

#define VOID_TO_CXX(TYPE,pin,pout) TYPE * pout = reinterpret_cast< TYPE * >(pin)

typedef void * foo_c_t;
typedef void * foobar_c_t;
extern "C"
{
void * foo_c_alloc();
void * foobar_c_alloc();
void foo_c_dealloc(void **p);
void foo_c_report(void *p);
int foo_c_get_int(void *p);
void foo_c_copy(void **fout,void *fin);
}


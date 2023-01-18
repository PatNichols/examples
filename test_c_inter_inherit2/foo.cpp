#include "foo.hpp"

extern "C"
{

void * foo_c_alloc() {
    return (void*)new FooSpace::Foo();
}

void * foobar_c_alloc() {
    return (void*)new FooSpace::FooBar();
}

void foo_c_dealloc(void **p) {
    VOID_TO_CXX(FooSpace::Foo,*p,f);
    if ( f != nullptr) delete f;
    f = nullptr;
}

void foo_c_report(void *p) {
    VOID_TO_CXX(FooSpace::Foo,p,f);
    f->report();
}

int foo_c_get_int(void *p) {
    VOID_TO_CXX(FooSpace::Foo,p,f);
    return f->get_int();
}

void foo_c_copy(void **fout,void *fin) {
    VOID_TO_CXX(FooSpace::Foo,fin,foo_in);
    VOID_TO_CXX(FooSpace::Foo*,fout,foo_out);
    if ( foo_in == nullptr ) {
       if ( *foo_out != nullptr) delete [] *foo_out; 
       *foo_out = nullptr;
    } else {
         if ( *foo_out == nullptr ) {
             *foo_out = new FooSpace::Foo();
         } else {
              **foo_out = *foo_in;
         }
    }
    *fout = (void*)(*foo_out);
}

}





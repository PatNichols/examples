#include "foo.hpp"
#include <stdio.h>
int main()
{
    int k;
    void * f1 = 0x0;
    void * f2 = 0x0;
    void * fb = 0x0;
    void * f3 = 0x0;
    
    f1 = foo_c_alloc();
    fb = foobar_c_alloc();
    
    fprintf(stderr,"foo\n");
    foo_c_report(f1);
    k = foo_c_get_int(f1);
    fprintf(stderr,"foo get int %d\n",k);

    fprintf(stderr,"foo bar\n");
    foo_c_report(fb);
    k = foo_c_get_int(fb);
    fprintf(stderr,"foobar get int %d\n",k);
    

    foo_c_copy(&f2,f1);
    fprintf(stderr,"foo2\n");
    foo_c_report(f2);
    k = foo_c_get_int(f2);
    fprintf(stderr,"foo2 get int %d\n",k);

    fprintf(stderr,"cleanup \n");

    foo_c_dealloc(&f2);    
    foo_c_dealloc(&fb);
    foo_c_dealloc(&f1);
    return EXIT_SUCCESS;
}


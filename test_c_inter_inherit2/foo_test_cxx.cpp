#include <iostream>
#include "foo.hpp"

int main()
{
    FooSpace::Foo f;
    FooSpace::FooBar fb;
    
    f.report();
    int k = f.get_int();
    std::cout << "foo get_int returned " << k << "\n";
    fb.report();
    k = fb.get_int();
    std::cout << "foobar get_int returned " << k << "\n";
    k = fb.get_int_x();
    std::cout << "foobar get_int_X returned " << k << "\n";    

    FooSpace::Foo f2 = fb;
    f2.report();
    k = f2.get_int();
    
    std::cout << "f2=foobar get_int returned " << k << "\n";    
    f2 = f;
    k = f2.get_int();
    std::cout << "f2=foo get_int returned " << k << "\n";    

    FooSpace::Foo * fp = new FooSpace::Foo(f);
    k = fp->get_int();
    fp->report();
    std::cout << "f ptr get_int returned " << k << "\n";    
    delete fp;
    return EXIT_SUCCESS;  
}

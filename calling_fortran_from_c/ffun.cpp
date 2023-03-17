#include <cstdlib>
#include <iostream>


extern "C" void c_sub1(double *a,double *b,double *c,const int n);
//extern "C" double * c_fun2(double *a,double *b,double *c,int *n);
extern "C" double c_fun2(const double a,const double b,const double c,int n);

int main(int argc,char **argv)
{
    double a = 1.;
    double b = 2.;
    double c = 3.;
    int n = 1;

    std::cout << "c++ init\n";
    std::cout << "a = " << a << "\n";
    std::cout << "b = " << b << "\n";
    std::cout << "c = " << c << "\n";
    
    for (int i=0;i<n;++i) {
        double ta = a;
        double tb = b;
        double tc = c;
         c = tb;
         b = ta;
         a = tc;        
    }

    std::cout << "after c++ switch\n";
    std::cout << "a = " << a << "\n";
    std::cout << "b = " << b << "\n";
    std::cout << "c = " << c << "\n";

    a = 1.;
    b = 2.;
    c = 3.;    
    
    c_sub1(&a,&b,&c,n);
    
    std::cout << "c++ after f\n";
    std::cout << "a = " << a << "\n";
    std::cout << "b = " << b << "\n";
    std::cout << "c = " << c << "\n";

    a = 1.;
    b = 2.;
    c = 3.;

    std::cout <<" c++ init2\n";
    std::cout << "a = " << a << "\n";
    std::cout << "b = " << b << "\n";
    std::cout << "c = " << c << "\n";
    std::cout << "n = " << n << "\n";

    double d = c_fun2(a,b,c,n);

    std::cout << "f ans = " << d << "\n";

    a = 1.;
    b = 2.;
    c = 3.;

    double e = 0.;

    for (int i=0;i<n;++i) e += a*b+c;

    std::cout << "c++ ans = " << e << "\n";

    return EXIT_SUCCESS;
}
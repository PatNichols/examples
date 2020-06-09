

#include <cstdlib>
#include <iostream>
#include <fstream>
#include "quarternion.hpp"

using namespace putils;

inline double drand() {
    double x = rand();
    x/=RAND_MAX;
    x=x+x-1.;
    return x;
}

quarternion<double> create()
{
    double w = drand();
    double x = drand();
    double y = drand();
    double z = drand();
    return quarternion<double>(w,x,y,z);
}

void check_io()
{
    quarternion<double> q(create());
    quarternion<double> w;
    std::ofstream otmp("qtmp");
    otmp << std::scientific;
    otmp << std::setprecision(16);
    otmp << q;
    otmp << "\n";
    otmp.close();
    std::ifstream itmp("qtmp");
    itmp >> w;
    itmp.close();
    std::cerr <<"q = ";
    std::cerr << q << "\n";
    std::cerr <<"w = ";
    std::cerr << w << "\n";
    w-=q;
    std::cerr <<"q-w= ";
    std::cerr << w << "\n";
}


int main()
{

    quarternion<double> a(create());
    quarternion<double> b(create());
    quarternion<double> c(create());

    std::cout << "a        = " << a << "\n";
    std::cout << "aconj    = " << a.conj() << "\n";
    c= a.conj();
    c*=a;
    std::cout << "a.c *a   = " << c << "\n";
    c= a;
    c*=a.conj();
    std::cout << "a *a.c  = " << c << "\n";
    c /= a.norm();
    std::cout << "a*a.c/n  = " << c<< "\n";    
    std::cout << "ainv     = " << a.inverse() << "\n";
    c = a;
    c *= a.inverse();
    std::cout << "a * ainv = " << c << "\n";
    c = a.inverse();
    c *= a;
    std::cout << "ainv * a = " << c << "\n";
    quarternion<double> d = a;
    d *= a.inverse();
    c = a.inverse();
    c *= a;
    d = d + c;
    std::cout << "xxx      = " << d << "\n";
    std::cout << "a mag    = " << a.abs() << "\n";
    std::cout << "a norm   = " << a.norm() << "\n";
    std::cout << "1/a      = " << (1./a) << "\n";    
    std::cout << "a * bc   = " << (a*b.conj()) << "\n";
    std::cout << "b * ac   = " << (b*a.conj()) << "\n";
    std::cout << "a / b R  = " << rdiv<double>(a,b) << "\n";
    std::cout << "b / a R  = " << rdiv<double>(b,a) << "\n";
    std::cout << "a / b L  = " << ldiv<double>(a,b) << "\n";
    std::cout << "b / a L  = " << ldiv<double>(b,a) << "\n";
    c = rdiv(a,b)*rdiv(b,a);
    std::cout << "(a/b * b/a) = " << c << "\n";
    
    std::cout << "a           = " << a << "\n";
    c = a;
    std::cout << "c=a         = " << c << "\n";
    c = a;
    c *= 2.;
    std::cout << "a*2         = " << c << "\n";
    std::cout << "a*2         = " << (a*2.0) << "\n";
    c = a;
    c += 2.0;
    std::cout << "a+2         = " << c << "\n";
    std::cout << "a+2         = " << (a+2.0) << "\n";
    c = a;
    c -= 2.0;
    std::cout << "a-2         = " << c << "\n";
    std::cout << "a-2         = " << (a-2.0) << "\n";
    c = a;
    c /= 2.0;
    std::cout << "a/2         = " << c << "\n";
    std::cout << "a/2         = " << (a/2.0) << "\n";
    std::cout << "b           = " << b << "\n"; 
    std::cout << "a           = " << a << "\n";
    c = a;
    std::cout << "c=a         = " << c << "\n";\
    c = a;
    c += b;
    std::cout << "a+b         = " << c << "\n";
    std::cout << "a+b         = " << (a+b) << "\n";
    c = a;
    c -= b;
    std::cout << "a-b         = " << c << "\n";
    std::cout << "a-b         = " << (a-b) << "\n";
    c = a;
    c *= b;
    std::cout << "a*b         = " << c << "\n";
    std::cout << "a*b         = " << (a*b) << "\n";

    std::cout << "(a*a.c +a.c*a)/2 = " << (a * a.conj() + a.conj() * a )*0.5 << "\n";
    std::cout << "a.norm()    = " << a.norm() << "\n";
    std::cout << "a.abs()     = " << a.abs() << "\n";
    std::cout << "            = " << sqrt(a.norm()) << "\n";
    std::cout << "a.c/a.norm  = " << (a.conj()/a.norm()) << "\n";
    std::cout << "a inv       = " << a.inverse() << "\n";


    std::cout << "nrm(a,ainv) = " << (a * a.inverse() + a.inverse()*a)*0.5 << "\n";
    std::cout << "prod        = " << normProduct<double>(a,a) << "\n";                
    std::cout << "prod        = " << normProduct<double>(a,a) << "\n";                
    c = a/a.norm();
    std::cout << "prod        = " << normProduct<double>(a,c) << "\n";
    std::cout << "div(a,a)    = " << normDivision<double>(a,a) << "\n";
    std::cout << "prod        = " << normProduct<double>(a,b) << "\n";
    std::cout << "div(a,a)    = " << normDivision<double>(a,b) << "\n";
    check_io();
    exit(EXIT_SUCCESS);
}
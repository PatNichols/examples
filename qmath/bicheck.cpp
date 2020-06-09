
#include <cstdlib>
#include <iostream>
#include <fstream>
#include "biquarternion.hpp"

using namespace putils;



inline double drand() {
    double x = rand();
    x/=RAND_MAX;
    x=x+x-1.;
    return x;
}

biquarternion<double> create()
{
    double wr = drand();
    double xr = drand();
    double yr = drand();
    double zr = drand();
    double wi = drand();
    double xi = drand();
    double yi = drand();
    double zi = drand();
    std::complex<double> w(wr,wi);
    std::complex<double> x(xr,xi);
    std::complex<double> z(zr,zi);
    std::complex<double> y(yr,yi);
    return biquarternion<double>(w,x,y,z);
}

void check_io()
{
    biquarternion<double> q(create());
    biquarternion<double> w;
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
    if (w==q) {
        std::cerr << "They are equal!\n";
    }else{
        std::cerr << "They are not equal!\n";
    }
}


int main()
{

    biquarternion<double> a(create());
    biquarternion<double> b(create());
    biquarternion<double> c(create());

    std::cout << "a        = " << a << "\n";
    std::cout << "aconj    = " << a.conj() << "\n";
    std::cout << "aconj    = " << a.complex_conj() << "\n";
    
    c= a.conj();
    c*=a;
    std::cout << "a.c *a   = " << c << "\n";
    c= a;
    c*=a.conj();
    std::cout << "a *a.c  = " << c << "\n";

    c= a.complex_conj();
    c*=a;
    std::cout << "a.cc *a = " << c << "\n";
    c= a;
    c*=a.complex_conj();
    std::cout << "a *a.cc = " << c << "\n";

    c= a.complex_conj().conj();
    c*=a;
    std::cout << "a.ccc *a= " << c << "\n";
    c= a;
    c*=a.complex_conj().conj();
    std::cout << "a *a.ccc= " << c << "\n";
    
    c /= a.norm();
    std::cout << "a*a.c/n  = " << c<< "\n";    
    std::cout << "ainv     = " << a.inverse() << "\n";
    c = a;
    c *= a.inverse();
    std::cout << "a * ainv = " << c << "\n";
    c = a.inverse();
    c *= a;
    std::cout << "ainv * a = " << c << "\n";
    biquarternion<double> d = a;
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

    c = a * a.conj() + a.conj() * a;
    c *= 0.5;
    std::cout << "(a*a.c +a.c*a)/2. = " << c  << "\n";
    std::cout << "norm       =  " << c.norm() << "\n";
    std::cout << "abs         = " << c.abs() <<  "\n";

    d = a * a.conj();
    c = rdiv<double>(a.conj(),d);
    c = ( a * c + c * a) * 0.5;
    std::cout << "(a*a-1 +a-1*a)/2 = " << c  << "\n";
    std::cout << "norm       =  " << c.norm() << "\n";
    std::cout << "abs         = " << c.abs() <<  "\n";

    std::complex<double> zz = c.t()*std::conj(c.t());
    std::cout << "zz         = " << zz << "\n";
    
    c = a * a.conj();;  
    c = rdiv<double>( a.conj(),c);
    c = a * c;
    c = c * c.complex_conj();
    std::cout << "(a*a-1 +a-1*a)/2 = " << c  << "\n";
    std::cout << "norm       =  " << c.norm() << "\n";
    std::cout << "abs         = " << c.abs() <<  "\n";


    
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

#include <petlib_array.hpp>

template < typename T >
void test_ops()
{
  petlib::varray<T> a(5);
  petlib::varray<T> b(5);
  petlib::varray<T> c(5);
  petlib::varray<T> d(5);
  a.random_fill();
  b.random_fill();
  std::cout << a << "\n";
  std::cout << b << "\n";
  c = a;
  c += b;
  std::cout << "+ op" << c << "\n";
  d = a + b;
  c = c - d;
  std::cout << "+ op" << d << "\n";
  std::cout << "+ op" << c << "\n";
  c = a;
  c -= b;
  std::cout << "- op" << c << "\n";
  d = a - b;
  c = c - d;
  std::cout << "- op" << d << "\n";
  std::cout << "- op" << c << "\n";
  c = a;
  c *= b;
  std::cout << "* op" << c << "\n";
  d = a * b;
  c = c / d;
  std::cout << "* op" << d << "\n";
  std::cout << "* op" << c << "\n";
  c = a;
  c /= b;
  std::cout << "/ op" << c << "\n";
  d = a / b;
  c = c / d;
  std::cout << "/ op" << d << "\n";
  std::cout << "/ op" << c << "\n";

  std::cout << "a = \n" << a << "\n";
  petlib::varray<T> e(a(petlib::range(1,3)));
  std::cout << "a(1,3) =\n";
  std::cout << e << "\n";

  using petlib::slice;
    
  std::cout << "a(0,3,2) =\n";
  std::cout << a(slice(0,3,2)) << "\n";
  petlib::varray<T> vx1(a(slice(0,3,2)));
  std::cout << vx1 << "\n";
  std::cout << "a(1,2,2) =\n";
  std::cout << a(slice(1,2,2)) << "\n";
  petlib::varray<T> f = a.shift(1);
  std::cout << "a.shift1 =\n";  
  std::cout << f << "\n";
  f = a.cshift(1);
  std::cout << "a.cshift1 =\n";  
  std::cout << f << "\n";
}

template < typename T >
void test_ops2()
{
  petlib::varray<T> ax(52);
  petlib::varray<T> bx(52);
  petlib::varray<T> cx(52);
  petlib::varray<T> dx(52);
  ax.random_fill();
  bx.random_fill();
  petlib::sub_array<T> a( ax(petlib::range(34,5)) );
  petlib::sub_array<T> b( bx(petlib::range(35,5)) );
  petlib::sub_array<T> c( cx(petlib::range(36,5)) );
  petlib::sub_array<T> d( dx(petlib::range(31,5)) );
  std::cout << a << "\n";
  std::cout << b << "\n";
  c = a;
  c += b;
  std::cout << "+ op" << c << "\n";
  d = a + b;
  c = c - d;
  std::cout << "+ op" << d << "\n";
  std::cout << "+ op" << c << "\n";
  c = a;
  c -= b;
  std::cout << "- op" << c << "\n";
  d = a - b;
  c = c - d;
  std::cout << "- op" << d << "\n";
  std::cout << "- op" << c << "\n";
  c = a;
  c *= b;
  std::cout << "* op" << c << "\n";
  d = a * b;
  c = c / d;
  std::cout << "* op" << d << "\n";
  std::cout << "* op" << c << "\n";
  c = a;
  c /= b;
  std::cout << "/ op" << c << "\n";
  d = a / b;
  c = c / d;
  std::cout << "/ op" << d << "\n";
  std::cout << "/ op" << c << "\n";

  std::cout << "a = \n" << a << "\n";
  petlib::varray<T> e = a(petlib::range(1,3));
  std::cout << "a(1,3) =\n";
  std::cout << e << "\n";

  using petlib::slice;
    
  std::cout << "a(0,3,2) =\n";
  std::cout << a(slice(0,3,2)) << "\n";
  petlib::varray<T> vx1(a(slice(0,3,2)));
  std::cout << vx1 << "\n";
  std::cout << "a(1,2,2) =\n";
  std::cout << a(slice(1,2,2)) << "\n";
  petlib::varray<T> f = a.shift(1);
  std::cout << "a.shift1 =\n";  
  std::cout << f << "\n";
  f = a.cshift(1);
  std::cout << "a.cshift1 =\n";  
  std::cout << f << "\n";
}

template < typename T >
void test_ops3()
{
  petlib::varray<T> ax(52);
  petlib::varray<T> bx(52);
  petlib::varray<T> cx(52);
  petlib::varray<T> dx(52);
  ax.random_fill();
  bx.random_fill();
  petlib::slice_array<T> a( ax(petlib::slice(34,5,2)) );
  petlib::slice_array<T> b( bx(petlib::slice(35,5,2)) );
  petlib::slice_array<T> c( cx(petlib::slice(36,5,2)) );
  petlib::slice_array<T> d( dx(petlib::slice(31,5,2)) );
  std::cout << a << "\n";
  std::cout << b << "\n";
  c = a;
  c += b;
  std::cout << "+ op" << c << "\n";
  d = a + b;
  c = c - d;
  std::cout << "+ op" << d << "\n";
  std::cout << "+ op" << c << "\n";
  c = a;
  c -= b;
  std::cout << "- op" << c << "\n";
  d = a - b;
  c = c - d;
  std::cout << "- op" << d << "\n";
  std::cout << "- op" << c << "\n";
  c = a;
  c *= b;
  std::cout << "* op" << c << "\n";
  d = a * b;
  c = c / d;
  std::cout << "* op" << d << "\n";
  std::cout << "* op" << c << "\n";
  c = a;
  c /= b;
  std::cout << "/ op" << c << "\n";
  d = a / b;
  c = c / d;
  std::cout << "/ op" << d << "\n";
  std::cout << "/ op" << c << "\n";

  std::cout << "a = \n" << a << "\n";

  using petlib::slice;
  
  std::cout << "a(range(1,3)) \n";
  std::cout << a(petlib::range(1,3)) << "\n";
  
  std::cout << "a(0,3,2) =\n";
  std::cout << a(slice(0,3,2)) << "\n";
  petlib::varray<T> vx1(a(petlib::slice(0,3,2)));
  std::cout << vx1 << "\n";
  std::cout << "a(1,2,2) =\n";
  std::cout << a(slice(1,2,2)) << "\n";
  petlib::varray<T> f = a.shift(1);
  std::cout << "a.shift1 =\n";  
  std::cout << f << "\n";
  f = a.cshift(1);
  std::cout << "a.cshift1 =\n";  
  std::cout << f << "\n";
}


int main()
{

  petlib::varray<double> a(5);
  
  a.random_fill();
  
  std::cout << "a = \n" << a << "\n";
  std::cout << "a.size() = " << a.size() << "\n";

  petlib::varray<double> b(5);
  b=a;
  
  std::cout << "b = a\n" << b << "\n";
  std::cout << "b.size() = " << b.size() << "\n";
  
  petlib::varray<double> bb(a);

  std::cout << "b(a) = \n" << bb << "\n";
  std::cout << "b(a).size() = " << bb.size() << "\n";


  std::cout << "a*b" << (a*b) << "\n";
  std::cout << "a*b size = " << (a*b).size() << "\n";
  
  petlib::varray<double> c( (2.*a) );
  
  std::cout << "c = \n" << c << "\n";
  std::cout << "c.size() = " << c.size() << "\n";
  
  if (c.size()!=5) c.resize(5);
  c = 2.*b + a;
  
  std::cout << "c = \n" << c << "\n";
  std::cout << "c.size() = " << c.size() << "\n";
  
  test_ops<double>();
  test_ops2<double>();
  test_ops3<double>();
}  
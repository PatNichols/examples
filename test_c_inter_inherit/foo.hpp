#pragma once
#include <cstdlib>
#include <string>

class Foo {
public:
  Foo():d(0.2),s("this is a test") {}
  double dvalue(); 
  std::string svalue();
private:
  double d;
  std::string s;
};


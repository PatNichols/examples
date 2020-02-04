#include <iostream>
#include <utility>
#include <thread>
#include <chrono>
 
void f1(int n)
{
    for (int i = 0; i < 5; ++i) {
        std::cout << "Thread 1 executing\n";
        ++n;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
 
void f2(int& n)
{
    for (int i = 0; i < 5; ++i) {
        std::cout << "Thread 2 executing\n";
        ++n;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
 
class foo
{
public:
    void bar()
    {
        for (int i = 0; i < 3; ++i) {
            std::cout << "Thread 3 executing\n";
            ++n;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    int n = 0;
};
 
class baz
{
public:
    void operator()()
    {
        for (int i = 0; i < 5; ++i) {
            std::cout << "Thread 4 executing\n";
            ++n;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    int n = 0;
};

#include <vector>
 
int main()
{
    int n = 0;
    foo f;
    baz b;
    std::vector<std::thread> tvec;
    tvec.emplace_back();
    tvec.emplace_back(f1,n+1);
    tvec.emplace_back(f2,std::ref(n));
    tvec.emplace_back(&foo::bar,&f);
    tvec.emplace_back(b);
    std::cout << "# threads = " << tvec.size() << "\n";
    size_t k=tvec.size()-1;
    while (k!=0) {
        tvec[k].join();
        --k;
    }
    std::cout << "Final value of n is " << n << '\n';
    std::cout << "Final value of foo::n is " << f.n << '\n';
}
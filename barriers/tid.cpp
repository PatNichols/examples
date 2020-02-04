#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <iomanip>
std::mutex g_display_mutex;


 
void foo(size_t i)
{
    std::thread::id this_id = std::this_thread::get_id();
 
    g_display_mutex.lock();
    std::cout << "thread " << this_id << " sleeping...\n";
    std::cout << " my id is " << i << "\n";
    g_display_mutex.unlock();
 
    std::this_thread::sleep_for(std::chrono::seconds(1));
}
 
int main()
{
    std::thread t1(foo,1);
    std::thread t2(foo,2);
 
    t1.join();
    t2.join();
}
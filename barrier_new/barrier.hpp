#ifndef PUTILS_BARRIER_HPP
#define PUTILS_BARRIER_HPP
#include <cstdlib>
#include <atomic>
#include <cstdint>
#include <mutex>
#include <thread>
#include <condition_variable>


namespace putils {

struct spinlock_barrier {
    std::atomic<std::uint32_t> m_cnt;
    std::atomic<std::uint32_t> m_gen;
    std::uint32_t m_cnt_reset;
    
    spinlock_barrier(size_t nthreads):m_cnt(nthreads),m_gen(0),m_cnt_reset(nthreads) {};
    
    bool wait()
    {
        std::uint32_t gen = m_gen.load();
        
        if ( --m_cnt ) {
            while (m_gen==gen) std::this_thread::yield();
            return false;
        }else{
            m_cnt = m_cnt_reset;
            ++m_gen;
            return true;
        }
    }

    template < class Fn, class ... Args >
    void reduce(Fn f,Args&& ...args) {
        std::uint32_t gen = m_gen.load();
        
        if ( --m_cnt ) {
            while (m_gen==gen) std::this_thread::yield();
        }else{
            f(std::forward< Args >(args)...);
            m_cnt = m_cnt_reset;
            ++m_gen;
        }
    }
    
    void wait_no_release()
    {
        std::uint32_t gen = m_gen.load();
        
        if ( --m_cnt ) {
            while (m_gen==gen) std::this_thread::yield();
        }else{
            m_cnt = m_cnt_reset;
        }    
    }

    void release() {
        ++m_gen;
    }
};

struct mutex_barrier {
    uint32_t m_cnt;
    std::uint32_t m_gen;
    std::uint32_t m_cnt_reset;
    std::mutex m_mutex;
    std::condition_variable m_cond;
    
    mutex_barrier(size_t nthreads):m_cnt(nthreads),m_gen(0),m_cnt_reset(nthreads),m_mutex(),m_cond() {};
    
    bool wait()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        std::uint32_t gen = m_gen;
        
        if ( --m_cnt ) {
            while (m_gen==gen) m_cond.wait(lock);
            return false;
        }else{
            m_cnt = m_cnt_reset;
            ++m_gen;
            m_cond.notify_all();
            return true;
        }
    }

    template < class Fn, class ... Args >
    void reduce(Fn f,Args&& ...args) {
        std::unique_lock<std::mutex> lock(m_mutex);
        std::uint32_t gen = m_gen;

        if ( --m_cnt ) {
            while (m_gen==gen) m_cond.wait(lock);
        }else{
            f(std::forward<Args>(args)...);
            m_cnt = m_cnt_reset;
            ++m_gen;
            m_cond.notify_all();
        }
    }
    
};

}
#endif


#ifndef PUTILS_BARRIERS_HPP
#define PUTILS_BARRIERS_HPP
#include <thread>
#include <cstdlib>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <cstdint>
#include <vector>
namespace putils { 

struct atomic_barrier {
    public:
        typedef std::uint32_t uint32_t;

        explicit atomic_barrier(uint32_t tcnt):m_cnt(tcnt),m_gen(0),m_cnt_reset_value(tcnt) {};
        atomic_barrier()=delete;
        atomic_barrier(const atomic_barrier&)=delete;
        atomic_barrier& operator=(const atomic_barrier&)=delete;
        void wait()
        {
            uint32_t gen = m_gen.load();
            
            if (--m_cnt==0) {
                if ( m_gen.compare_exchange_weak(gen,gen+1)) m_cnt = m_cnt_reset_value;
                return;
            }
            while ((gen==m_gen) && (m_cnt!=0)) std::this_thread::yield();
        }
        template < class F, class ... Args >
        void wait_and_reduce(F f,Args&& ...args)
        {
            uint32_t gen = m_gen.load();
            
            if (--m_cnt==0) {
                f(std::forward<Args>(args)...);
                if ( m_gen.compare_exchange_weak(gen,(gen+1))) m_cnt = m_cnt_reset_value;
                return;
            }
            while ((gen==m_gen) && (m_cnt!=0)) std::this_thread::yield();        
        }
        void wait_and_reduce(void (*f)(void *),void *args)
        {
            uint32_t gen = m_gen.load();
            
            if (--m_cnt==0) {
                f(args);
                if ( m_gen.compare_exchange_weak(gen,(gen+1))) m_cnt = m_cnt_reset_value;
                return;
            }
            while ((gen==m_gen) && (m_cnt!=0)) std::this_thread::yield();        
        }
        void wait_no_release()
        {
            uint32_t gen = m_gen.load();
            
            if (--m_cnt==0) {
                return;
            }
            while ((gen==m_gen) && (m_cnt!=0)) std::this_thread::yield();
        }
        void release()
        {
            uint32_t gen = m_gen.load();
            if (m_cnt==0) {
                if ( m_gen.compare_exchange_weak(gen,(gen+1))) m_cnt = m_cnt_reset_value;                
            }
        }
    private:
        std::atomic<uint32_t> m_cnt;
        std::atomic<uint32_t> m_gen;
        uint32_t m_cnt_reset_value; 
};

struct cond_barrier {
    public:
        typedef std::uint32_t uint32_t;

        explicit cond_barrier(uint32_t tcnt):m_mutex(),m_cond(),m_cnt(tcnt),m_gen(0),m_cnt_reset_value(tcnt) {}
        cond_barrier()=delete;
        cond_barrier(const cond_barrier&)=delete;
        cond_barrier& operator=(const cond_barrier&)=delete;
        void wait()
        {
            std::unique_lock< std::mutex > t_mutex(m_mutex,std::defer_lock);            
            uint32_t gen = m_gen;

            t_mutex.lock();
            if (--m_cnt==0) {
                ++m_gen;        
                m_cnt = m_cnt_reset_value;
                m_cond.notify_all();
                t_mutex.unlock();
                return;
            }
            while (gen==m_gen) m_cond.wait(t_mutex);
            t_mutex.unlock();
        }
        template < class F, class ... Args >
        void wait_and_reduce(F f,Args&& ...args)
        {
            std::unique_lock< std::mutex > t_mutex(m_mutex,std::defer_lock);            
            uint32_t gen = m_gen;
            
            t_mutex.lock();
            if (--m_cnt==0) {
                f(std::forward<Args>(args)...);
                ++m_gen;        
                m_cnt = m_cnt_reset_value;
                m_cond.notify_all();
                t_mutex.unlock();
                return;
            }
            while (gen==m_gen) m_cond.wait(t_mutex);
            t_mutex.unlock();
        }
        void wait_and_reduce(void (*f)(void *),void *args)
        {
            std::unique_lock< std::mutex > t_mutex(m_mutex,std::defer_lock);            
            uint32_t gen = m_gen;
            
            t_mutex.lock();
            if (--m_cnt==0) {
                f(args);
                ++m_gen;        
                m_cnt = m_cnt_reset_value;
                m_cond.notify_all();
                t_mutex.unlock();
                return;
            }
            while (gen==m_gen) m_cond.wait(t_mutex);
            t_mutex.unlock();
        }        
        void wait_no_release()
        {
            std::unique_lock< std::mutex > t_mutex(m_mutex,std::defer_lock);            
            uint32_t gen = m_gen;
            t_mutex.lock();            
            if (--m_cnt==0) {
                return;
            }
            while (gen==m_gen) m_cond.wait(t_mutex);        
            t_mutex.unlock();
        }
        void release() {
            if (m_cnt==0) {
                ++m_gen;
                m_cnt = m_cnt_reset_value;
                m_cond.notify_all();
            }        
        }        
    private:
        std::mutex m_mutex;
        std::condition_variable m_cond;
        uint32_t m_cnt;
        uint32_t m_gen;
        uint32_t m_cnt_reset_value; 
};

struct pth_cond_barrier {
    public:
        typedef std::uint32_t uint32_t;

        explicit pth_cond_barrier(uint32_t tcnt):m_mutex(),m_cond(),
            m_cnt(new uint32_t(tcnt)),m_gen(0),m_cnt_reset_value(tcnt) {
            m_mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
            m_cond = (pthread_cond_t*)malloc(sizeof(pthread_cond_t));
            pthread_mutex_init(m_mutex,0x0);
            pthread_cond_init(m_cond,0x0);
        }
        pth_cond_barrier()=delete;
        pth_cond_barrier(const pth_cond_barrier&)=delete;
        pth_cond_barrier& operator=(const pth_cond_barrier&)=delete;
        
        ~pth_cond_barrier() {
            pthread_mutex_destroy(m_mutex);
            pthread_cond_destroy(m_cond);
            delete m_cnt;
            free(m_cond);
            free(m_mutex);
        }
        void wait()
        {
            uint32_t gen(m_gen);
            pthread_mutex_lock(m_mutex);
            std::cerr << "here "<< *m_cnt << "\n";
            if (--*m_cnt) {
                std::cerr << "there " << *m_cnt << "\n";
                pthread_cond_wait(m_cond,m_mutex);
                while (gen==m_gen) {};
                std::cerr << "gen changed " << gen << "\n";
            }else{
                std::cerr << "in last place\n";
                ++m_gen;
                *m_cnt = m_cnt_reset_value;
                pthread_cond_broadcast(m_cond);
            }
            std::cerr << "leaving\n";
            pthread_mutex_unlock(m_mutex);
            std::cerr << "left\n";
        }
        template < class F, class ... Args >
        void wait_and_reduce(F f,Args&& ...args)
        {
            uint32_t gen = m_gen;
            pthread_mutex_lock(m_mutex);
            if (--*m_cnt==0) {
                f(std::forward<Args>(args)...);
                ++m_gen;        
                *m_cnt = m_cnt_reset_value;
                pthread_cond_broadcast(m_cond);
                pthread_mutex_unlock(m_mutex);
                return;
            }
            while (gen==m_gen) pthread_cond_wait(m_cond,m_mutex);
            pthread_mutex_unlock(m_mutex);
        }
        template < class ... Args >
        void wait_and_reduce(void (*f)(void *),Args&& ...args)
        {
            uint32_t gen = m_gen;
            pthread_mutex_lock(m_mutex);
            if (--*m_cnt==0) {
                f(std::forward<Args>(args)...);
                ++m_gen;        
                *m_cnt = m_cnt_reset_value;
                pthread_cond_broadcast(m_cond);
                pthread_mutex_unlock(m_mutex);
                return;
            }
            while (gen==m_gen) pthread_cond_wait(m_cond,m_mutex);
            pthread_mutex_unlock(m_mutex);
       }        
        void wait_no_release()
        {
            uint32_t gen = m_gen;
            pthread_mutex_lock(m_mutex);
            if (--*m_cnt==0) {
                return;
            }
            while (gen==m_gen) pthread_cond_wait(m_cond,m_mutex);
            pthread_mutex_unlock(m_mutex);
        }
        void release() {
            uint32_t gen = m_gen;
            if (*m_cnt==0) {
                ++m_gen;        
                *m_cnt = m_cnt_reset_value;
                pthread_cond_broadcast(m_cond);
                pthread_mutex_unlock(m_mutex);
                return;
            }
        }        
    private:
        pthread_mutex_t *m_mutex;
        pthread_cond_t *m_cond;
        volatile uint32_t * m_cnt;
        uint32_t m_gen;
        uint32_t m_cnt_reset_value; 
};



template < class barrier_t >
class tour_barrier
{
public:    
    typedef std::uint32_t uint32_t;
    tour_barrier()=delete;
    tour_barrier(const tour_barrier&)=delete;
    explicit tour_barrier(uint32_t n_threads):m_cnt(n_threads),barrs() {
        init();
    }
    tour_barrier& operator=(const tour_barrier&)=delete;

    void wait(uint32_t thrd_id)
    {
        uint32_t level = 0;
        uint32_t id_offset = 0;
        uint32_t winner = 1;
        uint32_t level_id,level_sz;
        uint32_t block_size = 2;
        uint32_t offset = 0;
        while (block_size <= m_cnt) {
            level_sz = m_cnt / block_size;
            level_id = thrd_id / block_size;
            if (level_id >= level_sz) --level_id;
            level_id += id_offset;
            barrs[level_id].wait_no_release();
            if (!winner) {
                break;
            }  
            block_size <<=1;
            id_offset += level_sz;
        }
        block_size >>=1;
        level_sz = m_cnt/block_size;
        id_offset -= level_sz;
        while (block_size > 1) {
            level_id = thrd_id / block_size;
            if (level_id >=level_sz) --level_id;
            level_id += id_offset;
            barrs[level_id].release();
            block_size >>=1;
            level_sz = m_cnt/block_size;
            id_offset -= level_sz;
        }            
    }
    void wait_and_reduce(uint32_t thrd_id,void *f(void*),void *args)
    {
        uint32_t level = 0;
        uint32_t id_offset = 0;
        uint32_t winner = 1;
        uint32_t level_id,level_sz;
        uint32_t block_size = 2;
        uint32_t offset = 0;
        while (block_size <= m_cnt) {
            level_sz = m_cnt / block_size;
            level_id = thrd_id / block_size;
            if (level_id >= level_sz) --level_id;
            level_id += id_offset;
            barrs[level_id].wait_no_release();
            if (!winner) {
                break;
            }  
            block_size <<=1;
            id_offset += level_sz;
        }
        if (winner) f(args);
        block_size >>=1;
        level_sz = m_cnt/block_size;
        id_offset -= level_sz;
        while (block_size > 1) {
            level_id = thrd_id / block_size;
            if (level_id >=level_sz) --level_id;
            level_id += id_offset;
            barrs[level_id].release();
            block_size >>=1;
            level_sz = m_cnt/block_size;
            id_offset -= level_sz;
        }            
    }

    template < class F, class ... Args >
    void wait_and_reduce(uint32_t thrd_id,F f,Args&& ... args)
    {
        uint32_t level = 0;
        uint32_t id_offset = 0;
        uint32_t winner = 1;
        uint32_t level_id,level_sz;
        uint32_t block_size = 2;
        uint32_t offset = 0;
        while (block_size <= m_cnt) {
            level_sz = m_cnt / block_size;
            level_id = thrd_id / block_size;
            if (level_id >= level_sz) --level_id;
            level_id += id_offset;
            barrs[level_id].wait_no_release();
            if (!winner) {
                break;
            }  
            block_size <<=1;
            id_offset += level_sz;
        }
        if (winner) f(std::forward<Args>(args)...);
        block_size >>=1;
        level_sz = m_cnt/block_size;
        id_offset -= level_sz;
        while (block_size > 1) {
            level_id = thrd_id / block_size;
            if (level_id >=level_sz) --level_id;
            level_id += id_offset;
            barrs[level_id].release();
            block_size >>=1;
            level_sz = m_cnt/block_size;
            id_offset -= level_sz;
        }            
    }
    
    
    void init()
    {
        if (m_cnt < 4) {
            barrs.push_back(barrier_t(m_cnt));
        }
        uint32_t level_sz = 2;
        uint32_t n_levels = 0;
        uint32_t block_size = 2;
        uint32_t n_barrs=0;
        while (block_size <= m_cnt) {
            n_barrs += m_cnt / block_size;
            block_size <<=1;
            ++n_levels;
        }
        barrs.reserve(n_barrs);
        block_size = 2;
        uint32_t npt = m_cnt;
        for (uint32_t i = 0 ; i < n_levels; ++i) {
            level_sz = m_cnt / block_size;
            for (uint32_t k = 0 ; k < level_sz; ++k) {    
                uint32_t cnt = 2;
                if ( (npt%2) && k==(level_sz-1)) {
                    ++cnt;
                }
                barrs.push_back(barrier_t(cnt));
            }
            npt = level_sz;
            block_size <<=1;
        }                    
    }
private:
    uint32_t m_cnt;
    std::vector< barrier_t > barrs;    

};

}
#endif
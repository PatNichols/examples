#include <pthread.h>

typedef pthread_mutex_t mutex_t;
typedef pthread_cond_t cond_t;

inline void mutex_init(mutex_t *m) { return pthread_mutex_init(m,0x0);}
inline void mutex_destroy(mutex_t *m) { return pthread_mutex_destroy(m);}
inline void cond_init(cond_t *m) { return pthread_cond_init(m,0x0);}
inline void cond_destroy(cond_t *m) { return pthread_cond_destroy(m);}


typedef struct {
        int     maxcnt;                 /* maximum number of runners    */
        struct _sb {
                cond_t  wait_cv;        /* cv for waiters at barrier    */
                cond_t wait_lk;        /* cond for waiters at barrier */
                int     runners;        /* number of running threads    */
        } sb[2];
        struct _sb      *sbp;           /* current sub-barrier          */
} barrier_t;


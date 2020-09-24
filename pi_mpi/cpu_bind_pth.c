#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

typedef struct {
    int tid;
    int nth;
    int ncores;
} thrd_info_t;

static const int num_thrds_max = 32;

void check_cpu_bind_pth ( int num_thds, int num_cores, int thd_id )
{
    int k, j, e;
    cpu_set_t set;

    pthread_getaffinity_np ( pthread_self(), sizeof ( set ), &set );
    e = -1;
    for ( j = 0; j < num_cores; ++j ) {
        e = CPU_ISSET ( j, &set );
        if ( e ) {
            flockfile(stderr);
            fprintf ( stderr, "thread %d is bound to core %d\n", thd_id, j );
            funlockfile(stderr);
            break;
        }
    }
    if ( e == -1 ) {
        flockfile(stderr);
        fprintf ( stderr, "thread %d is not bound to any core!\n", thd_id );
        funlockfile(stderr);
    }
    return;
}


void cpu_bind_pth( int num_thds, int num_cores, int thd_id, int strat )
{
    cpu_set_t set;
    const size_t cpu_set_size = sizeof ( cpu_set_t );
    int kcore, k, st_core;
    pthread_t pth = pthread_self();
    if (thd_id==0) {
        flockfile(stderr);
        fprintf ( stderr, "# threads = %d  # cores = %d  strategy = %d\n", num_thds, num_cores, strat );
        funlockfile(stderr);
    }
    kcore=0xFFFF;
    switch ( strat ) {
    case 0:
        kcore = (thd_id % num_cores);
        break;
    case 1:
        st_core = num_cores - 1;
        kcore = num_cores - 1;
        for ( k = 0; k < thd_id; ++k ) {
            kcore -= 1;
            if ( kcore < 0 ) {
                kcore = st_core;
            }
        }
        break;
    case 2:
        st_core = num_cores - 2;
        kcore = num_cores - 1;
        for ( k = 0; k < thd_id; ++k ) {
            kcore -= 2;
            if ( kcore < 0 ) {
                kcore = st_core;
                st_core= (st_core==(num_cores-1))?(num_cores-2):(num_cores-1);  
            }
        }
        break;
    default:
        st_core = 1;
        kcore = 0;
        for ( k = 0; k < thd_id; ++k ) {
                kcore += 2;
                if ( kcore >= num_cores ) {
                    kcore = st_core;
                    st_core = ( st_core == 0 ) ? 1 : 0;
                }
        }
        break;
    }
    CPU_ZERO ( &set );
    CPU_SET ( kcore, &set );
    pthread_setaffinity_np ( pth, cpu_set_size, &set );
    return;
}

void * run1(void * arg)
{
    thrd_info_t * info = (thrd_info_t*)arg;
    
    cpu_bind_pth((info->nth),(info->ncores),(info->tid),0);
    check_cpu_bind_pth((info->nth),(info->ncores),(info->tid));
    return 0x0;
}

void * run2(void * arg)
{
    thrd_info_t * info = (thrd_info_t*)arg;
        
    cpu_bind_pth((info->nth),(info->ncores),(info->tid),1);
    check_cpu_bind_pth((info->nth),(info->ncores),(info->tid));
    return 0x0;
}

void * run3(void * arg)
{
    thrd_info_t * info = (thrd_info_t*)arg;
    
    cpu_bind_pth(info->nth,info->ncores,info->tid,2);
    check_cpu_bind_pth(info->nth,info->ncores,info->tid);
    return 0x0;
}

void * run4(void * arg)
{
    thrd_info_t * info = (thrd_info_t*)arg;
    
    cpu_bind_pth(info->nth,info->ncores,info->tid,3);
    check_cpu_bind_pth(info->nth,info->ncores,info->tid);
    return 0x0;
}

void launch(int nth)
{
    const int num_cores = 8;
    pthread_t *pth = ( pthread_t* ) malloc ( sizeof ( pthread_t ) * nth );
    thrd_info_t *info = (thrd_info_t *)malloc(sizeof(thrd_info_t)* nth);
    int k;
    long rx;
    long *rxp=&rx;
    void **res=(void**)&rxp;
 
    fprintf(stderr,"launch %d %d\n",nth,num_cores);      
    for (k=0;k<nth;++k) {
        (info+k)->tid=k;
        (info+k)->nth=nth;
        (info+k)->ncores=num_cores;
        fprintf(stderr,"%d %d %d\n",k,nth,num_cores);
    }
    for (k=0;k<nth;++k) {
        pthread_create((pth+k),0x0,&run1,(info+k));
    }
    for (k=0;k<nth;++k) {
        pthread_join(pth[k],res);
    }
    fprintf(stderr,"did it once!\n");
    for (k=0;k<nth;++k) {
        pthread_create((pth+k),0x0,run2,(info+k));
    }
    for (k=0;k<nth;++k) {
        pthread_join(pth[k],res);
    }
    for (k=0;k<nth;++k) {
        pthread_create((pth+k),0x0,run3,(info+k));
    }
    for (k=0;k<nth;++k) {
        pthread_join(pth[k],res);
    }
    for (k=0;k<nth;++k) {
        pthread_create((pth+k),0x0,run4,(info+k));
    }
    for (k=0;k<nth;++k) {
        pthread_join(pth[k],res);
    }
    free ( info );
    free ( pth );
}

int main()
{
    const int th_szs[] = { 2, 4, 5, 7, 8, 11, 12, 14, 16, -1};
    int k,nth;
    k=0;
    nth = th_szs[k];
    ++k;
    while ( nth != -1 ) {
        launch(nth);
        nth = th_szs[k];
        ++k;
    }
}

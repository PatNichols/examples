#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <mpi.h>

void check_cpu_bind_pid ( int num_thds, int num_cores, int thd_id )
{
    int k, j, e;
    cpu_set_t set;
    pid_t pid= getpid();
    sched_getaffinity( pid, sizeof ( set ), &set );
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


void cpu_bind_pid( int num_thds, int num_cores, int thd_id, int strat )
{
    cpu_set_t set;
    const size_t cpu_set_size = sizeof ( cpu_set_t );
    int kcore, k, st_core;
    pid_t pid = getpid();
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
    sched_setaffinity ( pid, cpu_set_size, &set );
    return;
}



int main(int argc,char **argv)
{
    int rank,nproc;
    const int ncores=8;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&nproc);    
    cpu_bind_pid(nproc,ncores,rank,0);
    check_cpu_bind_pid(nproc,ncores,rank);
    cpu_bind_pid(nproc,ncores,rank,1);
    check_cpu_bind_pid(nproc,ncores,rank);
    cpu_bind_pid(nproc,ncores,rank,2);
    check_cpu_bind_pid(nproc,ncores,rank);
    cpu_bind_pid(nproc,ncores,rank,3);
    check_cpu_bind_pid(nproc,ncores,rank);
    MPI_Finalize();
    return EXIT_SUCCESS;
}

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <omp.h>
#include <sched.h>
#include <unistd.h>
#include <dirent.h>

void exit_mpi(int err)
{
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    fprintf(stderr,"Error exiting on node %d\n",rank);
    MPI_Abort(MPI_COMM_WORLD,err);
}


static int
one (const struct dirent *unused)
{
  return 1;
}

void spinning_quarter(int rank)
{
  struct dirent **eps;
  int n;
  int in_container=0;
  const char * inc_str ="include";

  n = scandir ("/usr", &eps, one, alphasort);
  if (n >= 0)
    {
      int cnt;
      for (cnt = 0; cnt < n; ++cnt)
      {
/*        fprintf(stderr,"-- %s\n",eps[cnt]->d_name); */
	if (strcmp(eps[cnt]->d_name,inc_str)==0) 
	{
/*	    fprintf(stderr," found /usr/include \n"); */
            in_container=1;
	}
/*	else{
	    fprintf(stderr," not /usr/include\n");
	}	
*/
      }
    }
  else
    perror ("Couldn't open the /usr directory");

  if (in_container==1) {
	  fprintf(stderr,"in container\n");
  }else{	
	  fprintf(stderr,"not in container!!!!!\n");
  }
  return;
}


double calc_pi_openmp(
    long nint,
    int rank,
    int nproc)
{
    long i;
    double h,sum,x;
    h = 1.0/nint;
    sum = 0;
    #pragma omp parallel
    {
        int e = sched_getcpu();
//        int e = 0;
        int tid = omp_get_thread_num();
        fprintf(stdout,"thread %d of rank %d is on cpu %d  \n",tid,rank,e);
    }
    #pragma omp parallel for default(shared) private(i,x) firstprivate(h) reduction(+:sum)
    for (i=rank; i<nint; i+=nproc) {
        x = h * ((double)i + 0.5);
        sum += 4.0/(1.0+x*x);
    }
    return sum * h;
}


long parse_long(char *arg)
{
    char *end_ptr;
    long val;

    errno=0;
    val = strtol(arg,&end_ptr,10);

    if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
            || (errno != 0 && val == 0)) {
        perror("strtol");
        exit_mpi(EXIT_FAILURE);
    }
    if (end_ptr == arg) {
        fprintf(stderr, "No digits were found\n");
        exit_mpi(EXIT_FAILURE);
    }
    return val;
}

int main(int argc,char **argv)
{
    long nint,i;
    double x,h,dx,sum,pi,mypi;
    int rank,nproc;

    MPI_Init(&argc,&argv);
    if (argc!=2) {
        fprintf(stderr,"Usage is ./pimpi N where N is the number of intervals!\n");
        exit_mpi(EXIT_FAILURE);
    }
    MPI_Comm_size(MPI_COMM_WORLD,&nproc);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
//    spinning_quarter(rank);
//    fprintf(stderr,"argument is %s\n",argv[1]);
    nint=parse_long(argv[1]);
    if (nint < nproc) {
        fprintf(stderr,"error # of intervals %ld is less than the number of mpi processes %d\n",nint,nproc);
        exit_mpi(EXIT_FAILURE);
    }
    h = 1.0/(double)nint;
    dx = 1.0/(nproc-1);
    sum = 0;
    for (i=rank; i<nint; i+=nproc) {
        x = h * ((double)i+0.5);
        sum += 4.0/(1.0+x*x);
    }
    mypi = h * sum;
    MPI_Reduce(&mypi,&pi,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
    if (!rank) {
        fprintf(stderr,"pi is approx %20.16le, error is %20.16le\n",
                pi,(pi-M_PI));
    }

    mypi = calc_pi_openmp(nint,rank,nproc);
    MPI_Reduce(&mypi,&pi,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
    if (!rank) {
        fprintf(stderr,"pi is approx %20.16le, error is %20.16le\n",
                pi,(pi-M_PI));
    }

    {
        char xhost[100];
        gethostname(xhost,99);        
        int cpu = sched_getcpu();
        fprintf(stdout,"rank %d  affinity is %d host= %s\n",rank,cpu,xhost);
    }

    MPI_Finalize();
    return EXIT_SUCCESS;
}

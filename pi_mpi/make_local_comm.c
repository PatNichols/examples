#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mpi.h>
#include <unistd.h>

void createLocalComm(MPI_Comm *local_comm,MPI_Group *local_grp)
{
    size_t hostname_len =32;
    int rank,ntask,err,local_rank;
    char * my_host;
    char * host_i;
    char * host_array;
    int *local_list;
    int local_cnt,i;
    MPI_Group WORLD_Group;
        
    MPI_Comm_size(MPI_COMM_WORLD,&ntask);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    
    host_array = (char*) malloc ( hostname_len * ntask );
    
    if (!host_array) {
        fprintf(stderr,"could not allocate array of host names \n");
        exit(EXIT_FAILURE); 
    }
    
    local_list = (int*) malloc ( ntask * sizeof(int));
    
    if (!local_list) {
        fprintf(stderr,"could not allocate local_list!\n");
        exit(EXIT_FAILURE);
    }
    
    my_host = host_array + rank * hostname_len;
    
    memset(my_host,0,hostname_len);
    
    err=gethostname(my_host,hostname_len);
    if (err==-1) {
        fprintf(stderr,"could not get hostname\n");
        exit(EXIT_FAILURE);
    }

/*    fprintf(stderr,"My rank %d on host %s\n",rank,my_host);
 */
     
    MPI_Allreduce(MPI_IN_PLACE,host_array,hostname_len*ntask,MPI_BYTE,MPI_BOR,MPI_COMM_WORLD);
    
    local_cnt=0;
    for (i=0;i<ntask;++i) {
        host_i = host_array + i * hostname_len;
        if (strncmp(host_i,my_host,(hostname_len-1))==0) {
            local_list[local_cnt]=i;        
            local_cnt++;
        }
    }
    
    MPI_Comm_group(MPI_COMM_WORLD,&WORLD_Group);
    MPI_Group_incl(WORLD_Group,local_cnt,local_list,local_grp);
    MPI_Comm_create(MPI_COMM_WORLD,*local_grp,local_comm);
    
    MPI_Comm_rank(*local_comm,&local_rank);
    
    
    if (local_rank==0) {
        my_host = host_array + rank * hostname_len;
        fprintf(stderr,"I am %d of %d tasks my local rank is 0 on %s with local group = ",rank,ntask,my_host);
        for (i=0;i<local_cnt;++i) {
            fprintf(stderr," %d",local_list[i]);
        }
        fprintf(stderr,"\n");
    }
    
    free(local_list);
    free(host_array);
    return;
}


int main(int argc,char **argv)
{
    MPI_Comm local_comm;
    MPI_Group local_group;
    int nlocal,local_rank,rank,ntask;

    MPI_Init(&argc,&argv);
    
    createLocalComm(&local_comm,&local_group);
    
    MPI_Barrier(MPI_COMM_WORLD);
    
    MPI_Comm_size(MPI_COMM_WORLD,&ntask);
    
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    
    MPI_Comm_size(local_comm,&nlocal);
    
    MPI_Comm_rank(local_comm,&local_rank);  
    
    fprintf(stderr,"I am %d of %d tasks - local rank %d of %d local tasks\n",
        rank,ntask,local_rank,nlocal);
    
    MPI_Finalize();
    
    return 0;
}    
    
    
    
    
    
    

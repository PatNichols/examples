#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include "putils_barrier.h"

struct sum_red_info_t {
    double *psum;
    size_t nth;
};

struct sum_thr_t {
    size_t id;
    size_t nth; 
    size_t n;
    putils_barrier_t *barr;
    double *psum;
    double *arr;
    struct sum_red_info_t sinfo;
};

typedef struct sum_thr_t sum_thr_t;
typedef struct sum_red_info_t sum_red_info_t;

void sum_red(void *args)
{
    sum_red_info_t *sinfo = (sum_red_info_t*)args;
    size_t nth = sinfo->nth;
    double *psum = sinfo->psum;
    size_t i;    
    double sum = 0.;
    for (i=0;i<nth;++i) sum+=psum[i];
    psum[0]=sum;
}

void * sum_fun(void *args)
{
    sum_thr_t *sum_t = (void*)args;
    size_t id = sum_t->id;
    size_t nth = sum_t->nth;
    size_t n = sum_t->n;
    putils_barrier_t *b = sum_t->barr;
    double *arr = sum_t->arr;
    double *psum = sum_t->psum;
    size_t off,csz,xsz,i,my_sz,my_off;
    double sum = 0.;
    
    off = 0;
    csz = n/nth;
    xsz = n%nth; 
    for (i=0;i<=id;++i) {
       my_off = off;
       my_sz = csz;
       if (xsz) {
        my_sz+=1;
        --xsz;
       }
       off += my_sz;    
    }
    sum = 0.0;
    for (i=my_off;i<off;++i) {
        sum += arr[i];
    }
    psum[id]=sum;
    putils_barrier_reduce(b,&sum_red,(void*)&(sum_t->sinfo));
    sum = 0.0;
    for (i=my_off;i<off;++i) {
        sum += arr[i];
    }
    psum[id]=sum;
    putils_barrier_reduce(b,&sum_red,(void*)&(sum_t->sinfo));
    sum=0.0;
    for (i=my_off;i<off;++i) {
        sum += arr[i];
    }
    psum[id]=sum;
    putils_barrier_reduce(b,&sum_red,(void*)&(sum_t->sinfo));
}

double summ(size_t n,size_t nth)
{
    putils_barrier_t *b;
    double *arr;
    double *psum;
    double ans=0.0;
    sum_thr_t **sthr;
    pthread_t * pth;
    size_t i;
    double x_ans,x,tcomp,tinit;
    struct timespec tstart_init;
    struct timespec tfin_init;
    struct timespec tfin_comp;
                
    clock_gettime(CLOCK_MONOTONIC,&tstart_init);
    b=putils_barrier_init(nth);  
    srand(time(0));
    arr = (double*)malloc(sizeof(double)*n);
    x_ans = 0.0;
    for (i=0;i<n;++i) {
        x = ((double)rand())/RAND_MAX;
        arr[i] = x+x-1.0;
        x_ans += arr[i];
    }
    psum = (double*)malloc(sizeof(double)*n);
    pth = (pthread_t*)malloc(sizeof(pthread_t)*nth);
    sthr = (sum_thr_t**)malloc(sizeof(sum_thr_t*)*nth);
    for (i=0;i<nth;++i) {
        psum[i]=0.0;
        sthr[i] = (sum_thr_t*)malloc(sizeof(sum_thr_t));
        sthr[i]->id = i;
        sthr[i]->nth = nth;
        sthr[i]->barr = b;
        sthr[i]->psum = psum;
        sthr[i]->arr = arr;
        sthr[i]->n  = n;
        sthr[i]->sinfo.psum = psum;
        sthr[i]->sinfo.nth = nth;
    }
    clock_gettime(CLOCK_MONOTONIC,&tfin_init);    
    for (i=0;i<nth;++i) {
        pthread_create(pth+i,0x0,&sum_fun,(void*)sthr[i]);
    }
    for (i=0;i<nth;++i) {
        pthread_join(pth[i],(void**)0x0);
    }
    ans = psum[0];
    clock_gettime(CLOCK_MONOTONIC,&tfin_comp);
    free(sthr);
    free(pth);
    free(psum);
    free(arr);
    putils_barrier_free(b);
    tinit = (tfin_init.tv_sec-tstart_init.tv_sec) + (tfin_init.tv_nsec - tstart_init.tv_nsec)*1.e-9;
    tcomp = (tfin_comp.tv_sec-tfin_init.tv_sec) + (tfin_comp.tv_nsec - tfin_init.tv_nsec)*1.e-9;
    fprintf(stderr,"n = %12u nth = %2u tinit = %6.3le tcomp = %6.3le ans = %12.3le x_ans = %12.3le diff = %12.3le\n",
        n,nth,tinit,tcomp,ans,x_ans,(ans-x_ans));
    return tcomp;
}    
    
int main(int argc,char **argv)
{
    size_t sz = (size_t)1e4;
    size_t nt = 1;
    size_t end_sz = (size_t)1.e10;
    size_t end_nt = 8;
    
    while (sz<=end_sz) {
        while (nt<=end_nt) {
            summ(sz,nt);
            nt = nt + nt;
        }
        nt = 1;
        sz *= 100;
    }
}    
    




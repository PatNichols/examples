#ifndef PUTILS_C_BARRIER_H
#define PUTILS_C_BARRIER_H

#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <stddef.h>

typedef struct {
    pthread_mutex_t *mutex;
    pthread_cond_t *cond;
    volatile int *cnt;
    int reset_cnt;
    volatile int *m_gen;
} putils_barrier_t;

typedef struct {
    putils_barrier_t **barr;
    size_t nbarr;
    size_t nth;
} putils_tour_barrier_t;

putils_tour_barrier_t *  putils_tour_barrier_init(int nthr);
void putils_tour_barrier_free(putils_tour_barrier_t *tb);
void putils_tour_barrier_wait(putils_tour_barrier_t *tb, size_t tid);
void putils_tour_barrier_reduce(putils_tour_barrier_t *tb, size_t tid, void (*f)(void *),void *args);
putils_barrier_t *putils_barrier_init(int nthread_);
void putils_barrier_free(putils_barrier_t *b);
int putils_barrier_wait(putils_barrier_t *b);
int putils_barrier_reduce(putils_barrier_t *b, void (*f)(void *),void *args);
int putils_barrier_wait_no_release(putils_barrier_t *b);
int putils_barrier_release(putils_barrier_t *b);

#endif
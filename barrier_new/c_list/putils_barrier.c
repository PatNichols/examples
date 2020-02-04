
#include "putils_barrier.h"

putils_tour_barrier_t *  putils_tour_barrier_init(int nthr) {
    putils_tour_barrier_t *t;
    size_t nbarr=0;
    size_t blk_sz=2;
    size_t lev_sz,npt,k,off;
    
    if (nthr < 4) {
        nbarr= 1;
        t = (putils_tour_barrier_t*)malloc(sizeof(putils_tour_barrier_t));
        t -> barr = (putils_barrier_t**)malloc(sizeof(putils_barrier_t));
        t -> nbarr= 1;
        t -> nth = nthr;
        *(t->barr) = putils_barrier_init(nthr);
        return t;
    } 
    while (blk_sz <= nthr) {
        lev_sz = nthr/blk_sz;
        nbarr+= lev_sz;
    }
    t = (putils_tour_barrier_t*)malloc(sizeof(putils_tour_barrier_t));
    t->barr = (putils_barrier_t**)malloc(sizeof(putils_barrier_t*)*nbarr);
    t->nbarr = nbarr;
    t->nth = nthr;
    blk_sz = 2;
    off = 0;
    npt = nthr;
    while (blk_sz <= nthr) {
        lev_sz = nthr/blk_sz;
        for (k=0;k<(lev_sz-1);++k) {
            t->barr[k+off] = putils_barrier_init(2);
        }
        k = lev_sz - 1;    
        if (npt%2) {
            t->barr[k+off] = putils_barrier_init(3);
        }else{
            t->barr[k+off] = putils_barrier_init(2);
        }
        npt = lev_sz;
        off += lev_sz;
        blk_sz <<=1;
    }
    return t;
};

void putils_tour_barrier_free(putils_tour_barrier_t *tb)
{
    size_t k;
    for (k=0;k<(tb->nbarr);++k) {
        putils_barrier_free(tb->barr[k]);
    }
    free(tb->barr);
    free(tb);
}


void putils_tour_barrier_wait(putils_tour_barrier_t *tb, size_t tid)
{
    putils_barrier_t **barr = tb->barr;
    size_t lev_sz = tb->nth;
    size_t nth = lev_sz;
    size_t lev_id;
    size_t blk_sz = 2;
    size_t off= 0;
    size_t winner = 1;
    while (blk_sz <= nth) {
        lev_sz = nth / blk_sz;
        lev_id = tid / blk_sz;
        if (lev_id >= lev_sz) --lev_id;
        winner = putils_barrier_wait_no_release(barr[off+lev_id]);
        if (!winner) break;
        blk_sz <<=1;
        off += lev_sz;
    }
    if (winner) f(args);
    blk_sz >>=1;
    lev_sz = nth/blk_sz;
    off -= lev_sz;
    while (blk_sz > 1) {
        lev_id = tid/blk_sz;
        if (lev_id <= lev_sz) --lev_id;
        putils_barrier_release(barr[off+lev_id]);
        blk_sz >>=1;
        lev_sz = nth/blk_sz;
        off -= lev_sz;
    }
}

void putils_tour_barrier_reduce(putils_tour_barrier_t *tb, size_t tid, void (*f)(void *),void *args)
{
    putils_barrier_t **barr = tb->barr;
    size_t lev_sz = tb->nth;
    size_t nth = lev_sz;
    size_t lev_id;
    size_t blk_sz = 2;
    size_t off= 0;
    size_t winner = 1;
    while (blk_sz <= nth) {
        lev_sz = nth / blk_sz;
        lev_id = tid / blk_sz;
        if (lev_id >= lev_sz) --lev_id;
        winner = putils_barrier_wait_no_release(barr[off+lev_id]);
        if (!winner) break;
        blk_sz <<=1;
        off += lev_sz;
    }
    if (winner) f(args);
    blk_sz >>=1;
    lev_sz = nth/blk_sz;
    off -= lev_sz;
    while (blk_sz > 1) {
        lev_id = tid/blk_sz;
        if (lev_id <= lev_sz) --lev_id;
        putils_barrier_release(barr[off+lev_id]);
        blk_sz >>=1;
        lev_sz = nth/blk_sz;
        off -= lev_sz;
    }
}

putils_barrier_t *putils_barrier_init(int nthread_)
{
    putils_barrier_t * barr = (putils_barrier_t*)malloc(sizeof(putils_barrier_t));
    barr->mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
    barr->cond = (pthread_cond_t *)malloc(sizeof(pthread_cond_t));
    barr->cnt = (int32_t*)malloc(sizeof(int32_t));
    barr->m_gen = (int32_t*)malloc(sizeof(int32_t));
    *(barr->cnt) = nthread_;
    barr->reset_cnt = nthread_;
    *(barr->m_gen) =0;
    pthread_mutex_init(barr->mutex,0x0);
    pthread_cond_init(barr->cond,0x0);
    return barr;
}

void putils_barrier_free(putils_barrier_t *b)
{
    pthread_cond_destroy(b->cond);
    pthread_mutex_destroy(b->mutex);
    free(b->m_gen);
    free(b->cnt);
    free(b->cond);
    free(b->mutex);
    free(b);
}

int putils_barrier_wait(putils_barrier_t *b)
{
    int32_t gen;
    pthread_mutex_lock(b->mutex);
    gen = *(b->m_gen);
    if (--*(b->cnt)) {
        while (gen == *(b->m_gen))
            pthread_cond_wait(b->cond,b->mutex);
        pthread_mutex_unlock(b->mutex);     
    }else{
        *(b->m_gen) = (gen + 1 ) % 100;
        *(b->cnt) = (b->reset_cnt);
        pthread_cond_broadcast(b->cond);
        pthread_mutex_unlock(b->mutex);
        return 1;
    }
    return 0;
}

int putils_barrier_reduce(putils_barrier_t *b, void (*f)(void *),void *args)
{
    int32_t gen,r;
    gen = *(b->m_gen);
    pthread_mutex_lock(b->mutex);
    if (--*(b->cnt)) {
        while (gen == *(b->m_gen))
            pthread_cond_wait(b->cond,b->mutex);
        pthread_mutex_unlock(b->mutex);     
    }else{
        f(args);
        *(b->m_gen) = (gen + 1 ) % 100;
        *(b->cnt) = (b->reset_cnt);
        pthread_mutex_unlock(b->mutex);
        pthread_cond_broadcast(b->cond);
        return 1;
    }
    return 0;
}

int putils_barrier_wait_no_release(putils_barrier_t *b)
{
    int32_t gen,r;
    pthread_mutex_lock(b->mutex);
    gen = *(b->m_gen);
    if (--*(b->cnt)) {
        while (gen == *(b->m_gen))
            pthread_cond_wait(b->cond,b->mutex);
        pthread_mutex_unlock(b->mutex);     
        return 0;
    }else{
        *(b->cnt) = (b->reset_cnt);
        return 1;
    }
}

int putils_barrier_release(putils_barrier_t *b)
{
    *(b->m_gen) = (*b->m_gen + 1 ) % 100;
    pthread_mutex_unlock(b->mutex);
    pthread_cond_broadcast(b->cond);
    return 0;
}

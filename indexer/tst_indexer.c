#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#define COL_MAJOR 0
#define ROW_MAJOR 1

struct indexer {
    int64_t strs[7];
    int64_t dims[7]; 
    int64_t ndim;
    int major;
};

typedef struct indexer indexer;

int64_t indicesFromFlatIndex_col_major(int64_t *indices,const int64_t flat_index,indexer *ixer)
{
    int64_t i;
    int64_t f = flat_index;
    int64_t *dims = ixer->dims;
    int64_t ndim = ixer->ndim;
    int64_t ix;
        
    for (i=0;i<ndim;++i) {
        ix = f % dims[i];
        indices[i] = ix;
        f = (f - ix)/dims[i];
    }
    return f;
}

int64_t indicesToFlatIndex_col_major(int64_t *indices,indexer *ixer)
{
    int64_t i,f;
    int64_t *strs = ixer->strs;
    int64_t *dims = ixer->dims;
    int64_t ndim = ixer->ndim;
    int maj = ixer->major;
    int64_t ix,st;

    f = indices[0];
    st = dims[0];
    for (i=1;i<ndim;++i) {
        f += st * indices[i];
        st *= dims[i];
    }
    return f;
}

int64_t indicesFromFlatIndex_row_major(int64_t *indices,const int64_t flat_index,indexer *ixer)
{
    int64_t f = flat_index;
    int64_t *strs = ixer->strs;
    int64_t *dims = ixer->dims;
    int64_t ndim = ixer->ndim;
    int maj = ixer->major;
    int64_t ix,i,j;
        
    j = ndim - 1;
    for (i=0;i<ndim;++i,--j) {
        ix = f % dims[j];
        indices[j] = ix;
        f = (f - ix)/dims[j];
    }
    return f;
}

int64_t indicesToFlatIndex_row_major(int64_t *indices,indexer *ixer)
{
    int64_t *dims = ixer->dims;
    int64_t ndim = ixer->ndim;
    int64_t st,f,i,j;

    j = ndim-1;
    f = indices[j];
    st = dims[j];
    for (i=1;i<ndim;++i) {
        --j;
        if (j<0) {
            fprintf(stderr,"XXXX ERR i  = %lld j =  %lld\n",i,j);
        }
        f += st * indices[j];
        st *= dims[j];
    }
    return f;
}

void test_cmaj3(indexer *ixer)
{
    int64_t i,j,k,l,f;
    int64_t ndim = ixer->ndim;
    int64_t *dims = ixer->dims;
    int64_t ind[7];
    int64_t cind[7];
    int64_t cnt;
    fprintf(stderr,"ndim = %lld\n",ndim);
    fprintf(stderr,"dims = (");
    for (i=0;i<ndim;++i) fprintf(stderr," %lld,",ixer->dims[i]);
    fprintf(stderr,")\n");
    cnt = 0;
    for (l=0;l<dims[2];++l) {
    for (j=0;j<dims[1];++j) {
    for (k=0;k<dims[0];++k) {
        ind[0]=k;
        ind[1]=j;
        ind[2]=l;
        fprintf(stderr,"indices = (");
        for (i=0;i<ndim;++i) fprintf(stderr," %lld,",ind[i]);
        fprintf(stderr," flat ");
        f = indicesToFlatIndex_col_major(ind,ixer);    
        fprintf(stderr,"= %lld\n",f);
        if (f!=cnt) {
            fprintf(stderr,"error in flat index!\n");
        }
        ++cnt;
        indicesFromFlatIndex_col_major(cind,f,ixer);    
        fprintf(stderr,"indices = (");
        for (i=0;i<ndim;++i) fprintf(stderr," %lld,",cind[i]);
        for (i=0;i<ndim;++i) {
            if ((cind[i]-ind[i])!=0) {
                fprintf(stderr,"ERROR!!! %lld %lld",cind[i],ind[i]);
            }
        }
        fprintf(stderr,")\n\n");    
    }
    }
    }       
}


void test_rmaj3(indexer *ixer)
{
    int64_t i,j,k,l,f;
    int64_t ndim = ixer->ndim;
    int64_t *dims = ixer->dims;
    int64_t ind[7];
    int64_t cind[7];
    int64_t cnt;
    fprintf(stderr,"ndim = %lld\n",ndim);
    fprintf(stderr,"dims = (");
    for (i=0;i<ndim;++i) fprintf(stderr," %lld,",ixer->dims[i]);
    fprintf(stderr,")\n");
    cnt = 0;
    for (l=0;l<dims[0];++l) {
    for (j=0;j<dims[1];++j) {
    for (k=0;k<dims[2];++k) {
        ind[0]=l;
        ind[1]=j;
        ind[2]=k;
        fprintf(stderr,"indices = (");
        for (i=0;i<ndim;++i) fprintf(stderr," %lld,",ind[i]);
        fprintf(stderr," flat ");
        f = indicesToFlatIndex_row_major(ind,ixer);    
        fprintf(stderr,"= %lld\n",f);
        if (f!=cnt) {
            fprintf(stderr,"error in flat index!\n");
        }
        ++cnt;
        indicesFromFlatIndex_row_major(cind,f,ixer);    
        fprintf(stderr,"indices = (");
        for (i=0;i<ndim;++i) fprintf(stderr," %lld,",cind[i]);
        for (i=0;i<ndim;++i) {
            if ((cind[i]-ind[i])!=0) {
                fprintf(stderr,"ERROR!!! %lld %lld",cind[i],ind[i]);
            }
        }
        fprintf(stderr,")\n\n");    
    }
    }
    }       
}

void test_cmaj4(indexer *ixer)
{
    int64_t i,j,k,l,m,f;
    int64_t ndim = ixer->ndim;
    int64_t *dims = ixer->dims;
    int64_t ind[7];
    int64_t cind[7];
    int64_t cnt;
    fprintf(stderr,"ndim = %lld\n",ndim);
    fprintf(stderr,"dims = (");
    for (i=0;i<ndim;++i) fprintf(stderr," %lld,",ixer->dims[i]);
    fprintf(stderr,")\n");
    cnt = 0;
    for (m=0;m<dims[3];++m) {
    for (l=0;l<dims[2];++l) {
    for (j=0;j<dims[1];++j) {
    for (k=0;k<dims[0];++k) {
        ind[0]=k;
        ind[1]=j;
        ind[2]=l;
        ind[3]=m;
        fprintf(stderr,"indices = (");
        for (i=0;i<ndim;++i) fprintf(stderr," %lld,",ind[i]);
        fprintf(stderr," flat ");
        f = indicesToFlatIndex_col_major(ind,ixer);    
        fprintf(stderr,"= %lld\n",f);
        if (f!=cnt) {
            fprintf(stderr,"error in flat index!\n");
        }
        ++cnt;
        indicesFromFlatIndex_col_major(cind,f,ixer);    
        fprintf(stderr,"indices = (");
        for (i=0;i<ndim;++i) fprintf(stderr," %lld,",cind[i]);
        for (i=0;i<ndim;++i) {
            if ((cind[i]-ind[i])!=0) {
                fprintf(stderr,"ERROR!!! %lld %lld",cind[i],ind[i]);
            }
        }
        fprintf(stderr,")\n\n");    
    }
    }
    }       
    }
}


void test_rmaj4(indexer *ixer)
{
    int64_t i,j,k,l,m,f;
    int64_t ndim = ixer->ndim;
    int64_t *dims = ixer->dims;
    int64_t ind[7];
    int64_t cind[7];
    int64_t cnt;
    fprintf(stderr,"ndim = %lld\n",ndim);
    fprintf(stderr,"dims = (");
    for (i=0;i<ndim;++i) fprintf(stderr," %lld,",ixer->dims[i]);
    fprintf(stderr,")\n");
    cnt = 0;
    for (l=0;l<dims[0];++l) {
    for (j=0;j<dims[1];++j) {
    for (k=0;k<dims[2];++k) {
    for (m=0;m<dims[3];++m) {
        ind[0]=l;
        ind[1]=j;
        ind[2]=k;
        ind[3]=m;
        fprintf(stderr,"indices = (");
        for (i=0;i<ndim;++i) fprintf(stderr," %lld,",ind[i]);
        fprintf(stderr," flat ");
        f = indicesToFlatIndex_row_major(ind,ixer);    
        fprintf(stderr,"= %lld\n",f);
        if (f!=cnt) {
            fprintf(stderr,"error in flat index!\n");
        }
        ++cnt;
        indicesFromFlatIndex_row_major(cind,f,ixer);    
        fprintf(stderr,"indices = (");
        for (i=0;i<ndim;++i) fprintf(stderr," %lld,",cind[i]);
        for (i=0;i<ndim;++i) {
            if ((cind[i]-ind[i])!=0) {
                fprintf(stderr,"ERROR!!! %lld %lld",cind[i],ind[i]);
            }
        }
        fprintf(stderr,")\n\n");    
    }
    }
    }       
    }
}




int main()
{
    int64_t findex;    
    indexer ix;
    ix.dims[0]=7;
    ix.dims[1]=2;
    ix.dims[2]=3;
    ix.dims[3]=5;
    ix.ndim = 3;
    test_cmaj3(&ix);
    test_rmaj3(&ix);
    ix.ndim = 4;
    test_cmaj4(&ix);
    test_rmaj4(&ix);
}    

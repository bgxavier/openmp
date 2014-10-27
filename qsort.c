#include <stdio.h>
#include <stdlib.h>

#define RANDOM_INTERVAL 100

int partition(int * a, int p, int r)
{
    int lt[r-p];
    int gt[r-p];
    int i;
    int j;
    int key = a[r];
    int lt_n = 0;
    int gt_n = 0;

#pragma omp parallel for
    for(i = p; i < r; i++){
        if(a[i] < a[r]){
            lt[lt_n++] = a[i];
        }else{
            gt[gt_n++] = a[i];
        }   
    }   

    for(i = 0; i < lt_n; i++){
        a[p + i] = lt[i];
    }   

    a[p + lt_n] = key;

    for(j = 0; j < gt_n; j++){
        a[p + lt_n + j + 1] = gt[j];
    }   

    return p + lt_n;
}

void quicksort(int * a, int p, int r)
{
    int div;

    if(p < r){ 
        div = partition(a, p, r); 
#pragma omp parallel sections
        {   
#pragma omp section
            quicksort(a, p, div - 1); 
#pragma omp section
            quicksort(a, div + 1, r); 

        }
    }
}

int main(void)
{
   int vector_size=200000;
   int *vector = (int *) malloc(sizeof(int)*vector_size);
   int i;

   srand(time(0));

   for(i=0; i<vector_size; i++){
        vector[i] = rand() % RANDOM_INTERVAL;
   }

    quicksort(vector, 0, vector_size);

    return 0;
}

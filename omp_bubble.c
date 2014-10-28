#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#define RANDOM_INTERVAL 100000
 
void bubble_sort(int *list, int n);
 
 
int main(int argc,char **argv) {
 
        int vector_size = atoi(argv[1]);
        int num_vectors = 2000;
        int i, j;
        int **vectors_bag = (int **)malloc(num_vectors * sizeof(int *));
 
        for(i=0;i<num_vectors;i++){
                vectors_bag[i] = (int*)malloc(vector_size * sizeof(int));
        }
 
        /* Popula Vetores com numeros aleatorios */
        srand(time(0));
        for(i=0; i<num_vectors; i++){
                for(j=0 ; j<vector_size; j++){
                        vectors_bag[i][j] = rand() % RANDOM_INTERVAL;
                }
        }
 
        #pragma omp parallel for schedule(static)
        for(i=0; i < num_vectors; i++){
                bubble_sort(vectors_bag[i], vector_size);
        }
 
}
 
void bubble_sort(int *list, int n)
{
        long c,d,t;
        for(c=0; c<(n-1); c++){
                for(d=0; d<n-c-1; d++){
                        if (list[d] > list[d+1]){
                                t = list[d];
                                list[d] = list[d+1];
                                list[d+1] = t;
                        }
                }
        }
}

#include <omp.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define RANDOM_INTERVAL 1000000

void bubble_sort(int *list, int n);
int cmpfunc(const void * a, const void * b);

int main(int argc, char **argv) {

        int vector_size = atoi(argv[1]);
        int num_vectors = atoi(argv[2]);
        int i, j;
        int **vectors_bag = (int **)malloc(num_vectors * sizeof(int *));
        double exec_time;
        omp_get_max_threads();


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

        exec_time = omp_get_wtime ( );

#pragma omp parallel for schedule(static,1)
        for(i=0; i < num_vectors; i++){
            if(i % 2 == 0)
                bubble_sort(vectors_bag[i], vector_size);
            else
                qsort(vectors_bag[i], vector_size, sizeof(int), cmpfunc);
        }
        exec_time = omp_get_wtime () - exec_time;
        printf ("tempo de execucao: %12f\n", exec_time);
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

/* Funcao comparativa para o qsort */
int cmpfunc (const void * a, const void * b)
{
   return ( *(int*)a - *(int*)b );
}

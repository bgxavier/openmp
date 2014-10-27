#include <omp.h>
#include <stdio.h>
#include <stdlib.h> 

#define RANDOM_INTERVAL 100
#define PRINT 1

void OddEvenSort(int *A, int N);

int main(int argc,char **argv) {
 
   int vector_size = atoi(argv[1]);
   int *vector = (int *) malloc(sizeof(int)*vector_size);
   int i;
   srand(time(0));

   for(i=0; i<vector_size; i++){
        vector[i] = rand() % RANDOM_INTERVAL;
   }

   OddEvenSort( vector, vector_size);

   if(PRINT){
   	for(i=0;i<vector_size;i++)
		printf("%d ", vector[i]);
   }
}

void OddEvenSort( int *A, int N) {
	int exch = 1;
	int start = 0; 
	int i; 
	int temp;
	while (exch || start) { 
		exch = 0;
		#pragma omp parallel for private(temp) 
		for (i = start; i < N-1; i += 2) {
			if (A[i]>A[i+1]){
				temp = A[i]; 
				A[i] = A[i+1]; 
				A[i+1] = temp; 
				exch = 1;
			}
		 }
	if (start == 0) start = 1; else start = 0;
	}
}

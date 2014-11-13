#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#define N 50000000             
#define PI 3.1415926535
#define DELTA .01415926535
 
int main (int argc, char *argv[])
{
int nthreads, tid, i;

float *a = (float*) malloc(N * sizeof(float));
float *b = (float*) malloc(N * sizeof(float));

//float a[N], b[N];                   /* provavelmente alocação de a e b deve ser dinâmica – devido a N */
omp_lock_t locka, lockb;
 
omp_init_lock(&locka);
omp_init_lock(&lockb);
 
#pragma omp parallel shared(a, b, nthreads, locka, lockb) private(tid)
  {
  tid = omp_get_thread_num();
  #pragma omp master
    {
    nthreads = omp_get_num_threads();
    printf("Number of threads = %d\n", nthreads);
    }
  printf("Thread %d starting...\n", tid);
  #pragma omp barrier
 
  #pragma omp sections nowait
    {
    #pragma omp section
      {

      printf("Thread %d initializing a[]\n",tid);
      omp_set_lock(&locka);
      for (i=0; i<N; i++){
        a[i] = i * DELTA;
	printf("%f\n",a[i]);
      }
      omp_unset_lock(&locka);

      omp_set_lock(&lockb);
      printf("Thread %d adding a[] to b[]\n",tid);
      for (i=0; i<N; i++){
        b[i] += a[i];
      	printf("%f\n", b[i]);
      }
      omp_unset_lock(&lockb);
//      omp_unset_lock(&locka);

      }
    #pragma omp section
      {
      printf("Thread %d initializing b[]\n",tid);
      omp_set_lock(&lockb);
      for (i=0; i<N; i++)
        b[i] = i * PI;
      omp_set_lock(&locka);
      printf("Thread %d adding b[] to a[]\n",tid);
      for (i=0; i<N; i++)
        a[i] += b[i];
      omp_unset_lock(&locka);
      omp_unset_lock(&lockb);
      }
    } 
  }  }

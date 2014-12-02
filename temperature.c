#include <stdio.h>
#include <omp.h>
#include <mpi.h>
#include <stdlib.h>
#include <string.h>

#define YEAR_MIN 1920
#define YEAR_MAX 1930
#define DATASET_DIR "dataset/files/all/"
#define KILLTAG 9999

void master();
void slave();

int main(int argc, char  *argv[])
{
    int i;
    int rank, nprocs;

    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    if(rank == 0)
        master();
    else
        slave();

    MPI_Finalize();
    return 0;
}

void master()
{
    int i,j;
    double t1,t2;
    int nprocs,rank;
    int next_year;
    int *years_bag = (int *) malloc(10 * sizeof(int));

    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    t1 = MPI_Wtime();

    j=0;
    for(i=YEAR_MIN; i<= YEAR_MAX;i++){
        years_bag[j] = i;
        j++;
    }

    for(rank=1; rank < nprocs; rank++){
       next_year = rank-1;
       MPI_Send(&years_bag[next_year], 1, MPI_INT,rank,1,MPI_COMM_WORLD);
    }

    /* Envia a tag de kill para que os slaves parem o processamento */
    for(rank = 1;rank<nprocs;rank++)
        MPI_Send(0,0,MPI_INT,rank,KILLTAG,MPI_COMM_WORLD);

}

void slave()
{
    char buffer[134];
    char substring[5];
    char path[44];
    int year;
    char year_str[5];
    double temperature = -1111;
    double temp;

    int nprocs,rank;
    MPI_Status status;

    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Recebe anos para processamento
    while(1){

        MPI_Recv(&year, 1, MPI_INT, 0, MPI_ANY_TAG,MPI_COMM_WORLD, &status);

        if (status.MPI_TAG == KILLTAG) {
            break;
        }

        printf("rank %d processando ano %d\n", rank, year);

        sprintf(year_str, "%d", year);
        strcpy(path,DATASET_DIR);
        strcat(path, year_str);

        FILE *fp;
        fp=fopen(path, "r");

        while (fgets(buffer, sizeof(buffer), fp)!=NULL){
            strncpy(substring,buffer+87,5);
            temp = atoi(substring);
            if(temp != 9999 && temp > temperature)
                temperature = temp;
        }

        fclose(fp);
        printf("%.1f\n", temperature/10);
    }

}

#include <stdio.h>
#include <omp.h>
#include <mpi.h>
#include <stdlib.h>
#include <string.h>

#define YEAR_MIN 1901
#define YEAR_MAX 1905
#define DATASET_DIR "dataset/files/all/"
#define KILLTAG 9999
#define DONE_TAG 2

void master();
void slave();

char* chop(char *string)
{
    int i, len;
    len = strlen(string);
    char *newstring;

    newstring = (char *)malloc(len-1);

    for(i = 0; i < strlen(string)-1; i++){
        newstring[i] = string[i];
    }

    return newstring;
}

int main(int argc, char  *argv[])
{
    int i;
    int rank, nprocs;

    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    if(rank == 0){
        master();
    }
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
    int finish;
    int years_total = YEAR_MAX - YEAR_MIN + 1;
    int years_done = 0;
    int *years_bag = (int *) malloc(years_total * sizeof(int));

    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    t1 = MPI_Wtime();

    j=0;
    for(i=YEAR_MIN; i<= YEAR_MAX;i++){
        years_bag[j] = i;
        j++;
    }

    // Envio inicial
    for(rank=1; rank < nprocs; rank++){
       next_year = rank-1;
       MPI_Send(&years_bag[next_year], 1, MPI_INT,rank,1,MPI_COMM_WORLD);
    }

    // Envia os próximos anos para processamento para os escravos liberados
    while(years_done < years_total){

        // verificar o próximo escravo liberado
        MPI_Probe(MPI_ANY_SOURCE, DONE_TAG, MPI_COMM_WORLD, &status);

        years_done++;
        next_year++;

        if(next_year < years_total)
            MPI_Send(&years_bag[next_year], 1, MPI_INT,status.MPI_SOURCE,1,MPI_COMM_WORLD);
    }

    /* Envia a tag de kill para que os slaves parem o processamento */
    for(rank = 1;rank<nprocs;rank++)
        MPI_Send(0,0,MPI_INT,rank,KILLTAG,MPI_COMM_WORLD);

}

void slave()
{
    char buffer[400];
    char substring[5];
    char path[44];
    int year;
    int finish;
    int special;
    int line_counter=0;
    char year_str[5];
    double temperature = -1111;
    double temp;
    int i,l,k;
    FILE *fp;

    int nprocs,rank;
    MPI_Status status;

    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Recebe ano para processamento
    while(1){

        MPI_Recv(&year, 1, MPI_INT, 0, MPI_ANY_TAG,MPI_COMM_WORLD, &status);

        if (status.MPI_TAG == KILLTAG) {
            break;
        }

        // Apenda o ano no path
        sprintf(year_str, "%d", year);
        strcpy(path,DATASET_DIR);
        strcat(path, year_str);

        // Computa numero de linhas no arquivo
        fp=fopen(path, "r");
        while (fgets(buffer, sizeof(buffer), fp)!=NULL){
            line_counter++;
        }
        fclose(fp);

        // Carrega arquivo em memória

        char lines[line_counter][sizeof(buffer)];

        l=0;
        fp=fopen(path, "r");
        while (fgets(buffer, sizeof(buffer), fp) !=NULL ){
            strcpy(lines[l],chop(buffer));//lines[l] = chop(buffer);
            l++;
        }
        fclose(fp);

        // Calcula temperatura maxima
#pragma omp parallel for shared(temperature) private(substring,temp,special)
        for(i=0;i<line_counter;i++){
             strncpy(substring,lines[i]+87,5);
             temp = atoi(substring);
             special = lines[i][92] - '0';
             if(temp != 9999 && (special == 1 || special == 4 || special == 0 || special == 5 || special == 9))
                #pragma omp critical
                {
                if(temp > temperature)
                    temperature = temp;
                }
        }

        printf("rank %d ano %d: %.1f\n", rank,year, temperature/10);

        MPI_Send(0, 0, MPI_INT, 0, DONE_TAG, MPI_COMM_WORLD);
    }

}

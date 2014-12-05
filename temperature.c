#include <stdio.h>
#include <omp.h>
#include <mpi.h>
#include <stdlib.h>
#include <string.h>

#define YEAR_MIN 1930
#define YEAR_MAX 1943
#define DATASET_DIR "dataset/files/all/"
#define KILLTAG 9999
#define DONE_TAG 2

void master();
void slave(int num_threads);

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
    int num_threads = atoi(argv[1]);


    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    if(rank == 0){
        master();
    }
    else
        slave(num_threads);

    MPI_Finalize();
    return 0;
}

void master()
{
    int i,j,dest;
    double t1,t2;
    int nprocs,rank;
    int next_year;
    int finish;
    int years_total = YEAR_MAX - YEAR_MIN + 1;
    int years_done = 0;
    int *years_bag = (int *) malloc(years_total * sizeof(int));
    double received_temp;
    double final_temp = -1111;

    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    t1 = MPI_Wtime();

    j=0;
    for(i=YEAR_MIN; i<= YEAR_MAX;i++){
        years_bag[j] = i;
        j++;
    }

    // Envio inicial
    for(dest=1; dest < nprocs; dest++){
       next_year = dest-1;
       MPI_Send(&years_bag[next_year], 1, MPI_INT, dest,1,MPI_COMM_WORLD);
    }

    // Envia os próximos anos para processamento para os escravos liberados
    while(years_done < years_total){

        // verificar o próximo escravo liberado
        MPI_Recv(&received_temp, 1, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        if(received_temp > final_temp)
            final_temp = received_temp;
        printf("rank %d received %d: %.1f from %d\n",rank,status.MPI_TAG,received_temp,status.MPI_SOURCE);
        years_done++;
        next_year++;

        if(next_year < years_total)
            MPI_Send(&years_bag[next_year], 1, MPI_INT,status.MPI_SOURCE,1,MPI_COMM_WORLD);
    }

    t2 = MPI_Wtime();

    /* imprime a maior temperatura coletada */
    printf("Maior temperatura coletada: %.1f\n", final_temp);

    /* Envia a tag de kill para que os slaves parem o processamento */
    for(dest=1;dest<nprocs;dest++)
        MPI_Send(0,0,MPI_INT,dest,KILLTAG,MPI_COMM_WORLD);


    /* Imprime o tempo de execução */
    printf("Tempo de execucao: %f\n", t2-t1);

}

void slave(int num_threads)
{
    char buffer[400];
    char substring[5];
    char path[44];
    int year;
    int finish;
    int special;
    int line_counter;
    char year_str[5];
    double temperature;
    double temp;
    int i,l,k;
    FILE *fp;

    MPI_Status status;

    // Recebe ano para processamento
    while(1){

        line_counter=0;
        temperature = -1111;

        MPI_Recv(&year, 1, MPI_INT, 0, MPI_ANY_TAG,MPI_COMM_WORLD, &status);

        if (status.MPI_TAG == KILLTAG) {
            break;
        }

        // complementa o ano no diretorio raiz
        sprintf(year_str, "%d", year);
        strcpy(path,DATASET_DIR);
        strcat(path, year_str);

        // calcula o numero de linhas no arquivo
        fp=fopen(path, "r");
        while (fgets(buffer, sizeof(buffer), fp)!=NULL){
            line_counter++;
        }
        fclose(fp);

        // carrega arquivo em memória
        char **lines = (char**)malloc(line_counter*sizeof(char *));

        for(k=0;k<line_counter;k++){
                lines[k] = (char*)malloc(sizeof(buffer) * sizeof(char));
        }

        l=0;
        fp=fopen(path, "r");
        while (fgets(buffer, sizeof(buffer), fp) !=NULL ){
            lines[l] = chop(buffer);
            l++;
        }
        fclose(fp);

        // calcula temperatura maxima para o ano
        omp_set_num_threads(num_threads);
#pragma omp parallel for schedule(static,100)  shared(temperature,lines,line_counter) private(substring,temp,special)
        for(i=0;i<line_counter;i++){
            strncpy(substring,lines[i]+87,5);
            temp = atoi(substring);
            special = lines[i][92] - '0';
            if(temp != 9999 && temp > temperature && (special == 1 || special == 4 || special == 0 || special == 5 || special == 9))
               #pragma omp critical
               {
                temperature = temp;
               }
        }
        double adjusted_temp = temperature/10;
        MPI_Send(&adjusted_temp, 1, MPI_DOUBLE, 0, year, MPI_COMM_WORLD);

        //MPI_Send(&year, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

        // libera memória
        for(k=0;k<line_counter;k++){
                free(lines[k]);
        }

    }

}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h> 

#define PRINT 0
#define NUM_WORDS 80800
#define NUM_LINHAS 3000
#define BUF 256
 
struct histogram
{
        char word[80];
        int freq;
};
 
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
 
int check_word(char *token, struct histogram *histo, int max_words, int *return_index){
 
        int res=0;
        int i;
 
        for(i=0;i<max_words;i++){
             if(!strcmp(histo[i].word,token)){
                //histo[i].freq++;
                res = 1;
		*return_index = i;
             }
        }
        return res;
}
 
int main(int argc, char **argv){
 
        FILE *fp;
 
        int i,k,j,l;
        int check;
        double exec_time;
        struct histogram histo[NUM_WORDS];
 
        for(k=0;k<NUM_WORDS;k++)
                histo[k].freq = 0;
 
        char **lines = (char**)malloc(NUM_LINHAS*sizeof(char));
 
        for(k=0;k<NUM_LINHAS;k++){
                lines[k] = (char*)malloc(BUF * sizeof(char));
        }
 
        // Carrega arquivo em memoria, linha a linha
        char buffer[BUF];
 
        l=0;
        fp = fopen(argv[1], "r");
 
                if (!fp)
                        return 1;
                while (fgets(buffer, sizeof(buffer), fp)!=NULL){
                        lines[l] = chop(buffer);
                        l++;
                }
                fclose(fp);
 
        char * token;
 
        i=0;
        // Percorre cada linha do arquivo

	int return_index = 0;

        exec_time = omp_get_wtime ( );

#pragma omp parallel for schedule(static,1000) private(token)
        for(j=0; j < NUM_LINHAS ; j++){
                token = strtok(lines[j], " ");
		#pragma omp critical
		{
                while (token != NULL){
                        check = check_word(token, histo, NUM_WORDS, &return_index);
                        if(!check){
                                strcpy(histo[i].word, token);
                                histo[i].freq=1;
                        }
			else
				histo[return_index].freq = histo[return_index].freq+1;
                        token = strtok(NULL, " ");
                        i++;
                
        	}
		}

	}
 
     	exec_time = omp_get_wtime () - exec_time;
        printf ("tempo de execucao: %12f\n", exec_time);
 
        // Imprime ocorrencias
        if(PRINT){
            for(i=0;i<NUM_WORDS;i++)
                if(histo[i].freq > 1)
                    printf("%s: %d\n",histo[i].word,histo[i].freq);
        }
 
}

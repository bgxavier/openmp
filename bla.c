#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define PRINT 0
#define NUM_WORDS 100000
#define NUM_LINHAS 2161
#define LINE_SIZE 128

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

    for(i = 0; i < strlen(string)-1; i++)
    {
        newstring[i] = string[i];
    }

    return newstring;
}

int check_word(char *token, struct histogram *histo, int max_words){

	int res=0;
	int i;
	
	for(i=0;i<max_words;i++){
             if(!strcmp(histo[i].word,token)){
             	histo[i].freq++;
		res = 1;
	     }
	}
	return res;
}

int main(int argc, char **argv){

	FILE *fp;

	char line_size[LINE_SIZE];

	char **lines = (char**)malloc(NUM_LINHAS*sizeof(char));

	int i,k,j,l;
	int check;

	struct histogram histo[NUM_WORDS];

       	for(k=0;k<NUM_WORDS;k++)
                histo[k].freq = 0;

//        size_t buffer_size = 255;
  
        //char *buffer = malloc(buffer_size * sizeof(char));

	char buffer[128];

	for(k=0;k<NUM_LINHAS;k++){
		lines[k] = (char*)malloc(LINE_SIZE * sizeof(char));
	}

	// Carrega arquivo em memoria, linha a linha
	fp = fopen("arquivo", "r");
	
		if (!fp)
        		return 1;
		l=0;
    	
		while (fgets(buffer, 128, fp)!=NULL){
			lines[l] = chop(buffer);
			l++;
		}
        		//printf("%s",buffer);
		fclose(fp);

	//for(i=0;i<NUM_LINHAS;i++)
	//	printf("%s\n",lines[i]);

  //      l = 0;
//        while(-1 != getline(&buffer, &buffer_size, fp)){
//		printf("%s",buffer);
 //               strcpy(lines[l],chop(buffer));
   //             l++;
//	}

	char * token;

	printf("%s",lines[0]);

	i=0;
	// Percorre cada linha do arquivo
	for(j=0; j < NUM_LINHAS; j++){
		token = strtok(lines[0], " ");

		while (token != NULL){
		
			printf("%s",token);
		//	check = check_word(token, histo, NUM_LINHAS);
	//		if(!check){
		//		strcpy(histo[i].word, token);
		//		histo[i].freq=1;
	//		}
			token = strtok(NULL, " ");
			i++;
		}
	}

	if(PRINT){
	    for(i=0;i<NUM_WORDS;i++)
		if(histo[i].freq > 1)
	            printf("%s: %d\n",histo[i].word,histo[i].freq);
	}

}





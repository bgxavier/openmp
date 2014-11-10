#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char **argv){

	char *arquivo = argv[1];
	FILE *fp;
	char line[255];
	char word[255];
	char *token;
	int i =0;

        char **file_lines = (char **)malloc(1000 * sizeof(char *));

        for(i=0;i<1000;i++){
                file_lines[i] = (char*)malloc(200 * sizeof(char));
        }

	fp = fopen(arquivo, "r");

	size_t buffer_size = 80;
        char *buffer = malloc(buffer_size * sizeof(char));
 
	i = 0;
        while(-1 != getline(&buffer, &buffer_size, fp))
        {
		file_lines[i] = buffer;
		i++;

        }

	printf("%s",file_lines[0]);

        fflush(stdout);

        fclose(fp);


#pragma omp parallel for
	for(i=0;i<1000;i++)
		

} 

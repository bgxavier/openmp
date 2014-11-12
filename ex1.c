#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char **argv){

	char *arquivo = argv[1];
	FILE *fp;
	char *token;

	int i =0;

//        char **file_lines = (char **)malloc(1000 * sizeof(char *));

  	 char *file_lines[70000];

       for(i=0;i<70000;i++){
                file_lines[i] = (char*)malloc(2000 * sizeof(char));
        }

	fp = fopen(arquivo, "r");

	size_t buffer_size = 2000;

        char *buffer = malloc(buffer_size * sizeof(char));
 
	i = 0;
        while(-1 != getline(&buffer, &buffer_size, fp))
        {
		//printf("%s",buffer);

		strcpy(file_lines[i],buffer);

		i++;

        }

	printf("%s",file_lines[0]);

        fflush(stdout);

        fclose(fp);

}

CC=gcc
CFLAGS="-fopenmp"
 
all: openmp-helloworld
    echo "Done"
 
openmp-helloworld : openmp-helloworld.c
    ${CC} ${CFLAGS} -o openmp-helloworld openmp-helloworld.c
 
clean:
    ${RM} *.o openmp-helloworld

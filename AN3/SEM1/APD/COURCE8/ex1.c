#include <stdio.h>
#include <mpi.h>

int main(int argc, char* argv[]){
    int my_rank;
    int comm_sz;

    MPI_Init(&argc,&argv);

    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);

    printf("Succes!\n"); // programul lanseaza de 4 ori rank pt ca i-am 4 procese

    MPI_Finalize();

    return 0;
}

/*
mpicc -o ex1 ex1.c
mpirun -np 4 ./ex1
*/
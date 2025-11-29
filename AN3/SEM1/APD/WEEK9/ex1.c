#include <stdio.h>
#include "mpi.h"

int main(int argc, char *argv[]){
    int comm_sz;
    int my_rank;

    MPI_Init(NULL,NULL);

    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if(my_rank == 0){
        int a = 5;
        int b = 7;

        int v[2] = {a,b};
        int sum;

        MPI_Send(v, 2, MPI_INT, 1, 0, MPI_COMM_WORLD);
        printf("Procesul 0: am trimis %d si %d\n", a, b);

        MPI_Recv(&sum, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("Procesul 0: am primit suma = %d\n", sum);
    }else if(my_rank == 1){
        int v[2];
        int sum;

        MPI_Recv(v, 2, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        sum = v[0] + v[1];

        MPI_Send(&sum, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}

/*
mpicc -o ex1 ex1.c
mpirun -np 2 ./ex1
*/
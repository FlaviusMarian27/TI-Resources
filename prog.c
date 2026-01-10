#include <stdio.h>
#include "mpi.h"

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int msg;
    int next = (rank + 1) % size;  // next process 
    int prev = (rank + 2) % size;  // previous process

    MPI_Recv(&msg, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Send(&rank, 1, MPI_INT, next, 0, MPI_COMM_WORLD);

    printf("Process %d received %d from process %d\n", rank, msg, prev);

    MPI_Finalize();
    return 0;
}
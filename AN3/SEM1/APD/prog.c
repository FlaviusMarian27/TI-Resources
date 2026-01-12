#include <stdio.h>
#include "mpi.h"

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int x[3], y[3];  x[0]=y[0]=rank; x[1]=y[1]=rank+1; x[2]=y[2]=rank+2;
    MPI_Scatter(x, 1, MPI_INT, y, 1, MPI_INT, 2, MPI_COMM_WORLD);  
    printf("%d %d %d\n", y[0], y[1], y[2]);
    MPI_Finalize();
    return 0;
}
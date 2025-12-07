#include <stdio.h>
#include <mpi.h>

int main(int argc, char* argv[]){
    int my_rank;
    int comm_sz;

    MPI_Init(&argc,&argv);

    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);

    if(my_rank != 0){
        int x = my_rank;
        MPI_Send(&x,1,MPI_INT,0,0,MPI_COMM_WORLD);
    }else{
        int from;
        for(int src = 1; src < comm_sz; src++){
            MPI_Recv(&from,1,MPI_INT,src,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            printf("Am primit %d de la %d\n", from, src);
        }
    }

    MPI_Finalize();

    return 0;
}

/*
mpicc -o hello hello.c
mpirun -np 4 ./hello
*/
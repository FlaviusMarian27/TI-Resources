#include <stdio.h>
#include <omp.h>

int main(void){
    int my_rank;

    #pragma omp parallel num_threads(20) private(my_rank)
    {
        my_rank = omp_get_thread_num();
        if(my_rank == 0){
            printf("Hello from thread=%d. There are %d threads\n",
                my_rank,omp_get_num_threads());
        }else{
            printf("Hello World from thread=%d\n",my_rank);
        }
    }

    return 0;
}
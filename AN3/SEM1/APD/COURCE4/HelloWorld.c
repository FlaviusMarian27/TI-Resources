#include <stdio.h>
#include <omp.h>

int main(void){
    printf("This is BEFORE parallel\n");

    #pragma omp parallel
    {
        printf("Hello World from thread %d out of %d\n",
                omp_get_thread_num(),omp_get_thread_num());
    }

    printf("This is AFTER parallel\n");

    return 0;
}
#include <stdio.h>
#include <omp.h>

#define NTHREADS 4
#define N 5

//aici practic fiecare thread ia din iteratie pe rand
void fct1(void){
    #pragma omp parallel for num_threads(NTHREADS)
        for(int i = 0; i < N; i = i + 1){
            printf("hello ");
            printf("Thread %d has incremented counter to %d\n",
                omp_get_thread_num(),i);
        }
}

//aici fiecare thread face de 5 ori fiecare print => 20
void fct2(void){
    #pragma omp parallel num_threads(NTHREADS)
        for(int i = 0; i < N; i = i + 1){
            printf("hello ");
            printf("Thread %d has incremented counter to %d\n",
                omp_get_thread_num(),i);
        }
}

int main(void){

    fct1();
    printf("\n");
    fct2();

    return 0;
}
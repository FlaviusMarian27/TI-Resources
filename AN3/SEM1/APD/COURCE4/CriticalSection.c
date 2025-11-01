#include<stdio.h>
#include<omp.h>

int main(void){

    int counter = 0;
    #pragma omp parallel num_threads(10) shared(counter)
    {
        counter = counter + 1;//WRONG! RACE CONDITION
        printf("Thread %d has incremented counter to %d\n",omp_get_thread_num(),counter);
    }

    printf("Counter value: %d\n\n",counter);

    //varianta corecta

    int counter2 = 0;
    #pragma omp parallel num_threads(10) shared(counter2)
    {
        #pragma omp critical
        {
            counter2 = counter2 + 1;
            printf("Thread %d has incremented counter to %d\n",
                omp_get_thread_num(),counter2);
        }
    }

    printf("Counter 2 value: %d\n",counter2);
    return 0;
}
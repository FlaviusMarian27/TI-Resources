#include <stdio.h>
#include <omp.h>

#define N 20

//lent dar corect blocheaza threadurile
void suma1(void){
    int s = 0;

    #pragma omp parallel for shared(s)
        for(int i = 0; i < N; i = i + 1){
            #pragma omp critical
            {
                s = s + i;
                printf("Thread %d has incremented s to %d\n",
                omp_get_thread_num(),s);
            }
        }

    printf("S = %d\n\n",s);
}

//mai rapid dar tine bloc cate un thread
void suma2(void){
    int s = 0;

    #pragma omp parallel shared(s)
    {
        int local_s = 0;

        #pragma omp for
            for(int i = 0; i < N; i = i + 1){
                local_s = local_s + i;
                printf("Thread %d has incremented local_s to %d\n",
                omp_get_thread_num(),local_s);
            }
        
        #pragma omp critical
            s = s + local_s;
    }

    printf("S = %d\n\n",s);
}

//cel mai eficient si elegant
void suma3(void){
    int s = 0;

    #pragma omp parallel for reduction(+:s)
        for(int i = 0; i < N; i = i + 1){
            s = s + i;
            printf("Thread %d has incremented s to %d\n",
                omp_get_thread_num(),s);
        }

    printf("S = %d\n",s);
}

int main(void){
    suma1();
    suma2();
    suma3();

    return 0;
}
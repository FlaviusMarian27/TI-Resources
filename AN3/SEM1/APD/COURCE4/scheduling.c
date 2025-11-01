#include <stdio.h>
#include <omp.h>

#define NTHREADS 4
#define POLICY static // or dynamic
#define CHUNKSIZE 2 // or 1, 3
#define N 10

void test(void){
    #pragma omp parallel for num_threads(NTHREADS) schedule(POLICY, CHUNKSIZE)
        for (int i = 0; i <N; i++){
            printf("iteration %d done by thread %d \n", i, omp_get_thread_num());
        }
}
//Da, cu schedule(static, 2) → ai 4 threaduri care preiau 
//10 iterații în blocuri de câte 2, fix repartizate la începutul execuției.
int main(void){
    test();

    return 0;
}
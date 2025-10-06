/**
 * Note the difference between measuring wall-clock time and CPU time!
 * 
 * clock_gettime() - measures wallclock time or CPUtime, depending on first argument used
 * clock_gettime(CLOCK_MONOTONIC, &t)
 * clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t); 
 * 
 * clock() - not a portable solution! measures CPU time on unix and wall-clock on windows 
 */

#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>   // for sleep()
#include <sys/time.h>

/* size of dummy workload for threads*/
#define MAXWORK 20000

/* number of working threads */
#define NTHREADS 4

/* thread function 1 - does some dummy counting */
void *pth_fct1(void *dummy)
{
    int count = 0;
    for (int i = 0; i < MAXWORK; i++)
        for (int j = 0; j < MAXWORK; j++)
            count++;
    return NULL;
} /* pth_fct1 */

/* solution 1 - spawns threads executing pth_fct1 */
void solution1(void)
{
    long thread;
    pthread_t *thread_handles;

    thread_handles = malloc(NTHREADS * sizeof(pthread_t));

    for (thread = 0; thread < NTHREADS; thread++)
        pthread_create(&thread_handles[thread], NULL,
                       pth_fct1, NULL);

    for (thread = 0; thread < NTHREADS; thread++)
        pthread_join(thread_handles[thread], NULL);

    free(thread_handles);
}

/* thread function 1 - does dummy counting  AND sleeps in between*/
void *pth_fct2(void *dummy)
{
    int count = 0;

    for (int i = 0; i < MAXWORK; i++)
    {
        if (i % 2000 == 0)
            sleep(1);
        for (int j = 0; j < MAXWORK; j++)
            count++;
    }

    return NULL;
} /* pth_fct2 */

/* solution 2 - spawns threads executing pth_fct1 */
void solution2(void)
{
    long thread;
    pthread_t *thread_handles;

    thread_handles = malloc(NTHREADS * sizeof(pthread_t));

    for (thread = 0; thread < NTHREADS; thread++)
        pthread_create(&thread_handles[thread], NULL,
                       pth_fct2, NULL);

    for (thread = 0; thread < NTHREADS; thread++)
        pthread_join(thread_handles[thread], NULL);

    free(thread_handles);
}

void main(void)
{
    struct timespec start, finish;
    double TIME1, TIME2, TIME3, TIME4, TIME5, TIME6;

    /**
     * Experiment1: Measuring Wall-clock time 
     * uses clock_gettime  CLOCK_MONOTONIC
     */

    clock_gettime(CLOCK_MONOTONIC, &start);
    solution1();
    clock_gettime(CLOCK_MONOTONIC, &finish);
    TIME1 = (finish.tv_sec - start.tv_sec) + (finish.tv_nsec - start.tv_nsec) / 1e9;


    clock_gettime(CLOCK_MONOTONIC,&start);
    solution2();
    clock_gettime(CLOCK_MONOTONIC, &finish);
    TIME2 = (finish.tv_sec - start.tv_sec) + (finish.tv_nsec - start.tv_nsec) / 1e9;

    //CPU_time
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
    solution1();
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &finish);
    TIME3 = (finish.tv_sec - start.tv_sec) + (finish.tv_nsec - start.tv_nsec) / 1e9;


    clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&start);
    solution2();
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &finish);
    TIME4 = (finish.tv_sec - start.tv_sec) + (finish.tv_nsec - start.tv_nsec) / 1e9;

    //clock()
    clock_t cstart,cend;
    cstart = clock();
    solution1();
    cend = clock();
    TIME5 = (double)(cend - cstart) / CLOCKS_PER_SEC;

    cstart = clock();
    solution2();
    cend = clock();
    TIME6 = (double)(cend - cstart) / CLOCKS_PER_SEC;


    printf("\n=== RESULTS ===\n");
    printf("TIME1 (Wall-clock, sol1): %lf sec\n", TIME1);
    printf("TIME2 (Wall-clock, sol2): %lf sec\n", TIME2);
    printf("TIME3 (CPU_time, sol1): %lf sec\n", TIME3);
    printf("TIME4 (CPU_time, sol2): %lf sec\n", TIME4);
    printf("TIME5 (clock(), sol1): %lf sec\n", TIME5);
    printf("TIME6 (clock(), sol2): %lf sec\n", TIME6);
}
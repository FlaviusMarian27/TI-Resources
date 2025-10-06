/**
 * Note the difference between measuring wall-clock time and CPU time!
 * 
 * clock_gettime() - measures wallclock time or CPUtime, depending on first argument used
 * clock_gettime(CLOCK_MONOTONIC, &t)
 * clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t); 
 * 
 * clock() - not a portable solution! measures CPU time on unix and wall-clock on windows 
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

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

    /**
     * Experiment1: Measuring Wall-clock time 
     * uses clock_gettime  CLOCK_MONOTONIC
     */

    solution1();

    solution2();

    /**
     *  Experiment2: Measuring CPU time PER PROCESS (all threads)
     *  uses clock_gettime CLOCK_PROCESS_CPUTIME_ID
     */
    
    solution1();

    solution2();

    /**
     *  Experiment3: using clock()
     *  CPU time on unix/linux
     *  wall-clock time on windows
     */
    
    solution1();

    solution2();
}
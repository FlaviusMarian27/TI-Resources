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
#include <unistd.h>

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

int main(void)
{

    /*
     * Experiment1: Measuring Wall-clock time 
     * uses clock_gettime  CLOCK_MONOTONIC
     */

    struct timespec start, stop;
    double TIME1, TIME2;
    
    //Wall-clock time
    clock_gettime(CLOCK_MONOTONIC,&start);
    solution1();
    clock_gettime(CLOCK_MONOTONIC,&stop);
    TIME1 = (stop.tv_sec - start.tv_sec) + (stop.tv_nsec - start.tv_nsec) / 1e9;

    clock_gettime(CLOCK_MONOTONIC,&start);
    solution2();
    clock_gettime(CLOCK_MONOTONIC,&stop);
    TIME2 = (stop.tv_sec - start.tv_sec) + (stop.tv_nsec - start.tv_nsec) / 1e9;

    printf("Time1 (CLOCK_MONOTONIC): %lf\n",TIME1);
    printf("Time2 (CLOCK_MONOTONIC): %lf\n",TIME2);

    /*
     *  Experiment2: Measuring CPU time PER PROCESS (all threads)
     *  uses clock_gettime CLOCK_PROCESS_CPUTIME_ID
    */
    
    //CPU_time
    double TIME3, TIME4;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&start);
    solution1();
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&stop);
    TIME3 = (stop.tv_sec - start.tv_sec) + (stop.tv_nsec - start.tv_nsec) / 1e9;

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&start);
    solution2();
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&stop);
    TIME4 = (stop.tv_sec - start.tv_sec) + (stop.tv_nsec - start.tv_nsec) / 1e9;

    printf("Time3 (CLOCK_PROCESS_CPUTIME_ID): %lf\n",TIME3);
    printf("Time4 (CLOCK_PROCESS_CPUTIME_ID): %lf\n",TIME4);

    /*
     *  Experiment3: using clock()
     *  CPU time on unix/linux
     *  wall-clock time on windows
    */
    
    clock_t start_t, end_t;
    double TIME5, TIME6;
    start_t = clock();
    solution1();
    end_t = clock();
    TIME5 = (double)(end_t - start_t)/CLOCKS_PER_SEC;

    start_t = clock();
    solution2();
    end_t = clock();
    TIME6 = (double)(end_t - start_t)/CLOCKS_PER_SEC;

    printf("Time5 (clock): %lf\n",TIME5);
    printf("Time6 (clock): %lf\n",TIME6);

    return 0;
}

/*
Output:
Time1 (CLOCK_MONOTONIC): 0.155070
Time2 (CLOCK_MONOTONIC): 10.196808
Time3 (CLOCK_PROCESS_CPUTIME_ID): 0.606284
Time4 (CLOCK_PROCESS_CPUTIME_ID): 0.770582
Time5 (clock): 0.609938
Time6 (clock): 0.767442

-> wall-clock time ul intre time1 si time2. TIME2 > TIME1 pt ca solution2() include
niste timpi de asteptare, adica fiecare thread sta de multe ori in sleep(1)

-> CPU_time dintre TIME3 si TIME4. Sunt aproximativ egale, cu mica diferenta, TIME4
este putin mai mare decat TIME3, astfel sleep(1) nu influenteaza atat de mult timpul
cum a fost in cazul anterior pt wall-clock time

->TIME1 vs TIME3. TIME3 > TIME1 pentru ca TIME3 aduna practic CPU_TIME_TOTAL, adica daca
avem 4 threaduri si fiecare thread are o durata de 0.15 secunde, rezultatul pt timpul
final este suma celor 4 threaduri, adica 0.60, iar TIME1 masoara practic durata totala
a executiei reale, care s-a desfasurat in paralel

->TIME2 vs TIME4. TIME4 > TIME2 pt ca solution2 petrece o mare parte din executie 
in sleep(1), iar procesul este inactiv, iar procesorul nu lucreaza an acel tim
    
*/
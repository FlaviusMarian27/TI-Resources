/* File:  abc.c
Creates 3 threads.
First thread repeatedly writes A.
Second thread repeatredly writes B.
Third thread repeatedly writes C.
The main thread repeatedly writes newline.

Add synchronization such that the output is always in the form:
on each new line there is a random permutation of A,B,C.
You can add any synchronization that you want, but you 
can not change the existing printf's or add new printf's

For example, the output can be:
ABC
BAC
ABC
BCA
...
 */

#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define REPEAT 100

pthread_barrier_t bar;

/* Thread function A */
void *HelloA(void *dummy)
{
    for (int i = 0; i < REPEAT; i++){
        printf("A");
        pthread_barrier_wait(&bar);
        pthread_barrier_wait(&bar);
    }

    return NULL;
}

/* Thread function B */
void *HelloB(void *dummy)
{
    for (int i = 0; i < REPEAT; i++){
        printf("B");
        pthread_barrier_wait(&bar);
        pthread_barrier_wait(&bar);
    }

    return NULL;
}

/* Thread function C */
void *HelloC(void *dummy)
{
    for (int i = 0; i < REPEAT; i++){
        printf("C");
        pthread_barrier_wait(&bar);
        pthread_barrier_wait(&bar);
    }

    return NULL;
}

int main(int argc, char *argv[])
{

    pthread_t thread_handleA, thread_handleB, thread_handleC;

    pthread_barrier_init(&bar,NULL,4);

    pthread_create(&thread_handleA, NULL, HelloA, NULL);
    pthread_create(&thread_handleB, NULL, HelloB, NULL);
    pthread_create(&thread_handleC, NULL, HelloC, NULL);

    for (int i = 0; i < REPEAT; i++){
        pthread_barrier_wait(&bar);
        printf("\n");
        pthread_barrier_wait(&bar);
    }

    pthread_join(thread_handleA, NULL);
    pthread_join(thread_handleB, NULL);
    pthread_join(thread_handleC, NULL);

    pthread_barrier_destroy(&bar);
    return 0;
}

/*
- pt afisare fiecarei litere o singura data, urmata de new line, ne-am folosit de
bariere pentru a pute sincroniza threadurile intre ele
- am folosit de doua apeluri pthread_barrier_wait() pt fiecare iteratie pentru astepta
ca fiecare thread sa afiseze o litera si dupa sa se afiseze newlineul, astfel evitand
sa se afiseze newlineul inaintea oricarui caracter litera
*/
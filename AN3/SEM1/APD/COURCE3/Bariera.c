/*
O barieră este un punct de sincronizare unde toate threadurile trebuie să ajungă, 
înainte ca oricare dintre ele să poată merge mai departe.
*/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NTHREADS 4

pthread_barrier_t barrier;

void* worker(void* arg) {
    int id = *(int*)arg;
    printf("Thread %d: Faza 1 începe\n", id);
    sleep(1 + id % 2); // simulăm timp diferit de execuție
    printf("Thread %d: A terminat faza 1, așteaptă la barieră\n", id);

    pthread_barrier_wait(&barrier); // toți așteaptă aici

    printf("Thread %d: Toți au ajuns, continuăm cu faza 2!\n", id);
    return NULL;
}

int main() {
    pthread_t threads[NTHREADS];
    int ids[NTHREADS];

    pthread_barrier_init(&barrier, NULL, NTHREADS);

    for (int i = 0; i < NTHREADS; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, worker, &ids[i]);
    }

    for (int i = 0; i < NTHREADS; i++)
        pthread_join(threads[i], NULL);

    pthread_barrier_destroy(&barrier);
    return 0;
}

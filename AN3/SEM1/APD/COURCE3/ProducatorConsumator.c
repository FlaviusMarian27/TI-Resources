/*
Ai:

un buffer comun (de exemplu o coadă / vector);

mai multe threaduri producătoare care adaugă elemente;

mai multe threaduri consumatoare care scot elemente.

 Problema:
→ dacă producătorul pune prea mult, bufferul se umple;
→ dacă consumatorul vrea să scoată, dar bufferul e gol, trebuie să aștepte.
*/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define N_PROD 2
#define N_CONS 2

int buffer[BUFFER_SIZE];
int count = 0;  // câte elemente sunt în buffer

pthread_mutex_t mutex;
pthread_cond_t not_full, not_empty;

void* producer(void* arg) {
    int id = *(int*)arg;
    for (int i = 0; i < 10; i++) {
        pthread_mutex_lock(&mutex);
        while (count == BUFFER_SIZE) {
            printf("Producator %d: buffer plin, astept...\n", id);
            pthread_cond_wait(&not_full, &mutex);
        }

        buffer[count++] = i;
        printf("Producator %d: a produs %d (count=%d)\n", id, i, count);

        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    return NULL;
}

void* consumer(void* arg) {
    int id = *(int*)arg;
    for (int i = 0; i < 10; i++) {
        pthread_mutex_lock(&mutex);
        while (count == 0) {
            printf("Consumator %d: buffer gol, astept...\n", id);
            pthread_cond_wait(&not_empty, &mutex);
        }

        int item = buffer[--count];
        printf("Consumator %d: a consumat %d (count=%d)\n", id, item, count);

        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&mutex);
        sleep(2);
    }
    return NULL;
}

int main() {
    pthread_t prod[N_PROD], cons[N_CONS];
    int ids[N_PROD > N_CONS ? N_PROD : N_CONS];

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&not_full, NULL);
    pthread_cond_init(&not_empty, NULL);

    for (int i = 0; i < N_PROD; i++) {
        ids[i] = i;
        pthread_create(&prod[i], NULL, producer, &ids[i]);
    }
    for (int i = 0; i < N_CONS; i++) {
        ids[i] = i;
        pthread_create(&cons[i], NULL, consumer, &ids[i]);
    }

    for (int i = 0; i < N_PROD; i++)
        pthread_join(prod[i], NULL);
    for (int i = 0; i < N_CONS; i++)
        pthread_join(cons[i], NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&not_full);
    pthread_cond_destroy(&not_empty);

    return 0;
}

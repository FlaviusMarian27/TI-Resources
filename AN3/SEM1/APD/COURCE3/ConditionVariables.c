/*
Sunt un mecanism prin care un thread poate aștepta un eveniment, 
iar alt thread poate semnala că acel eveniment s-a întâmplat.

Le folosești împreună cu un mutex, niciodată singure.
*/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int data_ready = 0; // flag simplu
pthread_mutex_t lock;
pthread_cond_t cond;

void* producer(void* arg) {
    sleep(2); // simulăm procesul de producere
    pthread_mutex_lock(&lock);
    data_ready = 1;
    printf("Producător: am produs date!\n");
    pthread_cond_signal(&cond); // anunță consumatorul
    pthread_mutex_unlock(&lock);
    return NULL;
}

void* consumer(void* arg) {
    pthread_mutex_lock(&lock);
    while (data_ready == 0) {
        printf("Consumator: aștept date...\n");
        pthread_cond_wait(&cond, &lock); // eliberează lock-ul temporar
    }
    printf("Consumator: am primit date!\n");
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main() {
    pthread_t prod, cons;

    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_create(&cons, NULL, consumer, NULL);
    pthread_create(&prod, NULL, producer, NULL);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);

    return 0;
}

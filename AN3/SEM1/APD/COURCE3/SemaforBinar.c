/*
Semafor = mecanism de control al accesului concurent

Semaforul este binar sau general

Semafor binar = mutex, are valori de 1 si 0, unde 1 este liber si 0 ocupat
este echivalent cu lock si unlock de la mutex
*/

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

int counter = 0;
sem_t sem; // semafor binar

void* increment(void* increment){
    for(int i = 0; i < 1000000; i++){
        sem_wait(&sem);
        counter++;
        sem_post(&sem);
    }

    return NULL;
}

int main(void){
    pthread_t t1, t2;

    sem_init(&sem,0,1);
    pthread_create(&t1,NULL,increment,NULL);

    /*
    pthread_creat(ID-ul threadului creat,
                  atribute speciale (de obicei NULL),
                  funcția pe care o rulează threadul,
                  Este argumentul transmis funcției increment)
    */

    pthread_create(&t2,NULL,increment,NULL);

    pthread_join(t1,NULL);
    /*
        pthread_join(Este ID-ul threadului (returnat de pthread_create,
                    ignori rezultatul returnat de thread (nu te interesează)) 
    */

    pthread_join(t2,NULL);

    sem_destroy(&sem);
    printf("Counter final: %d\n", counter);

    return 0;
}
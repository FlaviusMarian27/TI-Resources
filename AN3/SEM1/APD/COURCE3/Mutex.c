/*
- Un mutex este un lacat care permite accesul unui singur thread la o resursa. Mai
exact intr-o zona de punct critic. Punct critic este o bucata de cod unde se 
acceseaza resurse partajate ex count++

- Pe scurt mutex permite unui singur thread sa acceseze variabila share uita

- Threadul care a facut locked ala trebuie sa fac si unlocked

result este 2000000 - practic fiecare thread calculeaza pe rand
*/

#include <stdio.h>
#include <pthread.h>

int counter = 0;
pthread_mutex_t lock;

void* increment(void* arg){
    for(int i = 0; i < 1000000; i++){
        pthread_mutex_lock(&lock);
        counter++;
        pthread_mutex_unlock(&lock);
    }

    return NULL;
}

int main(void){

    pthread_t t1, t2;

    pthread_mutex_init(&lock,NULL);

    pthread_create(&t1,NULL,increment,NULL);
    pthread_create(&t2,NULL,increment,NULL);

    pthread_join(t1,NULL);
    pthread_join(t2,NULL);

    pthread_mutex_destroy(&lock);

    printf("Rezultatul final: %d\n",counter);
    return 0;
}
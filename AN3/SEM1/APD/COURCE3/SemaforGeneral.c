#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_CONCURRENT 3
#define NTHREADS 8

sem_t sem;

void *task(void *arg){
    int id = *(int*)arg;
    sem_wait(&sem);

    printf("Treadul %d intra in sectiunea critica!\n",id);
    sleep(1);
    printf("Treadul %d iese din sectiunea critica!\n",id);

    sem_post(&sem);

    return NULL;
}

int main(void){
    pthread_t threads[NTHREADS];
    int ids[NTHREADS];

    sem_init(&sem,0,MAX_CONCURRENT);//permite cate trei simultan

    for(int i = 0; i < NTHREADS; i++){
        ids[i] = i;
        pthread_create(&threads[i],NULL,task,&ids[i]);
    }

    for(int i = 0; i < NTHREADS; i++){
        pthread_join(threads[i],NULL);
    }

    sem_destroy(&sem);

    return 0;
}
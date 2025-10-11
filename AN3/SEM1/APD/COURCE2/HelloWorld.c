#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

int thread_count;

void *Hello(void *rank){
    int my_rank = *(int*)rank;
    printf("Hello from thread %d of %d!\n",my_rank,thread_count);
    return NULL;
}

int main(int argc, char* argv[]){
    int i;
    pthread_t *thread_handles;
    int *tid;

    thread_count = 10;
    thread_handles = malloc(thread_count * sizeof(pthread_t));
    tid = malloc(thread_count * sizeof(int));

    for(i = 0; i < thread_count; i = i + 1){
        tid[i] = i;
        pthread_create(&thread_handles[i],NULL,Hello,(void*)&tid[i]);
    }

    printf("Hello from the main thread\n");

    for(i = 0; i < thread_count; i = i + 1){
        pthread_join(thread_handles[i],NULL);
    }

    free(thread_handles);
    free(tid);
    
    return 0;
}
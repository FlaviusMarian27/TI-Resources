#include<stdio.h>
#include<pthread.h>

void *HelloA(void *dummy){
    for(int i = 0; i < 200; i = i + 1){
        printf("AAAA");
    }
    return NULL;
}

void *HelloB(void *dummy){
    for(int i = 0; i < 200; i = i + 1){
        printf("BBBB");
    }
    return NULL;
}

int main(int argc, char* argv[]){
    pthread_t thread_handle_A, thread_handle_B;

    pthread_create(&thread_handle_A,NULL,HelloA,NULL);
    pthread_create(&thread_handle_B,NULL,HelloB,NULL);

    for(int i = 0; i < 200; i = i + 1){
        printf("main");
    }

    pthread_join(thread_handle_A,NULL);
    pthread_join(thread_handle_B,NULL);

    return 0;
}
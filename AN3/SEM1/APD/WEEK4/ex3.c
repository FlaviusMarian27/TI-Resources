#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define REPEAT 5    /* each thread is doing REPEAT operations  */
#define NUM_THREADS 4

typedef struct node{
    int data;
    struct node* next;
}queueNode;

queueNode *head = NULL;
queueNode *tail = NULL;

int isEmpty(void){
    return (head == NULL);
}

void put(int val){
    struct node *new = (struct node *)malloc(sizeof(struct node));
    if(new == NULL){
        perror("Eroare la crearea nodului!\n");
        exit(-1);
    }

    new->data = val;
    new->next = NULL;

    if(!isEmpty()){
        tail-> next = new;
        tail = new;
    }else{
        head = new;
        tail = new;
    }
}

int get()
{
    if (isEmpty()){
        printf("Error! empty queue \n");
        exit(1);
    }

    struct node *oldhead = head;
    int rez = oldhead->data;
    head = head->next;

    if (head == NULL){
        tail = NULL;
    }
        
    free(oldhead);
    return rez;
}

pthread_mutex_t mutex;
pthread_cond_t cond;

void enqueue(int value){
    pthread_mutex_lock(&mutex);
    put(value);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
}

int dequeue(){
    pthread_mutex_lock(&mutex);

    while(isEmpty()){
        pthread_cond_wait(&cond,&mutex);
    }

    int value = get();
    pthread_mutex_unlock(&mutex);

    return value;
}

void *producer(void *t){
    int i;
    int my_id = *(int*)t;
    for(i = 0; i < REPEAT; i++){
        enqueue(i + my_id * 10);
        printf("Producer %d produced %d\n", my_id, i + my_id * 10);
        sleep(1);
    }

    return NULL;
}

void *consumer(void *t){
    int i;
    int my_id = *(int*)t;
    for(i = 0; i < REPEAT; i++){
        int rez = dequeue();
        printf("Consumer thread %d got %d \n", my_id, rez);
        sleep(1);
    }

    return NULL;
}

int main(void){
    pthread_t threads[NUM_THREADS];
    int ids[NUM_THREADS];

    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&cond,NULL);

    for(int i = 0; i < NUM_THREADS; i++){
        ids[i] = i;
        if(i % 2 == 0){
            pthread_create(&threads[i],NULL,producer,&ids[i]);
        }else{
            pthread_create(&threads[i],NULL,consumer,&ids[i]);
        }
    }

    for(int i = 0; i < NUM_THREADS; i++ ){
        pthread_join(threads[i],NULL);
    }

    pthread_mutex_destroy(&mutex);

    return 0;
}

/*
- pt problema producator consumator am folosit un buffer nelimitat, implementat prin
intermediul unei cozii cu liste alocate dinamic. Astfel folosesc mutex pentru 
sincronizarea intre threaduri si condition variables pentru semnala cand, coada 
noastra este goala sau daca are elemente. 
*/
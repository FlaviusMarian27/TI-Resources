/*
3. Acelasi enunt ca si problema 2 dar programul va afisa in plus si numarul total de aparitii al caracterului
in toate liniile citite.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define LINE_SIZE 1024

typedef struct{
    char buffer_line[LINE_SIZE];
    char character;
    int counter;
    int rank;
}ThreadData_t;

pthread_mutex_t mutex;
int total_counter = 0;

void *ThreadProcess(void *arg){
    ThreadData_t *thread_data = (ThreadData_t *)arg;

    for (int i = 0; i < strlen(thread_data->buffer_line); i++){
        if (thread_data->buffer_line[i] == thread_data->character){
            thread_data->counter++;
        }
    }

    printf("Thread %d found %d occurrences of '%c'\n",thread_data->rank, thread_data->counter, thread_data->character);

    if (pthread_mutex_lock(&mutex) != 0){
        fprintf(stderr, "Error locking mutex\n");
        exit(EXIT_FAILURE);
    }

    total_counter = total_counter + thread_data->counter;

    if (pthread_mutex_unlock(&mutex) != 0){
        fprintf(stderr, "Error unlocking mutex\n");
        exit(EXIT_FAILURE);
    }

    free(thread_data);
    return NULL;
}

int main(int argc, const char * argv[]){
    if(argc != 2){
        fprintf(stderr, "Usage: %s <ch>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    pthread_mutex_init(&mutex, NULL);

    char buffer[LINE_SIZE];
    int rank = 0;
    while (fgets(buffer, LINE_SIZE, stdin) != NULL){
        ThreadData_t *thread_data = (ThreadData_t*)malloc(sizeof(ThreadData_t));
        if (thread_data == NULL){
            fprintf(stderr, "Error allocating memory for thread data\n");
            exit(EXIT_FAILURE);
        }

        strcpy(thread_data->buffer_line, buffer);
        thread_data->character = argv[1][0];
        thread_data->rank = rank;
        thread_data->counter = 0;

        pthread_t thread;
        if (pthread_create(&thread, NULL, ThreadProcess, (void *)thread_data) != 0){
            fprintf(stderr, "Error creating thread\n");
            exit(EXIT_FAILURE);
        }

        pthread_join(thread, NULL);

        rank = rank + 1;
    }

    pthread_mutex_destroy(&mutex);

    printf("Am iesit din while()\n");
    printf("Total occurrences of '%c' in all lines: %d\n", argv[1][0], total_counter);

    return 0;
}
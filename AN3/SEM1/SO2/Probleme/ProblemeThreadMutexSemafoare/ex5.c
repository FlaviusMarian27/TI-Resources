/*
5. Acelasi enunt ca si la problema 4 cu modificarea ca in momentul in care un thread va identifica un numar prim
din subinterval aceasta il va adauga intr-un tablou comun tuturor thread-urile. Cand toate thread-urile au terminat
calculul programul principal va afisa la sfarsit tabloul cu toate numerele prime gasite.
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

pthread_mutex_t mutex;
int counter = 0;
int *array;

typedef struct{
    int A;
    int start;
    int end;
    int total_numbers;
    int total_threads;
    int rank;
}ThreadData_t;

int is_Prime(int number){
    if (number < 2){
        return 0;
    }

    for (int i = 2; i <= number/2; i++){
        if (number % i == 0){
            return 0;
        }
    }

    return 1;
}

void *thread_function(void *thread_id){
    ThreadData_t *thread_data = (ThreadData_t *)thread_id;

    thread_data->start = (thread_data->rank * (thread_data->total_numbers / thread_data->total_threads)) + thread_data->A;
    thread_data->end = (((thread_data->rank + 1) * (thread_data->total_numbers / thread_data->total_threads)) + thread_data->A) - 1;

    for(int i = thread_data->start; i < thread_data->end; i++){
        if(is_Prime(i) == 1){
            printf("Thread %d found %d which is prime\n",thread_data->rank,i);

            if (pthread_mutex_lock(&mutex) != 0){
                printf("Error locking mutex\n");
                exit(EXIT_FAILURE);
            }

            array[counter++] = i;

            if (pthread_mutex_unlock(&mutex) != 0){
                printf("Error unlocking mutex\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    return NULL;
}

int main(int argc, char *argv[]){
    if (argc != 4){
        fprintf(stderr, "Usage: ./ex4 A B N\n");
        exit(EXIT_FAILURE);
    }

    pthread_mutex_init(&mutex, NULL);
    int A = atoi(argv[1]);
    int B = atoi(argv[2]);
    int N = atoi(argv[3]);

    array = (int*)malloc((B-A+1)*sizeof(int));
    if (array == NULL){
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    pthread_t threads[N];
    ThreadData_t *thread_data = (ThreadData_t*)malloc(sizeof(ThreadData_t) * N);
    if (thread_data == NULL){
        fprintf(stderr, "Error allocating memory for thread data\n");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < N; i++){
        thread_data[i].A = A;
        thread_data[i].total_numbers = B - A + 1;
        thread_data[i].total_threads = N;
        thread_data[i].rank = i;
        thread_data[i].start = 0;
        thread_data[i].end = 0;

        if (pthread_create(&threads[i], NULL, thread_function, &thread_data[i]) != 0){
            fprintf(stderr, "Error creating thread\n");
            exit(EXIT_FAILURE);
        }
    }

    for(int i = 0; i < N; i++){
        pthread_join(threads[i], NULL);
    }

    printf("[ ");
    for (int i = 0; i < counter; i++){
        printf("%d ", array[i]);
    }
    printf("]\n");

    pthread_mutex_destroy(&mutex);
    free(array);
    free(thread_data);
    return 0;
}
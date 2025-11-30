/*
4. Sa se scrie un program care primeste ca si argumente doua numere ce reprezinta captele unui interval A si B si
un alt treilea numar N ce va reprezeinta un numar de thread-uri pe care programul le va crea. Programul va imparti
intervalul [A, B] in N intervale egale. Apoi programul va crea N threaduri. Fiecare thread va procesa cate un
subinterval din intervalul [A,B] si va identifica numere prime din subinerval. In momentul in care un thread va
identifica cate un numar prim il va printa la iesirea standard.
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

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
        }
    }

    return NULL;
}

int main(int argc, char *argv[]){
    if (argc != 4){
        fprintf(stderr, "Usage: ./ex4 A B N\n");
        exit(EXIT_FAILURE);
    }

    int A = atoi(argv[1]);
    int B = atoi(argv[2]);
    int N = atoi(argv[3]);

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

    free(thread_data);
    return 0;
}
/*
4. Sa se scrie un program care primeste ca si argumente doua numere ce reprezinta captele
unui interval A si B si un alt treilea numar N ce va reprezeinta un numar de thread-uri pe
care programul le va crea. Programul va imparti intervalul [A, B] in N intervale egale.
Apoi programul va crea N threaduri. Fiecare thread va procesa cate un subinterval din
intervalul [A,B] si va identifica numere prime din subinterval. In momentul in care un thread
va identifica cate un numar prim il va printa la iesirea standard.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct{
    int total_numbers;
    int number_threads;
    int start;
    int end;
    int rank;
}Threads_prime;

int is_Prime(int number){
    if (number < 2){
        return 0;
    }

    if (number == 2){
        return 1;
    }

    for (int i = 2; i <= number/2; i++){
        if (number % i == 0){
            return 0;
        }
    }

    return 1;
}

void *ThreadsPrimeProcess(void *arg){
    Threads_prime *data = (Threads_prime*)arg;

    data->start = data->rank * (data->total_numbers / data->number_threads);
    data->end = (data->rank + 1) * (data->total_numbers / data->number_threads) - 1;

    for (int i = data->start; i <= data->end; i++){
        if (is_Prime(i)){
            printf("Thread %d -> number %d\n",data->rank,i);
        }
    }

    return NULL;
}

int main(int argc, char *argv[]){
    if(argc != 4){
        fprintf(stderr,"Usage: %s A B N_threads",argv[0]);
        exit(EXIT_FAILURE);
    }

    int A = atoi(argv[1]);
    int B = atoi(argv[2]);
    int N_threads = atoi(argv[3]);

    pthread_t threads[N_threads];
    Threads_prime *data = (Threads_prime*)malloc(sizeof(Threads_prime) * N_threads);
    if (data == NULL){
        fprintf(stderr,"Error allocating thread array\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < N_threads; i++){
        data[i].total_numbers = B - A;
        data[i].number_threads = N_threads;
        data[i].rank = i;
        pthread_create(&threads[i],NULL,ThreadsPrimeProcess,&data[i]);
    }

    for (int i = 0; i < N_threads; i++){
        pthread_join(threads[i],NULL);
    }

    free(data);
    return 0;
}
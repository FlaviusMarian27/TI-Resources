/*
2. Sa se scrie un program care primeste ca si argument un caracter si citeste cate o linie de la intrarea standard.
Pentru fiecare linie citita programul va crea un thread care va numara de cate ori caracterul dat ca si argument se
regaseste in linia citita si va printa acest numar la iesirea standard.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define LINE_SIZE 1024

typedef struct{
    char lines[LINE_SIZE];
    char ch;
    int count;
    int rank;
}ThreadData_t;

void *threadProcess(void *arg){
    ThreadData_t *data = (ThreadData_t *)arg;

    for (int i = 0; i < strlen(data->lines); i++){
        if (data->lines[i] == data->ch){
            data->count++;
        }
    }

    printf("Thread %d found %d characters\n", data->rank, data->count);

    free(data);
    return NULL;
}

int main(int argc, char *argv[]){
    if (argc != 2){
        fprintf(stderr, "Usage: %s <character>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char ch = argv[1][0];
    char buffer[LINE_SIZE];
    int rank = 0;

    //printf("%c\n",data->ch);

    while (fgets(buffer, LINE_SIZE, stdin) != NULL){
        ThreadData_t *data = (ThreadData_t *)malloc(sizeof(ThreadData_t));
        if (data == NULL){
            fprintf(stderr, "Out of memory\n");
            exit(EXIT_FAILURE);
        }

        strcpy(data->lines, buffer);
        data->ch = ch;
        data->rank = rank;
        data->count = 0;

        pthread_t thread;
        if (pthread_create(&thread, NULL, threadProcess, (void *)data) != 0){
            fprintf(stderr, "Error creating thread\n");
            exit(EXIT_FAILURE);
        }

        pthread_join(thread, NULL);

        rank++;
    }

    return 0;
}
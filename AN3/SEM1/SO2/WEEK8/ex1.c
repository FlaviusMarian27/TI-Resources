/*
2. Sa se scrie un program care primeste ca si argument un caracter si citeste cate o
linie de la intrarea standard. Pentru fiecare linie citita programul va crea un thread
care va numara de cate ori caracterul dat ca si argument se regaseste in linia citita
si va printa acest numar la iesirea standard.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define MAX_SIZE_BUFFER 1024

typedef struct
{
    char buffer[MAX_SIZE_BUFFER];
    int count;
    char character;
    int rank;
}Thread_data;

int count_character(char str[], char ch){
    int counter = 0;
    for (int i = 0; i < strlen(str); i++){
        if (str[i] == ch){
            counter++;
        }
    }

    return counter;
}

void *procesareThread(void *arg){
    Thread_data data = *(Thread_data *)arg;
    data.count = count_character(data.buffer,data.character);

    printf("Thread %d processed %d characters.\n",data.rank,data.count);

    return NULL;
}

int main(int argc, char *argv[]){
    if (argc != 2){
        fprintf(stderr, "Usage: %s character\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    Thread_data data;

    data.character = argv[1][0];
    data.count = 0;
    data.rank = 0;
    int rank = 0;

    while (fgets(data.buffer,MAX_SIZE_BUFFER,stdin) != NULL){
        data.rank = rank;

        pthread_t thread;
        if (pthread_create(&thread,NULL,procesareThread,(void *)&data) != 0){
            fprintf(stderr,"Error creating thread\n");
            exit(EXIT_FAILURE);
        }

        if (pthread_join(thread,NULL) != 0){
            fprintf(stderr,"Error joining thread\n");
            exit(EXIT_FAILURE);
        }

        rank = rank + 1;
    }

    return 0;
}
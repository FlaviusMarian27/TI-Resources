/*
7. Sa se scrie un program care primeste ca si argument un fisier text si un numar N reprezentat un numar de thread-uri.
Programul va imparti fisierul in N partitii egale si pentru fiecare partitie va lansa un thread care va realiza
histograma caracterelor din fisier. Thread-urile vor completa rezultatul intr-un tablou comun de histrograma.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#define MAX_SIZE 4096

pthread_mutex_t mutex;
int histogram[256];
int total_characters = 0;

typedef struct
{
    char filename[128];
    int rank;
    size_t start;
    size_t end;
}ThreadData_t;

void *processThread(void *data){
    ThreadData_t *threadData = (ThreadData_t *)data;

    int fd = open(threadData->filename, O_RDONLY);
    if (fd < 0){
        fprintf(stderr, "Error opening file %s\n", threadData->filename);
        exit(EXIT_FAILURE);
    }

    lseek(fd, threadData->start, SEEK_SET);

    unsigned char buffer[MAX_SIZE];
    ssize_t remaining = threadData->end - threadData->start + 1;
    int local_histogram[256] = {0};
    int local_total_characters = 0;

    while (remaining > 0){
        ssize_t to_read;
        if (remaining > MAX_SIZE){
            to_read = remaining;
        }else{
            to_read = MAX_SIZE;
        }

        ssize_t readBytes = read(fd, buffer, to_read);
        if (readBytes < 0){
            fprintf(stderr, "Error reading from file %s\n", threadData->filename);
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < readBytes; i++){
            local_histogram[buffer[i]]++;
            local_total_characters += 1;
        }

        remaining -= readBytes;
    }

    if (close(fd) != 0){
        fprintf(stderr, "Error closing file %s\n", threadData->filename);
        exit(EXIT_FAILURE);
    }


    if (pthread_mutex_lock(&mutex) != 0){
        fprintf(stderr, "Error locking mutex\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 256; i++){
        histogram[i] += local_histogram[i];
    }
    total_characters += local_total_characters;

    if (pthread_mutex_unlock(&mutex) != 0){
        fprintf(stderr, "Error unlocking mutex\n");
        exit(EXIT_FAILURE);
    }

    free(threadData);
    return NULL;
}

int main(int argc, char *argv[]){
    if (argc != 3){
        fprintf(stderr,"Usage: %s <file> <numbers_threads>\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    pthread_mutex_init(&mutex, NULL);

    struct stat st;
    if (stat(argv[1], &st) < 0){
        fprintf(stderr, "Error opening file %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    int total_threads = atoi(argv[2]);

    size_t file_size = st.st_size;
    size_t per_thread = file_size / total_threads;


    pthread_t threads[total_threads];
    for (int i = 0; i < total_threads; i++){
        ThreadData_t *threadData = malloc(sizeof(ThreadData_t));
        if ( threadData == NULL){
            fprintf(stderr, "Error allocating thread data\n");
            exit(EXIT_FAILURE);
        }

        strcpy(threadData->filename, argv[1]);
        threadData->rank = i;

        threadData->start = i * per_thread;
        if (i == total_threads - 1){
            threadData->end = file_size - 1;
        }else{
            threadData->end = (i + 1) * per_thread - 1;
        }

        pthread_create(&threads[i], NULL, processThread, threadData);
    }

    for (int i = 0; i < total_threads; i++){
        pthread_join(threads[i], NULL);
    }

    for(int i = 0; i < 256; i++){
        if(histogram[i] > 0){
            //dprintf(fd, "%c: %d\n",(unsigned char)i,histogram[i]);
            double pct = (double) histogram[i] * 100/ total_characters;
            printf("%c : %d (%.2f%%)\n", (unsigned char)i, histogram[i], pct);
        }
    }

    pthread_mutex_destroy(&mutex);

    return 0;
}
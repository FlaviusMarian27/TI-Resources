/*
6. Sa se scrie un program care primeste ca si argument un director si o cale catre un fisier de iesire. Programul va
scana recursiv directorul dat ca si argument si pentru fiecare fisier regular gasit va crea un thread care va realiza
o histograma a caracterelor intalnite. Toate thread-urile vor realiza histograma intr-o zona de memorie comuna (un tablou
de histograma comun). La sfarsit, programul va scrie histograma finala in fisierul identificat prin calea data ca si al
doilea argument al programului.

Pentru un caz de test, se poate descărca o arhivă rulând în terminal:
wget https://staff.cs.upt.ro/~valy/so/test_so_3_1.tar.gz

Pentru dezarhivare se poate folosi următoarea comandă:
tar xf test_so_3_1.tar.gz
*/
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUFFER_SIZE 4096

pthread_mutex_t mutex;
int histogram[256];
int total_characters = 0;

typedef struct{
    char filepath[BUFFER_SIZE];
}ThreadData_t;

void *processThread(void *arg){
    ThreadData_t *thread_data = (ThreadData_t *)arg;

    int fd = open(thread_data->filepath, O_RDONLY);
    if (fd < 0){
        fprintf(stderr, "Error opening %s\n", thread_data->filepath);
        free(thread_data);
        exit(EXIT_FAILURE);
    }

    int local_histogram[256] = {0};
    int local_total_characters = 0;

    unsigned char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0){
        for(int i = 0; i < bytes_read; i++){
            unsigned char ch = buffer[i];
            local_histogram[ch]++;
            local_total_characters = local_total_characters + 1;
        }
    }

    if (close(fd) != 0){
        fprintf(stderr, "Error closing %s\n", thread_data->filepath);
        exit(EXIT_FAILURE);
    }

    pthread_mutex_lock(&mutex);
    for(int i = 0; i < 256; i++){
        histogram[i] = histogram[i] + local_histogram[i];
    }
    total_characters += local_total_characters;
    pthread_mutex_unlock(&mutex);
    free(thread_data);
    return NULL;
}

void readDirectory(const char *directory, pthread_t *threads, int *thread_count){
    DIR *dir = opendir(directory);
    if(dir == NULL){
        fprintf(stderr, "Error opening directory %s\n", directory);
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    while((entry = readdir(dir)) != NULL){
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){
            continue;
        }

        char filepath[BUFFER_SIZE];
        snprintf(filepath, BUFFER_SIZE, "%s/%s", directory, entry->d_name);

        struct stat st;
        if(lstat(filepath, &st) < 0){
            fprintf(stderr, "Error stating directory %s\n", directory);
            exit(EXIT_FAILURE);
        }

        if(S_ISDIR(st.st_mode)){
            readDirectory(filepath,threads,thread_count);
        }else if(S_ISREG(st.st_mode)){
            //printf("%s\n", filepath);
            ThreadData_t *thread_data = (ThreadData_t*)malloc(sizeof(ThreadData_t));
            if(thread_data == NULL){
                fprintf(stderr, "Error allocating memory for thread data\n");
                exit(EXIT_FAILURE);
            }

            strcpy(thread_data->filepath, filepath);
            pthread_create(&threads[*thread_count],NULL,processThread,thread_data);
            (*thread_count)++;
        }
    }

    if (closedir(dir) != 0){
        fprintf(stderr, "Error closing directory %s\n", directory);
        exit(EXIT_FAILURE);
    }
}

int countThreadDirectories(const char *directory){
    DIR *dir = opendir(directory);
    if(dir == NULL){
        fprintf(stderr, "Error opening directory %s\n", directory);
        exit(EXIT_FAILURE);
    }

    int count = 0;
    struct dirent *entry;
    while((entry = readdir(dir)) != NULL){
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){
            continue;
        }

        char filepath[BUFFER_SIZE];
        snprintf(filepath, BUFFER_SIZE, "%s/%s", directory, entry->d_name);

        struct stat st;
        if(lstat(filepath, &st) < 0){
            fprintf(stderr, "Error stating directory %s\n", directory);
            exit(EXIT_FAILURE);
        }

        if(S_ISDIR(st.st_mode)){
            count = count + countThreadDirectories(filepath);
        }else if(S_ISREG(st.st_mode)){
            count = count + 1;
        }
    }

    if (closedir(dir) != 0){
        fprintf(stderr, "Error closing directory %s\n", directory);
        exit(EXIT_FAILURE);
    }

    return count;
}

int main(int argc, char *argv[]){
    if (argc != 3){
        fprintf(stderr, "Usage: %s <directory> <file_out>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    pthread_mutex_init(&mutex, NULL);

    //printf("%d\n", countThreadDirectories(argv[1]));
    int max_threads = countThreadDirectories(argv[1]);
    pthread_t *threads = malloc(sizeof(pthread_t) * (max_threads + 1));
    if(threads == NULL){
        fprintf(stderr, "Error allocating memory for array threads\n");
        exit(EXIT_FAILURE);
    }

    int thread_count = 0;
    readDirectory(argv[1],threads,&thread_count);
    for(int i = 0; i < thread_count; i++){
        pthread_join(threads[i],NULL);
    }

    int fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(fd < 0){
        fprintf(stderr, "Error opening output file %s\n", argv[2]);
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < 256; i++){
        if(histogram[i] > 0){
            //dprintf(fd, "%c: %d\n",(unsigned char)i,histogram[i]);
            double pct = (double) histogram[i] * 100/ total_characters;
            dprintf(fd, "'%c' : %d (%.2f%%)\n", i, histogram[i], pct);
        }
    }

    if (close(fd) != 0){
        fprintf(stderr, "Error closing output file %s\n", argv[2]);
        exit(EXIT_FAILURE);
    }

    free(threads);
    pthread_mutex_destroy(&mutex);

    return 0;
}
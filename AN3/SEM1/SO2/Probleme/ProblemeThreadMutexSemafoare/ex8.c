/*
8. Să se scrie un program care primește ca și argument un fisier text si un numar N reprezentand un numar de threaduri.
Programul va citi din fisier cate un buffer de dimensiunea CHUNK (configurabila printr-un define) si il va da spre
procesare cate unui thread. Daca nu mai sunt thread-uri disponibile din cele N programul va astepta pana cand exista
vreun thread disponibil pentru a prelucra urmatori buffer disponibil. Toate thread-urile vor completa rezultatul intr-un
tablou comun de histograma. Este necesar ca programul sa tina evidenta starii de executie si de join a fiecarui thread.
Numarul N nu va fi ales ca si la problema 7 astfel incat fisierul sa fie impartit in partitii egale. Se va considera ca
exista un pool de N thread-uri care se va ocupa de procesare. Daca nu exista thread-uri disponibile din pool (toate sunt
ocupate cu procesarea unui buffer) atunci programul va astepta eliberarea unui thread. Se poate utiliza o functia care
sa obtina care thread este liber din pool-ul de thread-uri. Se va tine evidenta starii de join si se va face join pe
fiecare thread care isi termina executia
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>

#define CHUNCK 4096
#define MAX_THREADS 16
#define TOTAL_CHARACTERS 256

pthread_mutex_t mutex;
int histogram[TOTAL_CHARACTERS];

pthread_t threads[MAX_THREADS];
int thread_is_busy[MAX_THREADS];

typedef struct{
    unsigned char *buffer;
    ssize_t size;
}ThreadData_t;

void *processThread(void *arg){
    ThreadData_t *data = (ThreadData_t *)arg;

    int local_histogram[TOTAL_CHARACTERS] = {0};

    for(int i = 0; i < data->size; i++){
        local_histogram[data->buffer[i]]++;
    }

    if (pthread_mutex_lock(&mutex) != 0){
        fprintf(stderr,"mutex lock failed\n");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < TOTAL_CHARACTERS; i++){
        histogram[i] = histogram[i] + local_histogram[i];
    }

    if (pthread_mutex_unlock(&mutex) != 0){
        fprintf(stderr,"mutex unlock failed\n");
        exit(EXIT_FAILURE);
    }

    free(data->buffer);
    free(data);

    return NULL;
}

int is_busy(int number_threads){
    for(int i = 0; i < number_threads; i++){
        if (thread_is_busy[i] == 0){
            return i;
        }
    }

    return -1;
}

void processFile(const char *file_name, int number_threads){
    int fd = open(file_name, O_RDONLY);
    if (fd < 0){
        fprintf(stderr,"file open failed\n");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < number_threads; i++){
        thread_is_busy[i] = 0;
    }

    while (1){
        unsigned char *buffer = (unsigned char *) malloc(CHUNCK);
        if (buffer == NULL){
            fprintf(stderr,"malloc failed\n");
            exit(EXIT_FAILURE);
        }

        ssize_t bytes_read = read(fd, buffer, CHUNCK);
        if (bytes_read < 0){
            fprintf(stderr,"read failed\n");
            free(buffer);
            exit(EXIT_FAILURE);
        }

        if (bytes_read == 0){
            free(buffer);
            break;
        }

        ThreadData_t *data = (ThreadData_t *) malloc(sizeof(ThreadData_t));
        if (data == NULL){
            fprintf(stderr,"malloc failed\n");
            exit(EXIT_FAILURE);
        }

        data->buffer = buffer;
        data->size = bytes_read;

        int idx;
        while ((idx = is_busy(number_threads)) == -1){
            for (int i = 0; i < number_threads; i++){
                if (thread_is_busy[i]){
                    pthread_join(threads[i], NULL);
                    thread_is_busy[i] = 0;
                }
            }
        }

        pthread_create(&threads[idx], NULL, processThread, data);
        thread_is_busy[idx] = 1;
    }

    if (close(fd) != 0){
        fprintf(stderr,"close failed\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < number_threads; i++){
        if (thread_is_busy[i]){
            pthread_join(threads[i], NULL);
        }
    }
}

int main(int argc, char *argv[]){
    if (argc != 3){
        fprintf(stderr, "Usage: ./ex8 <file> <number of threads>\n");
        exit(EXIT_FAILURE);
    }

    int N = atoi(argv[2]);
    if (N <= 0 || N > MAX_THREADS) {
        fprintf(stderr, "Invalid thread count\n");
        exit(1);
    }

    pthread_mutex_init(&mutex, NULL);

    processFile(argv[1], N);

    pthread_mutex_destroy(&mutex);

    for (int i = 0; i < 256; i++){
        if (histogram[i] > 0){
            printf("%c : %d\n", i, histogram[i]);
        }
    }

    return 0;
}
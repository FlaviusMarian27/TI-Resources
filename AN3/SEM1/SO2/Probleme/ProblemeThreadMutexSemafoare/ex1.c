/*
1. Să se scrie un program C ce se va apela în linie de comandă astfel:
./prog <directory> <output_file> <threshold> <ch1> <ch2> .. <chn>

Programul va parcurge recursiv directorul dat ca și argument.
Pentru fiecare fișier obișnuit (regular) găsit se va crea un thread ce va număra de câte ori caracterele date ca și
argumente apar în conținutul acestuia. Dacă numărul total de caractere numărate pentru fiecare fișier depășește numărul
<threshold>, programul va crea o legătură simbolică în același director cu fișierul procesat, cu același nume, dar cu
terminația "_th". Se va crea de asemenea și un fisier de statistică referit prin <output_file>. Fișierul va avea o formă
tabelară, iar capul de tabel va fi generat dinamic în funcție de argumente, astfel:
forma generala:
<file_path>;<nr_ch1>;<nr_ch2>;...;<nr_chn> <total>

Pentru un caz de test, se poate descărca o arhivă rulând în terminal:
wget https://staff.cs.upt.ro/~valy/so/test_so_3_1.tar.gz

Pentru dezarhivare se poate folosi următoarea comandă:
tar xf test_so_3_1.tar.gz
*/

#include <ctype.h>
#include<stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_SIZE_BUFFER 1024
#define MAX_READ_BUFFER 4096
#define MAX_THREADS     20

typedef struct{
    char filepath[MAX_SIZE_BUFFER]; //fisierului cautat
    char *chars; //lista de caractere cautate
    int n_chars; //cate caractere cautam
    int *counts; //vectorul de frecventa pt fiecare caracter
}Thread_data_t;

void *process_field(void *arg){
    Thread_data_t *data = (Thread_data_t *)arg;

    int fd = open(data->filepath, O_RDONLY);
    if (fd < 0){
        fprintf(stderr, "Error opening file %s\n", data->filepath);
        exit(EXIT_FAILURE);
    }

    char buffer[MAX_READ_BUFFER];
    ssize_t bytes_read;
    while ((bytes_read = read(fd, buffer, MAX_READ_BUFFER)) > 0){
        for (int i = 0; i < bytes_read; i++){
            for (int c = 0; c < data->n_chars; c++){
                if (buffer[i] == data->chars[c]){
                    data->counts[c]++;
                }
            }
        }
    }

    if (close(fd) != 0){
        fprintf(stderr, "Error closing file %s\n", data->filepath);
        exit(EXIT_FAILURE);
    }

    pthread_exit(NULL);
}

void readFromDirectory(const char* directory_name, pthread_t *threads, Thread_data_t *tdata,
                    int *thread_count,char *chars, int n_chars){

    DIR *dir = opendir(directory_name);
    if(dir == NULL){
        fprintf(stderr, "Error opening directory %s\n", directory_name);
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    while((entry = readdir(dir)) != NULL){
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){
            continue;
        }

        char filename[MAX_SIZE_BUFFER];
        snprintf(filename, MAX_SIZE_BUFFER, "%s/%s", directory_name, entry->d_name);

        struct stat st;
        if (lstat(filename, &st) < 0){
            fprintf(stderr, "Error stating file %s\n", filename);
            exit(EXIT_FAILURE);
        }

        if(S_ISDIR(st.st_mode)){
            readFromDirectory(filename, threads, tdata, thread_count, chars, n_chars);
        }else if(S_ISREG(st.st_mode)){
            //printf("%s\n", filename);
            strcpy(tdata[*thread_count].filepath, filename);
            tdata[*thread_count].chars = chars;
            tdata[*thread_count].n_chars = n_chars;
            tdata[*thread_count].counts = calloc(n_chars, sizeof(int));
            if (tdata[*thread_count].counts == NULL){
                fprintf(stderr, "Error allocating memory for thread data\n");
                exit(EXIT_FAILURE);
            }

            pthread_create(&threads[*thread_count], NULL, &process_field, &tdata[*thread_count]);
            (*thread_count)++;
        }
    }

    if (closedir(dir) != 0){
        fprintf(stderr, "Error closing directory %s\n", directory_name);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char const *argv[]){
    if (argc < 5){
        fprintf(stderr, "/%s <directory> <output_file> <threshold> <ch1> <ch2> .. <chn>\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    pthread_t threads[MAX_THREADS];
    Thread_data_t tdata[MAX_THREADS];
    int thread_count = 0;

    //char *directory_name = argv[1];
    //char *output_file = argv[2];
    int threshold = atoi(argv[3]);

    int n_chars = argc - 4;
    char *chars = (char*)malloc(n_chars * sizeof(char));
    if (chars == NULL){
        fprintf(stderr, "Error allocating memory for chars\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n_chars; i++){
        chars[i] = argv[4 + i][0];
    }

    readFromDirectory(argv[1], threads, tdata, &thread_count, chars, n_chars);

    for (int i = 0; i < thread_count; i++){
        pthread_join(threads[i], NULL);
    }

    int fd_out = open(argv[2], O_CREAT | O_TRUNC | O_WRONLY | O_APPEND, 0644);
    if (fd_out < 0){
        fprintf(stderr, "Error opening output file %s\n", argv[2]);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < thread_count; i++){
        int total = 0;

        dprintf(fd_out, "%s\n", tdata[i].filepath);

        for (int j = 0; j < tdata[i].n_chars; j++){
            dprintf(fd_out, "%d ", tdata[i].counts[j]);
            total += tdata[i].counts[j];
        }

        dprintf(fd_out, "%d total\n",total);

        if (total > threshold){
            char linkname[1025];
            snprintf(linkname, sizeof(linkname), "%s", tdata[i].filepath);
            strcat(linkname, "_th");

            unlink(linkname);
            if (symlink(tdata[i].filepath, linkname) < 0){
                fprintf(stderr, "Error creating link %s\n", tdata[i].filepath);
                exit(EXIT_FAILURE);
            }
        }

        free(tdata[i].counts);
    }

    if (close(fd_out) != 0){
        fprintf(stderr, "Error closing output file %s\n", argv[2]);
        exit(EXIT_FAILURE);
    }

    free(chars);

    return 0;
}
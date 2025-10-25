/*
1. Să se scrie un program C care primește ca și argument în linie de comandă calea către un director de pe disc.
Programul va parcurge directorul primit ca și argument, va procesa în paralel conținutul tuturor fișierelor obișnuite identificate și va calcula numărul total de litere mici din acestea. 
INDICIU: se va crea câte un proces copil pentru fiecare fișier obișnuit identificat și acestea vor apela exit având ca și parametru numărul de litere mici pentru fișierul curent.
Părintele va prelua informația aceasta folosind wait, WIFEXITED și WEXITSTATUS.
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define BUFFER_SIZE 4096
#define FILE_SIZE 1024

int count_characters(const char* file_path){
    int count = 0;

    int fd = open(file_path, O_RDONLY);
    if (fd < 0){
        fprintf(stderr, "Error opening file: %s\n", file_path);
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0){
        for (int i = 0; i < bytes_read; i++){
            if (islower(buffer[i])){
                count++;
            }
        }
    }

    if (close(fd) != 0){
        fprintf(stderr, "Error closing file: %s\n", file_path);
        exit(EXIT_FAILURE);
    }

    return count;
}

void readFromDirectory(const char* directory_path){
    DIR* directory = opendir(directory_path);
    if (directory == NULL){
        fprintf(stderr, "Error opening directory: %s\n", directory_path);
        exit(EXIT_FAILURE);
    }

    struct dirent* entry;
    while ((entry = readdir(directory)) != NULL){
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){
            continue;
        }

        char file_path[BUFFER_SIZE];
        snprintf(file_path, BUFFER_SIZE, "%s/%s", directory_path, entry->d_name);

        struct stat st;
        if (stat(file_path, &st) < 0){
            fprintf(stderr, "Error stating file: %s\n", file_path);
            exit(EXIT_FAILURE);
        }

        if (S_ISDIR(st.st_mode)){
            readFromDirectory(file_path);
        }else if (S_ISREG(st.st_mode)){
            pid_t pid = fork();

            if (pid < 0){
                fprintf(stderr, "Error forking process\n");
                exit(EXIT_FAILURE);
            }

            if (pid == 0){
                int count = count_characters(file_path);
                fprintf(stdout, "%d characters read\n", count);
                exit(count);
            }
        }
    }

    if (closedir(directory) != 0){
        fprintf(stderr, "Error closing directory: %s\n", directory_path);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]){
    if (argc != 2){
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    readFromDirectory(argv[1]);
    //printf("%d\n", count_characters(argv[1]));

    int status;
    int total = 0;
    while (wait(&status) > 0){
        if (WIFEXITED(status)){
            total = total + WEXITSTATUS(status);
        }
    }

    printf("Total: %d\n", total);
    return 0;
}

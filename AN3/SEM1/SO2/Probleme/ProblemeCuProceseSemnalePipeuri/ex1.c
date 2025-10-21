/*
1. Să se scrie un program C care primește ca și argument în linie de comandă calea 
către un director de pe disc. Programul va parcurge directorul primit ca și argument, 
va procesa în paralel conținutul tuturor fișierelor obișnuite identificate și va 
calcula numărul total de litere mici din acestea. 
INDICIU: se va crea câte un proces copil pentru fiecare fișier obișnuit identificat 
și acestea vor apela exit având ca și parametru numărul de litere mici pentru fișierul 
curent.
Părintele va prelua informația aceasta folosind wait, WIFEXITED și WEXITSTATUS.
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>

#define THE_WAY_PATH_SIZE 1024
#define MAX_SIZE_BUFFER 4096

int count_how_many_characters(const char *path){
    int count = 0;
    int fd = open(path, O_RDONLY);
    if (fd < 0){
        fprintf(stderr, "Error opening file %s\n", path);
        exit(EXIT_FAILURE);
    }

    char buffer[MAX_SIZE_BUFFER];
    ssize_t bytes_read;
    while ((bytes_read = read(fd, buffer, MAX_SIZE_BUFFER)) > 0){
        for (int i = 0; i < bytes_read; i = i + 1){
            if (islower(buffer[i])){
                count = count + 1;
            }
        }
    }

    if (close(fd) != 0){
        fprintf(stderr, "Error closing file %s\n", path);
        exit(EXIT_FAILURE);
    }

    return count;
}

void readFromDirectory(const char* directory_path){
    DIR* directory = opendir(directory_path);
    if(directory == NULL){
        fprintf(stderr,"Error opening directory\n");
        exit(EXIT_FAILURE);
    }

    struct dirent* entry;
    char file_name[THE_WAY_PATH_SIZE];
    while((entry = readdir(directory)) != NULL){
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){
            continue;
        }

        snprintf(file_name, THE_WAY_PATH_SIZE, "%s/%s", directory_path, entry->d_name);
        struct stat st;
        if(stat(file_name, &st) < 0){
            fprintf(stderr, "Error stat\n");
            exit(EXIT_FAILURE);
        }

        if(S_ISDIR(st.st_mode)){
            readFromDirectory(file_name);
        }else if(S_ISREG(st.st_mode)){
            pid_t pid;
            if((pid = fork()) < 0){
                fprintf(stderr, "Error fork\n");
                exit(EXIT_FAILURE);
            }

            if (pid == 0){
                int count_characters = count_how_many_characters(file_name);
                printf("Fisierul %s are %d litere mici!\n",file_name,count_characters);
                exit(count_characters % 256);
            }
        }
    }

    if (closedir(directory) < 0){
        fprintf(stderr,"Error closing directory\n");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]){
    if (argc != 2){
        fprintf(stderr, "Usage: %s <directory_path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    readFromDirectory(argv[1]);

    int total = 0;
    int status;
    while (wait(&status) > 0){
        if (WIFEXITED(status)){
            total = total + WEXITSTATUS(status);
        }
    }


    printf("Total litere mici: %d\n", total);
    return 0;
}
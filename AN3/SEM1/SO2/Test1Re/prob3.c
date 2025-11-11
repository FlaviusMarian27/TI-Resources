/*
Scrie un program C care primește o cale către un director:

./program <cale_director>

Programul parcurge recursiv directorul și pentru fiecare fișier obișnuit:
creează un proces copil care execută wc <fisier> cu execlp;
redirecționează ieșirea standard a copilului într-un pipe comun;
Parintele va afisa la iesirea standard. AI varianta cu write dar mai usor cu fdopen!!!! Vezi prob 4
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <ctype.h>
#include <string.h>
#include <sys/wait.h>

#define BUF_SIZE 1024
#define MAX_SIZE 4096

void process(const char* path, int write_fd){
    if (dup2(write_fd,1) < 0){
        fprintf(stderr, "Error dup2\n");
        exit(EXIT_FAILURE);
    }

    execlp("wc", "wc", path, NULL);
    perror("execlp");
    exit(EXIT_FAILURE);
}

void readFromDirectory(const char* dirName, int read_fd, int write_fd){
    DIR *dir = opendir(dirName);
    if (dir == NULL){
        fprintf(stderr, "Error opening directory %s\n", dirName);
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL){
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){
            continue;
        }

        char fileName[MAX_SIZE];
        sprintf(fileName, "%s/%s", dirName, entry->d_name);

        struct stat fileStat;
        if (stat(fileName, &fileStat) < 0){
            fprintf(stderr, "Error stat file %s\n", fileName);
            exit(EXIT_FAILURE);
        }

        if (S_ISDIR(fileStat.st_mode)){
            readFromDirectory(fileName, read_fd, write_fd);
        }else if (S_ISREG(fileStat.st_mode)){
            //printf("%s\n", fileName);
            pid_t pid = fork();
            if (pid < 0){
                fprintf(stderr, "Error fork\n");
                exit(EXIT_FAILURE);
            }

            if (pid == 0){
                close(read_fd);
                process(fileName, write_fd);
                close(write_fd);
                exit(0);
            }
        }
    }

    if (closedir(dir) != 0){
        fprintf(stderr, "Error closing directory %s\n", dirName);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]){
    if (argc != 2){
        fprintf(stderr, "Usage: program filename\n");
        exit(EXIT_FAILURE);
    }

    int pipefd[2];
    if (pipe(pipefd) < 0){
        fprintf(stderr, "Error creating pipe\n");
        exit(EXIT_FAILURE);
    }

    readFromDirectory(argv[1], pipefd[0], pipefd[1]);

    close(pipefd[1]);

    char buffer[BUF_SIZE];
    ssize_t bytesRead;
    while ((bytesRead = read(pipefd[0], buffer, BUF_SIZE)) > 0){
        write(1, buffer, bytesRead);
    }

    close(pipefd[0]);
    int status;
    while (wait(&status) > 0){
        if (WIFEXITED(status)){
            printf("Child exited with status %d\n", WEXITSTATUS(status));
        }
    }

    return 0;
}
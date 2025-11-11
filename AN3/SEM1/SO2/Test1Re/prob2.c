/*
 Se da un director ca si argument in linie de comanda. Se cere sa se creeza cate un proces fiu
 pentru fiecare fisier regulat care sa numere independet cate litere mici are fiecare fisier si
 sa trimita prin pipe catre parinte. Parintele va afisare totalul!
 */

#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define MAX_SIZE 4096

void processFile(const char *path, int write_fd){
    int fd = open(path, O_RDONLY);
    if(fd < 0){
        fprintf(stderr,"Error opening file %s\n",path);
        exit(EXIT_FAILURE);
    }

    char buffer[MAX_SIZE];
    ssize_t bytesRead;
    int lower_case = 0;
    while((bytesRead = read(fd, buffer, MAX_SIZE)) > 0){
        for(int i = 0; i < bytesRead; i++){
            if (islower(buffer[i])){
                lower_case = lower_case + 1;
            }
        }
    }

    if (close(fd) != 0){
        fprintf(stderr,"Error closing file %s\n",path);
    }

    FILE *fin = fdopen(write_fd, "w");
    if(fin == NULL){
        fprintf(stderr,"Error opening file %s\n",path);
        exit(EXIT_FAILURE);
    }

    fprintf(fin, "%s %d\n", path, lower_case);

    if (fclose(fin) != 0){
        fprintf(stderr,"Error closing file %s\n",path);
        exit(EXIT_FAILURE);
    }
}

void readFromDirectory(const char *dirName, int read_fd, int write_fd){
    DIR *dir = opendir(dirName);
    if(dir == NULL){
        fprintf(stderr,"Error opening directory %s\n",dirName);
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    while((entry = readdir(dir)) != NULL){
        if(strcmp(entry->d_name,".")==0 || strcmp(entry->d_name,"..")==0){
            continue;
        }

        char fileName[MAX_SIZE];
        snprintf(fileName, MAX_SIZE, "%s/%s", dirName, entry->d_name);

        struct stat fileStat;
        if(stat(fileName, &fileStat) < 0){
            fprintf(stderr,"Error reading file %s\n",fileName);
            exit(EXIT_FAILURE);
        }

        if(S_ISDIR(fileStat.st_mode)){
            readFromDirectory(fileName, read_fd, write_fd);
        }else if(S_ISREG(fileStat.st_mode)){
            pid_t pid = fork();
            if(pid < 0){
                fprintf(stderr,"Error forking child process\n");
                exit(EXIT_FAILURE);
            }

            if(pid == 0){
                close(read_fd);
                processFile(fileName, write_fd);
                close(write_fd);
                exit(0);
            }
        }
    }

    if (closedir(dir) != 0){
        fprintf(stderr,"Error closing directory %s\n",dirName);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]){
    if (argc != 2){
        fprintf(stderr,"Usage: prob2 <directory name>\n");
        exit(EXIT_FAILURE);
    }

    int pipefd[2];
    if(pipe(pipefd) < 0){
        fprintf(stderr,"Error creating pipe\n");
        exit(EXIT_FAILURE);
    }

    readFromDirectory(argv[1], pipefd[0], pipefd[1]);

    close(pipefd[1]);

    int total_lower_case = 0;

    FILE* fin = fdopen(pipefd[0], "r");
    if(fin == NULL){
        fprintf(stderr,"Error opening file %s\n",argv[1]);
        exit(EXIT_FAILURE);
    }

    int l = 0;
    char name_path[MAX_SIZE];
    while (fscanf(fin, "%s %d", name_path, &l) == 2){
        printf("%s -> %d\n", name_path, l);
        total_lower_case = total_lower_case + l;
    }

    if (fclose(fin) != 0){
        fprintf(stderr,"Error closing file %s\n",argv[1]);
        exit(EXIT_FAILURE);
    }

    close(pipefd[0]);
    int status;
    while (wait(&status) > 0){
        if (WIFEXITED(status)){
            printf("Child exited with status %d\n", WEXITSTATUS(status));
        }else{
            printf("Child process exited abnormally\n");
        }
    }

    printf("Total lower case: %d\n",total_lower_case);

    return 0;
}
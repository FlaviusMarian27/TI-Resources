/*
    Implemantare wc manual!!!!
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/wait.h>

#define MAX_SIZE_BUFFER 4096
#define SIZE 1024

void wc_manual(const char* path, int write_fd){
    int fd = open(path, O_RDONLY);
    if(fd < 0){
        fprintf(stderr, "Can't open %s\n", path);
        exit(EXIT_FAILURE);
    }

    char temp_buffer[MAX_SIZE_BUFFER];
    ssize_t bytes_read;
    int total_bytes= 0;
    int total_words = 0;
    int total_lines = 0;
    int in_word = 0;

    while((bytes_read = read(fd, temp_buffer, MAX_SIZE_BUFFER)) > 0){
        total_bytes = total_bytes + bytes_read;
        for(int i = 1; i < bytes_read; i++){
            char character = temp_buffer[i];

            if(character == '\n'){
                total_lines = total_lines + 1;
            }

            if (isspace(character)){
                in_word = 0;
            }else if (in_word == 0){
                in_word = 1;
                total_words = total_words + 1;
            }
        }
    }

    //printf("%d %d %d\n",total_lines,total_words,total_bytes);
    if (close(fd) != 0){
        fprintf(stderr, "Can't close %s\n", path);
        exit(EXIT_FAILURE);
    }

    FILE* fin = fdopen(write_fd, "w");
    if(fin == NULL){
        fprintf(stderr, "Can't fdopen %s\n", path);
        exit(EXIT_FAILURE);
    }

    fprintf(fin, "%d %d %d %s\n", total_lines, total_words, total_bytes, path);

    if (fclose(fin) != 0){
        fprintf(stderr, "Can't fclose %s\n", path);
        exit(EXIT_FAILURE);
    }
}

void readFromDirectory(const char *dirName, int read_fd, int write_fd){
    DIR *dir = opendir(dirName);
    if(dir == NULL){
        fprintf(stderr, "Can't open directory %s\n", dirName);
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    while((entry = readdir(dir)) != NULL){
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){
            continue;
        }

        char filename[SIZE];
        snprintf(filename, MAX_SIZE_BUFFER, "%s/%s", dirName, entry->d_name);

        struct stat st;
        if (lstat(filename, &st) < 0){
            fprintf(stderr, "Can't stat %s\n", filename);
            exit(EXIT_FAILURE);
        }

        if(S_ISDIR(st.st_mode)){
            readFromDirectory(filename,read_fd,write_fd);
        }else if(S_ISREG(st.st_mode)){
            //printf("%s\n",filename);
            pid_t pid = fork();
            if(pid < 0){
                fprintf(stderr, "Can't fork\n");
                exit(EXIT_FAILURE);
            }

            if(pid == 0){
                close(read_fd);
                wc_manual(filename,write_fd);
                close(write_fd);
                exit (EXIT_SUCCESS);
            }
        }
    }

    if (closedir(dir) != 0){
        fprintf(stderr, "Can't close directory %s\n", dirName);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]){
    if(argc != 2){
        fprintf(stderr, "Usage: ./prob5 filename\n");
        exit(EXIT_FAILURE);
    }

    int pipefd[2];
    if(pipe(pipefd) < 0){
        fprintf(stderr, "Can't create pipefd\n");
        exit(EXIT_FAILURE);
    }

    readFromDirectory(argv[1], pipefd[0], pipefd[1]);

    //wc_manual(argv[1], pipefd[1]);
    close(pipefd[1]);

    FILE* fin = fdopen(pipefd[0], "r");
    if (fin == NULL){
        fprintf(stderr, "Can't fdopen %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    int total_lines = 0;
    int total_words = 0;
    int total_bytes = 0;

    int l,w,b;
    char name_File[SIZE];
    while (fscanf(fin,"%d %d %d %s\n",&l,&w,&b,name_File) == 4){
        printf("%d %d %d %s\n",l,w,b,name_File);
        total_lines = total_lines + l;
        total_words = total_words + w;
        total_bytes = total_bytes + b;
    }

    if(fclose(fin) != 0){
        fprintf(stderr, "Can't fclose %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    close(pipefd[0]);
    printf("Total-> New lines: %d Words: %d Bytes: %d\n",total_lines,total_words,total_bytes);

    int status;
    while (wait(&status) > 0){
        if(WIFEXITED(status)){
            printf("Child exited with code %d\n",WEXITSTATUS(status));
        }
    }

    return 0;
}

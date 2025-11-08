/*
6. Să se scrie un program C, care constă în 2 procese înrudite:
procesul copil: va citi în întregime un fișier text primit ca și argument în linie de comandă și va trimite doar vocalele procesului părinte folosind un pipe
procesul părinte: va număra câți octeți a primit prin pipe, va afișa acest număr și după va prelua starea procesului copil.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#define BUF_SIZE 4096

int is_Vowels(char vowels){
    if (strchr("aeiouAEIOU", vowels) != NULL){
        return 1;
    }

    return 0;
}

void readFromPath(const char *path, int capat_scriere){
    int fd = open(path, O_RDONLY);
    if (fd < 0)
    {
        fprintf(stderr, "open() failed\n");
        exit(EXIT_FAILURE);
    }
    char buf[BUF_SIZE];
    ssize_t bytes_read;
    while ((bytes_read = read(fd, buf, BUF_SIZE)) > 0){
        for (int i = 0; i < bytes_read; i++){
            if (is_Vowels(buf[i])){
                write(capat_scriere, &buf[i], 1);
            }
        }
    }

    if (close(fd) < 0){
        fprintf(stderr, "close() failed\n");
        exit(EXIT_FAILURE);
    }
}

int count_bytes(int capat_citire){
    int counter = 0;
    char buf[BUF_SIZE];
    while (read(capat_citire,buf,1) > 0){
        counter++;
    }

    return counter;
}

int main(int argc, char *argv[]){
    if(argc != 2){
        fprintf(stderr, "Usage: ex1 <filename>\n");
        exit(EXIT_FAILURE);
    }

    /*
    char ch = 'a';
    if(is_Vowels(ch))
    {
        printf("este\n");
    }else
    {
        printf("no\n");
    }*/

    int pipefd[2];
    if (pipe(pipefd) < 0){
        fprintf(stderr, "pipe() failed\n");
        exit(EXIT_FAILURE);
    }

    int pid;
    if ((pid = fork()) < 0){
        fprintf(stderr, "fork() failed\n");
        exit(EXIT_FAILURE);
    }

    if (pid == 0){
        close(pipefd[0]);

        readFromPath(argv[1], pipefd[1]);

        close(pipefd[1]);
        exit(0);
    }else{
        close(pipefd[1]);

        printf("counter = %d\n", count_bytes(pipefd[0]));

        close(pipefd[0]);

        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)){
            printf("exit status %d\n", WEXITSTATUS(status));
        }
    }

    return 0;
}
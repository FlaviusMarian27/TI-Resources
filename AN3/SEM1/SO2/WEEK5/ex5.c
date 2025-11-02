/*
7. Să se scrie un program C, care constă în 2 procese înrudite:
procesul copil: va invoca comanda "ls -l" și se va asigura că informația generată 
de această comandă este transmisă părintelui folosind un pipe
procesul părinte: va număra câți octeți a primit prin pipe, va afișa acest număr 
și după va prelua starea procesului copil.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#define BUF_SIZE 4096

int main(int argc, char *argv[]){
    int pfd[2];
    if (pipe(pfd) < 0){
        fprintf(stderr, "pipe() failed\n");
        exit(EXIT_FAILURE);
    }

    pid_t pid;
    pid = fork();
    if (pid < 0){
        fprintf(stderr, "fork() failed\n");
        exit(EXIT_FAILURE);
    }

    if (pid == 0){
        close(pfd[0]);

        if (dup2(pfd[1], STDOUT_FILENO) < 0){
            fprintf(stderr, "dup2() failed\n");
            exit(EXIT_FAILURE);
        }

        close(pfd[1]);
        execlp("ls","ls","-l",NULL);
        perror("execlp() failed");
        exit(EXIT_FAILURE);
    }else{
        close(pfd[1]);

        char buf[BUF_SIZE];
        ssize_t bytesRead;
        int total = 0;

        while ((bytesRead = read(pfd[0], buf, BUF_SIZE)) > 0){
            total = total + bytesRead;
        }

        close(pfd[0]);
        waitpid(pid,NULL,0);

        printf("Parintele a citit %d bytes!\n",total);
    }

    return 0;
}
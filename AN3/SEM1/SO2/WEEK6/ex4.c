/*
Sa se scrie un program C care implementeaza urmatorul pipeline

ls -l | cut -f 1 -d ' '

execlp+dup2+pipe
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
    int pipeFd[2];
    if (pipe(pipeFd) < 0 ){
        fprintf(stderr, "pipe() failed\n");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid < 0 ){
        fprintf(stderr, "fork() failed\n");
        exit(EXIT_FAILURE);
    }

    if (pid == 0){
        close(pipeFd[0]);
        if (dup2(pipeFd[1], STDOUT_FILENO) < 0 ){
            fprintf(stderr, "dup2() failed\n");
            exit(EXIT_FAILURE);
        }
        close(pipeFd[1]);
        execlp("ls", "ls", "-l", NULL);
        perror("execlp() ls failed");
        exit(EXIT_SUCCESS);
    }

    pid_t pid2 = fork();
    if (pid2 < 0 ){
        fprintf(stderr, "fork() failed\n");
        exit(EXIT_FAILURE);
    }

    if (pid2 == 0){
        close(pipeFd[1]);
        if (dup2(pipeFd[0], STDIN_FILENO) < 0 ){
            fprintf(stderr, "dup2() failed\n");
            exit(EXIT_FAILURE);
        }
        close(pipeFd[0]);
        execlp("cut", "cut", "-f", "1","-d"," ", NULL);
        perror("execlp() cut() failed");
        exit(EXIT_SUCCESS);
    }

    close(pipeFd[1]);
    close(pipeFd[0]);

    int status;
    while (wait(&status) > 0){
        if (WIFEXITED(status)){
            printf("Child exited with code %d\n", WEXITSTATUS(status));
        }
    }

    return 0;
}
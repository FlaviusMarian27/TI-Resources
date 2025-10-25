/*
    Sa se creeze 2 procese
    Copilul 1: ls -l
    Copilul 2: ex2
    ex2 - executabilul de la exercitiul 2
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define SIZE_PID 2

int main(void){
    pid_t pid[SIZE_PID];

    for(int i = 0; i < SIZE_PID; i++){
        pid[i] = fork();

        if (pid[i] < 0){
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid[i] == 0){
            if (i == 0){
                execlp("ls","ls","-l",NULL);
                perror("execlp");
                exit(EXIT_FAILURE);
            }else if (i == 1){
                execl("/home/flavius/Documents/SO2-CLION/WEEK4/ex2","ex2",NULL);
                perror("execl");
                exit(EXIT_FAILURE);
            }
        }
    }

    for(int i = 0; i < SIZE_PID; i = i + 1){
        wait(NULL);
    }

    return 0;
}
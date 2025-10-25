/*
Sa se creeze o structura de forma:

        parinte
     /     |     \
  copil1 copil2 copil3
            |
            nepot

se va folosi getpid() si getppid() pt fiecare copil
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void){

    pid_t pid[3];
    for(int i = 0; i < 3; i = i + 1){
        if((pid[i] = fork()) < 0){
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if(pid[i] == 0){
            printf("Child %d: PID=%d, PPID=%d\n", i, getpid(), getppid());
            if (i == 1){
                pid_t nep = fork();
                if (nep < 0){
                    perror("fork");
                    exit(EXIT_FAILURE);
                }

                if (nep == 0){
                    printf("   Nepotul: PID=%d, PPID=%d\n", getpid(), getppid());
                    exit(0);
                }else{
                    wait(NULL);
                }
            }
            exit(0);
        }
    }

    for(int i = 0; i < 3; i = i + 1){
        wait(NULL);
    }

    printf("Success\n");
    return 0;
}
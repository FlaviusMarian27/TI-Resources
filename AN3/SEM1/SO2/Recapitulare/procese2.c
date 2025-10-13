#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>

int main(int argc, char* argv[]){
    pid_t pid = fork();
    if(pid < 0){
        perror("Eroare la fork!\n");
        exit(-1);
    }

    if(pid == 0){
        execlp("ls","ls","-l",NULL);
        perror("Eroare execlp!\n");
        exit(-1);
    }else{
        wait(NULL);
        printf("Parintele a terminat dupa ce fiul a rulat ls.\n");
    }

    return 0;
}
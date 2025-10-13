#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
    pid_t pid = fork();
    if(pid < 0){
        perror("Eroare la fork!\n");
        exit(-1);
    }

    if(pid == 0){
        printf("Fiul ruleaza ...\n");
        sleep(2);
        printf("Fiul s-a terminat\n");
        exit(7);
    }else{
        int status;
        printf("Parintele asteapta fiul...\n");
        wait(&status);
        printf("Fiul s-a terminat cu codul %d\n",WEXITSTATUS(status));
    }

    return 0;
}
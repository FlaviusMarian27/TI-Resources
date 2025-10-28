#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

void MyHandler(int sig){
    printf("Parintele a primit SIGUSR1\n");
}

int main(void){
    struct sigaction sa;
    memset(&sa,0,sizeof(struct sigaction));
    sa.sa_handler = MyHandler;
    sa.sa_flags = 0;

    if(sigaction(SIGUSR1,&sa,NULL) < 0){
        perror("Eroare SIGUSR1!\n");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if(pid < 0){
        perror("Eroare la fork!\n");
        exit(EXIT_FAILURE);
    }

    if(pid == 0){
        printf("Fiul %d da semnalul SIGURS1 catre %d\n",getpid(),getppid());
        sleep(2);
        kill(getppid(),SIGUSR1);
        exit(0);
    }

    pause();
    printf("Parintele iese.\n");

    return 0;
}
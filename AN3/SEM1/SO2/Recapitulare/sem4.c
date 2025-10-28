#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

void MyHandler(int sig){
    if(sig == SIGUSR1){
        printf("Am primit semnalul SIGUSR1!\n");
        printf("La revedere!\n");
        sleep(2);
        exit(0);
    }
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

    printf("PID=%d — trimite semnal cu: kill -USR1 %d\n", getpid(), getpid());

    // in alt terminal pui kill -USR1 PID

    while(1){
        pause();
    }

    return 0;
}
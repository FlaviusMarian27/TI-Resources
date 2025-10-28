#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void Myhandler(int sig){
    printf("Am primit SIGTERM!. Vom opri!\n");
    sleep(1);
    exit(0);
}

int main(void){
    struct sigaction sa;
    memset(&sa,0,sizeof(struct sigaction));
    sa.sa_handler = Myhandler;
    sa.sa_flags = 0;

    if(sigaction(SIGTERM,&sa,NULL) < 0){
        perror("Eroare SIGTERM!\n");
        exit(EXIT_FAILURE);
    }

    printf("PID=%d — trimite SIGTERM cu: kill -TERM %d\n", getpid(), getpid());

    return 0;
}
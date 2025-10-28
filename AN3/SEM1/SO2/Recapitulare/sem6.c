#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <unistd.h>

void MyHandler(int sig){
    printf("ALARM!\n");
    alarm(1);
}

int main(void){
    struct sigaction sa;
    memset(&sa,0,sizeof(struct sigaction));
    sa.sa_handler = MyHandler;
    sa.sa_flags = SA_RESTART;

    if(sigaction(SIGALRM,&sa,NULL) < 0){
        perror("Eroare la SIGALRM!\n");
        exit(EXIT_FAILURE);
    }

    alarm(1);
    printf("Timer pornit. Apasa Enter pentru a iesi.\n"); 

    int n;
    scanf("%d",&n);
    printf("%d\n",n);

    return 0;
}
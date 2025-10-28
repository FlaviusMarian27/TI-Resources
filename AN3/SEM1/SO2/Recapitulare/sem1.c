/*
Scrie un program care interceptează semnalul SIGINT (Ctrl+C) și 
afișează un mesaj fără să închidă procesul.
*/

#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int count = 0;

void Myhandler(int sig){
    count++;
    printf("Am primit SIGINT de %d ori\n", count);
    if(count >= 5){
        printf("Ajunge, ies.\n");
        exit(0);
    }
}

int main(void){
    struct sigaction sa;
    memset(&sa,0,sizeof(sa));
    sa.sa_handler = Myhandler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    if(sigaction(SIGINT,&sa,NULL) < 0){
        perror("Eroare sigaction\n");
        exit(EXIT_FAILURE);
    }

    printf("PID=%d — apasa Ctrl+C\n", getpid());
    while (1){
        pause();
    }

    return 0;
}
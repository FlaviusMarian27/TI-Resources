/*
4. Să se scrie un program C, care își definește un comportament nou la recepția semnalului SIGUSR1.
Programul va afișa un mesaj de fiecare dată când acest semnal este primit.
Testarea programului se va face prin trimiterea semnalului SIGUSR1 dintr-un alt terminal, folosind comanda ps aux pentru a afla PIDul și comanda kill pentru a trimite semnalul.
*/

#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<string.h>

void MyHandler(int sig){
    printf("Am primit semnalul SIGUSR1!\n");
}

int main(void){
    struct sigaction sa;
    memset(&sa,0,sizeof(struct sigaction));
    sa.sa_handler = MyHandler;
    sa.sa_flags = 0;

    if(sigaction(SIGUSR1,&sa,NULL) < 0){
        perror("Eroare SIGUSR1");
        exit(EXIT_FAILURE);
    }

    printf("PID=%d — trimite semnal cu: kill -USR1 %d\n", getpid(), getpid());
    printf("Astept semnale...\n");

    while(1){
        pause();
    }

    return 0;
}
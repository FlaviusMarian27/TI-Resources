/*
Creează un proces copil care trimite un semnal SIGUSR1 părintelui.
Părintele va afișa PID-ul copilului care a trimis semnalul.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

void MyHandler(int sig, siginfo_t *info, void *ctx){
    printf("Parintele a primit SIGUSR1 de la PID=%d\n", info->si_pid);
}

int main(void){
    struct sigaction sa;
    memset(&sa,0,sizeof(struct sigaction));
    sa.sa_sigaction = MyHandler;
    sa.sa_flags = SA_SIGINFO;

    if(sigaction(SIGUSR1,&sa,NULL) < 0){
        perror("Eroare la SIGURS1!\n");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if(pid < 0){
        perror("Eroare la crearea procesului!\n");
        exit(EXIT_FAILURE);
    }

    if(pid == 0){
        sleep(1);
        kill(getppid(),SIGUSR1);
        exit(0);
    } else {
        pause(); 
        printf("Părinte: semnal procesat, copilul %d a terminat.\n", pid);
    }

    return 0;
}
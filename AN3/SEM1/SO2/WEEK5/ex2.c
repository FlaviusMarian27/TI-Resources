/*
Program: proces parinte si proces copil. Procesul copil va apela sleep(2), dupa care ii
trimite procesului parinte semnalul SIGUSR2 si isi termina executia. Procesulparinte va
afisa o data la 500 ms mesajul "Astept" pana la primirea semanului SIGUSR2.
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void handler(int sig){
    wait(NULL);
    printf("Semnal receptionat!\n");
    exit(0);
}

int main(void){
    pid_t pid;
    pid = fork();
    if (pid < 0){
        fprintf(stderr, "fork error\n");
        exit(EXIT_FAILURE);
    }

    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_handler = handler;
    sigemptyset(&act.sa_mask);

    if (sigaction(SIGUSR2, &act, NULL) < 0){
        fprintf(stderr, "sigaction error\n");
        exit(EXIT_FAILURE);
    }

    if (pid == 0){
        sleep(2);
        kill(getppid(), SIGUSR2);
        exit(0);
    }else{
        while (1){
            usleep(500000);
            printf("Astept\n");
        }
    }

    return 0;
}
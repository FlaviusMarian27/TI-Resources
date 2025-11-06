/*
5. Să se scrie un program C care constă în 2 procese înrudite:
procesul copil: își va seta un comportament nou la recepția semnalului SIGUSR1: va afișa un mesaj și își va termina execuția.
procesul părinte: va trimite semnalul SIGUSR1 procesului copil și după va prelua starea acestuia.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

void handler_child(int sig) {
    printf("Copilul a primit semnalul SIGUSR1 si se inchide.\n");
    sleep(2);
    exit(0);
}

int main(void){
    pid_t pid;
    pid = fork();
    if (pid < 0){
        fprintf(stderr, "Fork Failed\n");
        exit(EXIT_FAILURE);
    }

    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_handler = handler_child;
    sigemptyset(&act.sa_mask);
    if (sigaction(SIGUSR1, &act, NULL) < 0){
        fprintf(stderr, "sigaction failed\n");
        exit(EXIT_FAILURE);
    }

    if (pid == 0){
        printf("Child %d wait the signal!\n",getpid());

        while (1){
            pause();
        }

        exit(0);
    }else{
        sleep(2);
        printf("Parent %d sent the signal to child!\n",getpid());
        sleep(2);
        if (kill(pid, SIGUSR1) < 0){
            fprintf(stderr, "kill failed\n");
            exit(EXIT_FAILURE);
        }

        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)){
            printf("Child %d exited with code %d\n", getpid(), WEXITSTATUS(status));
        }else{
            printf("Child %d exited abnormally\n", getpid());
        }
    }

    return 0;
}

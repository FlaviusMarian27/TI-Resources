/*
Program: proces părinte și proces copil.
Părintele va seta o alarmă care expiră peste 5 secunde,
după care va crea procesul copil, după care afișează o dată la 500 ms mesajul „Aștept”.
Când expiră alarma, părintele trimite semnalul SIGUSR1 procesului copil,
care la recepție va afișa de 5 ori „PRIMIT”!
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>

int done = 0;
void HandlerParent(int sig){
    done = 1;
}

void HandlerChild(int sig){
    for (int i = 0; i < 5; i = i + 1){
        usleep(200000);
        printf("PRIMIT\n");
    }

    exit(0);
}

int main(void){
    alarm(5);

    pid_t pid;
    pid = fork();
    if (pid < 0){
        fprintf(stderr, "fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0){
        struct sigaction sig_usr1;
        memset(&sig_usr1, 0, sizeof(sig_usr1));
        sig_usr1.sa_handler = HandlerChild;
        sigemptyset(&sig_usr1.sa_mask);
        if (sigaction(SIGUSR1, &sig_usr1, NULL) < 0){
            fprintf(stderr, "sigaction failed");
            exit(EXIT_FAILURE);
        }

        sig_usr1.sa_handler = SIG_IGN;
        if (sigaction(SIGALRM, &sig_usr1, NULL) < 0){
            perror("sigaction failed ALARM");
            exit(EXIT_FAILURE);
        }

        while (1){
            pause();
        }

        exit(0);
    }else{
        struct sigaction sig_alarm;
        memset(&sig_alarm, 0, sizeof(sig_alarm));
        sig_alarm.sa_handler = HandlerParent;
        sigemptyset(&sig_alarm.sa_mask);

        if (sigaction(SIGALRM, &sig_alarm, NULL) < 0){
            fprintf(stderr, "sigaction failed");
            exit(EXIT_FAILURE);
        }

        while (1){
            if (done == 1){
                break;
            }
            usleep(500000);
            printf("Astept\n");
        }

        kill(pid, SIGUSR1);
        wait(NULL);
    }

    return 0;
}
/*
Program care afiseaza mesajul "Astept" o data la 100 ns. DIn alt terminal se va
trimite programului semnalul SIGUSR1, iar programul la prire va aifsa mesajul "Mersi"
si isi va termina executia.

in alt terminal se va scrie.

ps -aux 
kill -SIGUSR1 pid
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

void MyHandler(int sig){
    printf("Mersi!\n");
    exit(0);
}

int main(void){
    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_handler = MyHandler;
    sigemptyset(&act.sa_mask);

    if (sigaction(SIGUSR1, &act, NULL) < 0){
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    while (1){
        sleep(1);
        printf("Astept\n");
    }

    return 0;
}
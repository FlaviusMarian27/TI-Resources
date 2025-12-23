/*
1. Să se scrie un program C ce implementează un client TCP. Programul va primi în linie de comandă adresa
ip și portul serverului la care clientul se va conecta:
./prog <ip> <port>

Programul va avea următoarea funcționalitate:
Thread-ul principal al programului se va conecta la server. Dacă conexiunea nu reușește programul se va
termina. În cazul în care conexiunea reușește programul va creea 3 alte thread-uri noi care vor comunica
printr-un buffer comun (string) și o variabilă comună (length) ce reprezintă dimensiunea datelor din buffer.
Buffer-ul va conține doar un string la un moment dat. Thread-urile care vor scrie în buffer-ul string vor
seta dimensiunea acestuia în variabila length.

Thread-ul 1 (write_to_socket): va monitoriza un buffer-ul și dimensiunea acestuia. În momentul în care
variabila length va fi nenulă acest thread va citit buffer-ul comun, îl va trimite pe socket și după
aceasta va reseta la zero variabila length.

Thread-ul 2 (read_from_stdin): va citi câte o linie de la intrarea standard și o va trimite către
thread-ul 1, prin variabilele buffer și length pentru a fi trimisă pe socket. Acesta va copia linia
în buffer-ul string și va seta length cu dimensiunea acestuia spre a fi preluată de thread-ul 1 pentru
a fi transmisă pe socket

Thread-ul 3 (read_from_socket): va citi date text de la socket, va schimba literele mari în litere mici
și invers și va trimite apoi rezultatul în bufferul string spre a fi apoi trimis de thread-ul 1 peste
socket. Mecanismul de comunicare cu acesta este similar ca și la thread-ul 2.

Se recomandă ca thread-urile să fie joinable. Este necesar ca variabilele string si length să fie
controlate prin mutex.

Programul va fi testat cu ajutorul unui server TCP ce va fi lansat cu ajutorul utilitarului netcat:

nc -l <port> -s <ip> -v
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define BUF_SIZE 1024

int sockefd;
char buffer[BUF_SIZE];
int length = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* write_to_socket(void* arg){
    while(1){
        pthread_mutex_lock(&mutex);

        if (length > 0){
            write(sockefd,buffer,length);
            length = 0;
        }

        pthread_mutex_unlock(&mutex);

        sleep(1);
    }

    return NULL;
}

void* read_from_stdin(void* arg){
    char local_buffer[BUF_SIZE];

    while (fgets(local_buffer,BUF_SIZE,stdin) != NULL){
        pthread_mutex_lock(&mutex);

        if (length == 0){
            strcpy(buffer,local_buffer);
            length = strlen(buffer);
        }

        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

void* read_from_socket(void* arg){
    char local_buffer[BUF_SIZE];
    int bytes_read;
    while ((bytes_read = read(sockefd,local_buffer,BUF_SIZE)) > 0){
        for (int i = 0; i < bytes_read; i++){
            if (islower(local_buffer[i])){
                local_buffer[i] = toupper(local_buffer[i]);
            }else if (isupper(local_buffer[i])){
                local_buffer[i] = tolower(local_buffer[i]);
            }
        }

        pthread_mutex_lock(&mutex);
        if (length == 0){
            memcpy(buffer, local_buffer, bytes_read);
            length = bytes_read;
        }
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

int main(int argc, char *argv[]){
    if(argc != 3){
        fprintf(stderr,"Usage: %s <ip> <port>\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    pthread_mutex_init(&mutex,NULL);

    struct sockaddr_in servaddr;
    if ((sockefd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        fprintf(stderr,"Socket creation error\n");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    servaddr.sin_port = htons(atoi(argv[2]));

    if (connect(sockefd,(struct sockaddr *)&servaddr,sizeof(servaddr)) < 0){
        fprintf(stderr,"Connection error\n");
        exit(EXIT_FAILURE);
    }

    pthread_t t1, t2, t3;
    if (pthread_create(&t1,NULL,write_to_socket,NULL) != 0){
        fprintf(stderr,"Thread creation error\n");
        exit(EXIT_FAILURE);
    }

    if (pthread_create(&t2,NULL,read_from_stdin,NULL) != 0){
        fprintf(stderr,"Thread creation error\n");
        exit(EXIT_FAILURE);
    }

    if (pthread_create(&t3,NULL,read_from_socket,NULL) != 0){
        fprintf(stderr,"Thread creation error\n");
        exit(EXIT_FAILURE);
    }

    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    pthread_join(t3,NULL);

    close(sockefd);
    pthread_mutex_destroy(&mutex);

    return 0;
}
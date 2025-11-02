/*
6. Să se scrie un program C, care constă în 2 procese înrudite:
procesul copil: va citi în întregime un fișier text primit ca și argument în linie 
de comandă și va trimite doar vocalele procesului părinte folosind un pipe
procesul părinte: va număra câți octeți a primit prin pipe, va afișa acest număr și 
după va prelua starea procesului copil.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

#define BUF_SIZE 4096

int is_vowel(char ch){
    if (strchr("aeiouAEIOU", ch) != NULL){
        return 1;
    }
    return 0;
}

void readFromPath(const char* path, int end_of_writing){
    int fd = open(path, O_RDONLY);
    if (fd < 0){
        fprintf(stderr,"open\n");
        exit(EXIT_FAILURE);
    }

    ssize_t bytesRead;
    char buffer[BUF_SIZE];
    while ((bytesRead = read(fd, buffer, BUF_SIZE)) > 0){
        for (int i = 0; i < bytesRead; i++){
            if (is_vowel(buffer[i]) == 1){
                write(end_of_writing, &buffer[i], 1);
            }
        }
    }

    if (close(fd) != 0){
        fprintf(stderr,"close\n");
        exit(EXIT_FAILURE);
    }
}

int readFromPipe(int pfd){
    int counter = 0;
    char character;

    while (read(pfd,&character,1) > 0){
        counter = counter + 1;
    }

    return counter;
}

int main(int argc, char *argv[]){
    if(argc != 2){
        fprintf(stderr,"Usage: ./exe file\n");
        exit(EXIT_FAILURE);
    }

    int pfd[2];
    if (pipe(pfd) < 0){
        fprintf(stderr,"pipe\n");
        exit(EXIT_FAILURE);
    }

    pid_t pid;
    pid = fork();
    if (pid < 0){
        fprintf(stderr,"fork\n");
        exit(EXIT_FAILURE);
    }

    if (pid == 0){
        close(pfd[0]);
        readFromPath(argv[1], pfd[1]);
        close(pfd[1]);
        exit(EXIT_SUCCESS);
    }else{
        close(pfd[1]);

        int counter = 0;
        counter = readFromPipe(pfd[0]);

        close(pfd[0]);
        waitpid(pid,NULL,0);
        printf("Parintele a primit %d vocale din fisierul %s!\n",counter,argv[1]);
    }

    return 0;
}
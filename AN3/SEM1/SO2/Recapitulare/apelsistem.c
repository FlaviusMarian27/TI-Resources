//Citim dintr-un fisier si afisam in terminal
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>

#define BUF_SIZE 1024

int main(int argc, char* argv[]){
    if(argc != 2){
        perror("Trebuie 2 argumente!\n");
        exit(-1);
    }

    int fd = open(argv[1],O_RDONLY);
    if(fd < 0){
        perror("Eroare la open!\n");
        exit(-1);
    }

    char buffer[BUF_SIZE];
    ssize_t bytesRead;
    while((bytesRead = read(fd,buffer,BUF_SIZE)) > 0){
        if(write(STDOUT_FILENO,buffer,bytesRead) != bytesRead){
            perror("Eroare la scrierea la stdout!\n");
            close(fd);
            exit(-1);
        }
    }

    if(bytesRead < 0){
        perror("Eroare la citire!\n");
        exit(-1);
    }

    if(close(fd) != 0){
        perror("Eroare la inchiderea fisierului!\n");
        exit(-1);
    }

    return 0;
}
//citim dintr un fisier si scriem altul
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>

#define SIZE_BUFFER 1024

int main(int argc, char* argv[]){
    if(argc != 3){
        perror("Foloseste ./p file_in.txt file_out.txt");
        exit(-1);
    }

    int fd_in = open(argv[1],O_RDONLY);
    if(fd_in < 0){
        perror("Eroare la deschiderea fisierului de intrare!\n");
        exit(-1);
    }

    int fd_out = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC,0644);
    if(fd_out < 0){
        perror("Eroare la deschiderea fisierului de iesire!\n");
        close(fd_in);
        exit(-1);
    }

    char buffer[SIZE_BUFFER];
    ssize_t bytesRead;
    while((bytesRead = read(fd_in,buffer,SIZE_BUFFER)) > 0){
        if(write(fd_out,buffer,bytesRead) != bytesRead){
            perror("Eroare la scriere in fisierul de iesire!\n");
            close(fd_in);
            close(fd_out);
            exit(-1);
        }
    }

    if(bytesRead < 0){
        perror("La citirea din fisierului de intrare\n");
        exit(-1);
    }

    
    if(close(fd_in) != 0 || close(fd_out) != 0){
        perror("Eroare la inchiderea fisierului de intrare!\n");
        exit(-1);
    }

    return 0;
}
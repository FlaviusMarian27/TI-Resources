#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>

#define MAX_SIZE 4096

int main(int argc, char* argv[]){
    if(argc != 3){
        perror("Trebuie program <file.in> <file.out>\n");
        exit(1);
    }

    int fd_in = open(argv[1],O_RDONLY);
    if(fd_in < 0){
        perror("Eroare la deschiderea fisierului de intrare!\n");
        exit(1);
    }

    int fd_out = open(argv[2],O_WRONLY | O_CREAT | O_TRUNC,0644);
    if(fd_out < 0){
        perror("Eroare la deschiderea fisierului de iesire!\n");
        exit(1);
    }

    ssize_t bytesRead;
    char buffer[MAX_SIZE];
    int count = 0;
    while((bytesRead = read(fd_in,buffer,MAX_SIZE)) > 0){
        for(int i = 0; i < bytesRead; i = i + 1){
            if(buffer[i] >= 97 && buffer[i] <= 122){
                count = count + 1;
                printf("%c",buffer[i]);
            }else{
                printf("%02X",buffer[i]);
            }
        }
    }

    printf("\n");
    //printf("%d\n",count);
    char msg[100];
    int len = snprintf(msg,sizeof(msg),"Numărul total de litere mici afișate este %d!\n",count);
        
    if(write(fd_out,msg,len) == -1){
        perror("Eroare la scrierea in fisier!\n");
        exit(1);
    }

    if(close(fd_in) != 0){
        perror("Eroare la inchiderea fisierului de intrare!\n");
        exit(1);
    }

    if(close(fd_out) != 0){
        perror("Eroare la inchiderea fisierului de iesire!\n");
        exit(1);
    }

    return 0;
}
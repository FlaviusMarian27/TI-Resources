/*
1. Să se scrie un program care primește ca și argumente în linie de comandă calea către 2 
fișiere:
          <program> <fișier-intrare> <fișier-ieșire>
Programul va citi în întregime <fișier-intrare>, și va afișa la ieșirea standard octeții 
transformați după regula următoare: dacă octetul are valoarea între 97 și 122, va fi afișat 
folosind printf, ca și literă mică altfel se va afișa în hexadecimal
La final, programul va scrie în <fișier-ieșire> o linie de forma:
''Numărul total de litere mici afișate este ...''
*/

#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>

int main(int argc, char* argv[]){
    if(argc != 3){
        perror("Trebuie ./p file_in.txt file_out.txt!\n");
        exit(-1);
    }

    int fd_in = open(argv[1],O_RDONLY);
    if(fd_in < 0){
        perror("Eroare la deschiderea file_input!\n");
        exit(-1);
    }

    int fd_out = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(fd_out < 0){
        perror("Eroare la deschiderea file_output!\n");
        close(fd_in);
        exit(-1);
    }

    unsigned char buffer;
    ssize_t byteRead;
    int count = 0;
    while((byteRead = read(fd_in,&buffer,1)) > 0){
        if(buffer >= 97 && buffer <= 122){
            printf("%c", buffer);
            count = count + 1;
        }else{
            printf("%02X", buffer);
        }
    }
    printf("\n");

    char msg[100];
    snprintf(msg,sizeof(msg),"Numărul total de litere mici afișate este %d!\n",count);
    if(write(fd_out,msg,strlen(msg)) < 0){
        perror("Eroare la scrierea fisierelui!\n");
        exit(-1);
    }

    if(close(fd_in) != 0){
        perror("Eroare la inchiderea file_input!\n");
        exit(-1);
    }

    if(close(fd_out) != 0){
        perror("Eroare la inchiderea file_output!\n");
        exit(-1);
    }

    return 0;
}

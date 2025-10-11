/*
6. Să se scrie un program care primește ca argument în linie de comandă calea către un fișier ce va fi interpretat în mod binar.
În acest fișier se consideră că există un număr necunoscut de numere întregi pe 1 byte fără semn.
Programul va citi acest fișier și dacă va găsi un octet ce reprezintă un caracter printabil literă mare, literă mică sau cifră, atunci îl va scrie imediat în fișierul de ieșire în format text.
Calea către fișierul de ieșire este dată de al doilea argument al programului.
Dacă va găsi un octet ce nu este printabil literă mare, literă mică sau cifră, va scrie ca text valorea în hexazecimal între paranteze rotunde.
Exemplu (1):
dacă în fișier se găsește la un moment dat octetul cu valorea 0x4E în fișierul de ieșire va scrie N
dacă în fișier se găsește la un moment dat octetul cu valorea 0x1A în fișierul de ieșire va scrie (1A)
Exemplu (2):
dacă conținutul fișierului binar (valori în hexazecimal ai octeților) este:  19587A61053014
fișierul de ieșire va avea următorul conținut text:                                     (19)Xza(05)0(14)
Funcția se va testa printr-un program ce va primi căile pentru cele două fișiere ca și argumente în linie de comandă. Primul argument reprezintă calea fișierului binar și al doilea calea fișierului text în care va scrie datele în formatul de mai sus.
Programul se poate testa cu un fișier ce se poate descărca folosind următoarea comandă:
wget http://staff.cs.upt.ro/~valy/pt/integers.bin
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>

#define SIZE 4096

void readBinaryFiles(const char* path1, const char* path2){
    int fd = open(path1,O_RDONLY);
    if(fd < 0){
        perror("Eroare la open!\n");
        exit(1);
    }

    int fd_out = open(path2,O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(fd_out < 0){
        perror("Eroare la open out!\n");
        exit(1);
    }

    ssize_t readBytes;
    unsigned char buffer[SIZE];
    while((readBytes = read(fd,buffer,SIZE)) > 0){
        for(int i = 0; i < readBytes; i = i + 1){
            if(isalnum(buffer[i])){
                write(fd_out,&buffer[i],1);
            }else{
                char text[8];
                int len = snprintf(text,sizeof(text),"(%02X)",buffer[i]);
                write(fd_out,text,len);
            }
        }
    }

    if(close(fd) != 0){
        perror("Eroare la close!\n");
        exit(1);
    }

    if(close(fd_out) != 0){
        perror("Eroare la close out!\n");
        exit(1);
    }
}

int main(int argc, char* argv[]){
    if(argc != 3){
        perror("Eroare argumente insuficiente!\nTrebuie ./executabil input output");
        exit(1);
    }

    readBinaryFiles(argv[1],argv[2]);

    return 0;
}
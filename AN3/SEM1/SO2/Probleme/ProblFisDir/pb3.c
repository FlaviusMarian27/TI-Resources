/*
Să se scrie un program care primește ca și argumente în linie de comandă calea către 2 
fișiere:
          <program> <fișier-intrare> <fișier-ieșire>
Fișierul de intrare va conține un număr necunoscut de numere întregi pe 4 octeți.
Programul va citi fișierul de intrare în întregime și va scrie în fișierul de ieșire, în 
format text, numărul minim, numărul maxim și media numerelor citite din fișierul de intrare 
binar.
*/
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>

int main(int argc, char* argv[]){
    if(argc != 3){
        perror("Trebuie: ./executatbil <file1> <file2>\n");
        exit(-1);
    }

    int fd_in = open(argv[1],O_RDONLY);
    if(fd_in < 0){
        perror("Eroare open input!\n");
        exit(-1);
    }

    int fd_out = open(argv[2],O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(fd_out < 0){
        perror("Eroare open output!\n");
        exit(-1);
    }

    int value;
    ssize_t bytesRead;
    long long sum_of_values = 0;
    int count = 0;
    int minim_value = 9999;
    int maxim_value = -9999;

    while((bytesRead = read(fd_in,&value,sizeof(int))) > 0){
        if(value > maxim_value){
            maxim_value = value;
        }

        if(value < minim_value){
            minim_value = value;
        }

        sum_of_values = sum_of_values + value;
        count = count + 1;
    }

    if(count == 0){
        write(fd_out,"Fisierul este gol!\n",20);
    }else{
        double media = (double) sum_of_values / count;
        char buffer[100];
        int len = snprintf(buffer,sizeof(buffer),"Minim: %d\nMaxim: %d\nMedia: %.2f\n",
                           minim_value, maxim_value, media);
        write(fd_out,buffer,len);
    }

    if(close(fd_in) != 0){
        perror("Eroare close input!\n");
        exit(-1);
    }

    if(close(fd_out) != 0){
        perror("Eroare close output!\n");
        exit(-1);
    }

    return 0;
}
/*
2. Să se scrie un program care primește ca și argument în linie de comandă calea 
către o intrare de pe disc.
Programul va afișa pentru intrarea respectivă următoarele informații:
Tipul intrării: director / fișier obișnuit / legătură simbolică
Permisiunile pentru owner/user, sub forma: rwx, - dacă vreuna lipsește
Dimensiunea în octeți 
*/

#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<unistd.h>

int main(int argc, char* argv[]){
    if(argc != 2){
        perror("Argumente insuficiente!\n");
        exit(-1);
    }

    struct stat st;
    if(stat(argv[1],&st) < 0){
        perror("Eroare la stat!\n");
        exit(-1);
    }

    if(S_ISREG(st.st_mode)){
        printf("Este un fisier simplu!\n");
    }else if(S_ISDIR(st.st_mode)){
        printf("Este un director!\n");
    }else if(S_ISLNK(st.st_mode)){
        printf("Este o legatura simbolica!\n");
    }else{
        printf("Este altceva!\n");
    }

    printf("Permisiuni: ");
    if(access(argv[1],R_OK) == 0){
        printf("r");
    }else{
        printf("-");
    }

    if(access(argv[1],W_OK) == 0){
        printf("w");
    }else{
        printf("-");
    }

    if(access(argv[1],X_OK) == 0){
        printf("x");
    }else{
        printf("-");
    }

    printf("\nDimensiunea: %ld bytes\n",st.st_size);
    return 0;
}
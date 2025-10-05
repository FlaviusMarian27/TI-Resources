#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

void afisare_informatie(struct stat st){
    printf("Dimensiunea: %ld bytes\n",st.st_size);
    if(S_ISREG(st.st_mode)){
        printf("Este un fisier simplu!\n");
    }else if(S_ISDIR(st.st_mode)){
        printf("Este un director!\n");
    }else if(S_ISLNK(st.st_mode)){
        printf("Este un link simbolic\n");
    }else{
        printf("Este un alt tip de fisier!\n");
    }
}

int main(int argc, char* argv[]){
    if(argc != 2){
        perror("Folosete ./executabil <fisier>\n");
        exit(-1);
    }

    struct stat st1, st2, st3;
    printf("=== Cu stat() ===\n");
    if(stat(argv[1],&st1) == 0){
        afisare_informatie(st1);
    }else{
        perror("Eroare st1\n");
    }

    printf("\n=== Cu lstat() ===\n");
    if(lstat(argv[1],&st2) == 0){
        afisare_informatie(st2);
    }else{
        perror("Eroare st2\n");
    }

    printf("\n=== Cu fstat() ===\n");
    int fd = open(argv[1],O_RDONLY);
    if(fd < 0){
        perror("Eroare open!\n");
        exit(-1);
    }

    if(fstat(fd,&st3) == 0){
        afisare_informatie(st3);
    }else{
        perror("Eroare st3\n");
    }

    close(fd);
    return 0;
}
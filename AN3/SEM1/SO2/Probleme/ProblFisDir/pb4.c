/*
4. Să se scrie un program C care primește ca și argumente în linie de comandă o cale 
relativă sau absolută de director și o cale către un fișier.
Programul va avea următoarele funcționalități:
va parcurge recursiv directorul dat ca și parametru
pentru fiecare fișier întâlnit, programul va număra câte litere mici conține acesta
va scrie rezultatul într-un fișier text dat ca și argument în linie de comandă, sub 
forma:
<cale-director> <dimensiune> <nr-litere-mici> <tip-fișier>
unde tip-fișier poate fi: REG, LNK , DIR sau PIPE
dacă primul argument nu este director, programul va afișa un mesaj de eroare
dacă al doilea argument reprezintă un fișier deja existent pe disc, acesta va fi 
suprascris
Programul se va testa folosind un director ce conține directoare, fișiere obișnuite, 
legături simbolice și named pipes.
*/

#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<sys/stat.h>
#include<ctype.h>

int main(int argc, char *argv[]){
    if(argc != 3){
        perror("Argumente insuficiente!\nTrebuie ./exe director file\n");
        exit(-1);
    }

    struct stat st;
    if(stat(argv[1],&st) < 0){
        perror("Eroare la stat pt verificarea directorului!\n");
        exit(1);
    }

    if(S_ISDIR(st.st_mode) == 0){
        printf("Eroare! %s nu este un director!\n",argv[1]);
        exit(1);
    }

    DIR* director = opendir(argv[1]);
    if(director == NULL){
        perror("Eroare la deschiderea directorului!\n");
        exit(1);
    }

    //printf("Succes!\n");
    int fd_out = open(argv[2],O_WRONLY | O_CREAT | O_TRUNC,0644);
    if(fd_out < 0){
        perror("Eroare la deschiderea fisierului!\n");
        exit(1);
    }

    struct dirent *entry;
    struct stat info;
    char path[1024];
    char line[512];

    while((entry = readdir(director)) != NULL){
        if(strcmp(entry->d_name,".") == 0 || strcmp(entry->d_name,"..") == 0){
            continue;
        }

        snprintf(path,sizeof(path),"%s/%s",argv[1],entry->d_name);//calea fisierului
        if(lstat(path,&info) < 0){
            perror("Eroare la lstat!\n");
            exit(1);
        }

        char tip[8];
        if(S_ISREG(info.st_mode)){
            strcpy(tip,"REG");
        }else if(S_ISDIR(info.st_mode)){
            strcpy(tip,"DIR");
        }else if(S_ISLNK(info.st_mode)){
            strcpy(tip,"LINK");
        }else if(S_ISFIFO(info.st_mode)){
            strcpy(tip,"PIPE");
        }else{
            strcpy(tip,"OTHER");
        }

        int count = 0;
        if(S_ISREG(info.st_mode)){
            int fd_in = open(path,O_RDONLY);
            if(fd_in < 0){
                perror("Eroare la deschiderea fisierului!\n");
                exit(1);
            }else{
                ssize_t bytesRead;
                char buffer[512];
                while((bytesRead = read(fd_in,buffer,sizeof(buffer))) > 0){
                    for(ssize_t i = 0; i < bytesRead; i = i + 1){
                        if(islower(buffer[i])){
                            count = count + 1;
                        }
                    }
                }
            }

            if(close(fd_in) != 0){
                perror("Eroare la inchiderea fisierului pt litere!\n");
                exit(1);
            }
        }

        int len = snprintf(line,sizeof(line),"%s %ld %d %s\n",path,info.st_size,count,tip);
        write(fd_out,line,len);
    }

    if(closedir(director) != 0){
        perror("Eroare la inchiderea directorului!\n");
        exit(1);
    }

    if(close(fd_out) != 0){
        perror("Eroare la inchiderea fisierului!\n");
        exit(1);
    }

    return 0;
}
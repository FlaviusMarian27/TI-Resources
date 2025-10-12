/*
Să se scrie un program C, care primește ca și argumente în linie de comandă: calea 
către un director de pe disc <dir> și un număr natural N.
Programul va parcurge directorul primit în mod recursiv, va identifica toate fișierele 
obișnuite și va afișa caracterul de pe poziția N din fiecare.
Dacă fișierul are extensia ".del", acesta va fi șters de pe disc după afișarea 
caracterului dorit. Dacă se întâlnește o legătură simbolică, programul va elimina 
permisiunea de read pentru ținta legăturii.
*/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

void parcurge(const char* path, int N){
    DIR* dir = opendir(path);
    if(dir == NULL){
        perror("Eroare opendir!\n");
        exit(-1);
    }

    struct dirent *entry;
    struct stat st;
    char fullpath[1024];
    while((entry = readdir(dir)) != NULL){
        if(strcmp(entry->d_name,".") == 0 || strcmp(entry->d_name,"..") == 0){
            continue;
        }

        snprintf(fullpath,sizeof(fullpath),"%s/%s",path,entry->d_name);
        if(lstat(fullpath,&st) < 0){
            perror("Eroare la lstat!\n");
            exit(-1);
        }

        if(S_ISDIR(st.st_mode)){
            parcurge(fullpath,N);
        }else if(S_ISLNK(st.st_mode)){
            char target[256];
            ssize_t len = readlink(fullpath,target,sizeof(target) - 1);
            if (len < 0) {
                perror("Eroare la readlink");
                continue;
            }

            target[len] = '\0';
            if(chmod(target,st.st_mode & ~S_IRUSR) < 0){
                perror("Eroare la modificarea permisiunilor");
            } else {
                printf("Permisiune de citire eliminata pentru: %s\n", target);
            }
        }else if (S_ISREG(st.st_mode)){ 
            int fd = open(fullpath, O_RDONLY);
            if (fd < 0) {
                perror("Eroare la deschiderea fisierului");
                continue;
            }

            if (lseek(fd, N, SEEK_SET) < 0) {
                perror("Eroare la lseek");
                close(fd);
                continue;
            }

            char c;
            ssize_t r = read(fd, &c, 1);
            if (r == 1)
                printf("%s -> caracter[%d] = '%c'\n", fullpath, N, c);
            else
                printf("%s -> nu are caracter la poziția %d\n", fullpath, N);

            close(fd);

            const char *ext = strrchr(entry->d_name, '.');
            if (ext && strcmp(ext, ".del") == 0) {
                if (unlink(fullpath) == 0)
                    printf("Șters: %s\n", fullpath);
                else
                    perror("Eroare la ștergere");
            }
        }
    }

    if(closedir(dir) != 0){
        perror("Eroare closedir!\n");
        exit(-1);
    }
}

int main(int argc, char* argv[]){
    if(argc != 3){
        perror("Trebuie ./exe director N\n");
        exit(-1);
    }   

    int N = atoi(argv[2]);
    if(N < 0){
        perror("N trebuie sa fie un numar natural\n");
        exit(-1);
    }

    parcurge(argv[1],N);
    return 0;
}
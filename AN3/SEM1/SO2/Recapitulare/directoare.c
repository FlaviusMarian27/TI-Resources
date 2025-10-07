#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<sys/stat.h>
#include<string.h>

int main(int argc, char* argv[]){
    if(argc != 2){
        perror("Argumente insuficiente!\nTrebuie: ./exe calea\n");
        exit(-1);
    }

    DIR *dirp = opendir(argv[1]);
    if(dirp == NULL){
        perror("Eroare opendir!\n");
        exit(-1);
    }

    struct dirent *entry;
    struct stat st;
    char path[512];
    while((entry = readdir(dirp)) != NULL){
        if(entry->d_name[0] == '.'){
            continue;
        }
        snprintf(path,sizeof(path),"%s/%s",argv[1],entry->d_name);
        if (stat(path, &st) == 0){
            printf("%-20s  %10ld bytes\n", entry->d_name, st.st_size);
        }
    }

    if(closedir(dirp) != 0){
        perror("Eroare la close!\n");
        exit(-1);
    }
    return 0;
}
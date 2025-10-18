#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

#define BUFFER_SIZE 1024

void readFromDirectoryRecursive(const char* director_path){
    DIR* director = opendir(director_path);
    if(director == NULL){
        perror("Error opening directory");
        exit(EXIT_FAILURE);
    }

    struct dirent* entry;
    while((entry = readdir(director)) != NULL){
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){
            continue;
        }

        char buffer[BUFFER_SIZE];
        snprintf(buffer, BUFFER_SIZE, "%s/%s", director_path, entry->d_name);

        struct stat st;
        if(lstat(buffer, &st) < 0){
            perror("lstat error");
            exit(EXIT_FAILURE);
        }

        printf("Name: %s\tCalea: %s\tDimensiune: %ld ->", entry->d_name, buffer, st.st_size);
        if(S_ISDIR(st.st_mode)){
            printf("Directory\n");
            readFromDirectoryRecursive(buffer);
        }else if (S_ISREG(st.st_mode)){
            printf("Regular file\n");
        }else if (S_ISLNK(st.st_mode)){
            printf("Link\n");
        }else if (S_ISSOCK(st.st_mode)){
            printf("Socket\n");
        }else if (S_ISFIFO(st.st_mode)){
            printf("FIFO\n");
        }else{
            printf("Other\n");
        }
    }

    if(closedir(director) != 0){
        perror("Error closing directory");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]){
    if (argc != 2){
        printf("Usage: %s <pathname>\n", argv[0]);
    }

    readFromDirectoryRecursive(argv[1]);
    return 0;
}
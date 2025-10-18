/*Se cere sa se parcurga un director dat ca si argument in linie de comanda in mod
nerecursiv. Practic parcurgi o singura data directorul. Trebuie sa afisezi 
nume cale si dimensiunea in bytes*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

#define BUFSIZE 1024

void readFromDirectoryNonRecursive(const char* director_path){
    DIR* directory = opendir(director_path);
    if(directory == NULL){
        perror("Error opening directory");
        exit(EXIT_FAILURE);
    }

    struct dirent* entry;
    while((entry = readdir(directory)) != NULL){
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){
            continue;
        }

        char filename[BUFSIZE];
        snprintf(filename, BUFSIZE, "%s/%s", director_path, entry->d_name);

        struct stat st;
        if(stat(filename, &st) == -1){
            perror("Error stat");
            exit(EXIT_FAILURE);
        }

        printf("Name: %s\tCalea: %s\tDimensiunea: %ld bytes\n", entry->d_name, filename, st.st_size);
    }

    if (closedir(directory) < 0){
        perror("Error closing directory");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    if(argc != 2) {
        printf("Usage: %s <pathname>\n", argv[0]);
    }

    readFromDirectoryNonRecursive(argv[1]);
    return 0;
}
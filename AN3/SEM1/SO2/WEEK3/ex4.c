/*
Pentru fiecare fișier cu permisiune de execuție setată 
pentru owner, se va revoca acea permisiune (folosind chmod).
*/

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

#define MAX_SIZE_PATH 1024

void setPermisions(const char* director_path){
    DIR* dir = opendir(director_path);
    if(dir == NULL){
        perror("opendir error");
        exit(EXIT_FAILURE);
    }

    struct dirent* entry;
    while((entry = readdir(dir)) != NULL){
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){
            continue;
        }

        char filename[MAX_SIZE_PATH];
        snprintf(filename, MAX_SIZE_PATH, "%s/%s", director_path, entry->d_name);

        struct stat st;
        if(lstat(filename, &st) < 0){
            perror("lstat error");
            exit(EXIT_FAILURE);
        }

        if(S_ISDIR(st.st_mode)){
            setPermisions(filename);
        }else if(S_ISREG(st.st_mode) || strstr(entry->d_name, ".c") != NULL){
            if (st.st_mode & S_IXUSR){
                printf("Revoc exec pentru: %s\n", filename);
                if (chmod(filename, st.st_mode & ~S_IXUSR) < 0){
                    perror("chmod error");
                }
            }
        }
    }

    if(closedir(dir) < 0){
        perror("closedir error");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]){
    if (argc != 2){
        printf("Usage: %s <director>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    setPermisions(argv[1]);
    return 0;
}
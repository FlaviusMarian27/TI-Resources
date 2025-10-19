/*
pentru fiecare fișier cu extensia “.c”, având dimensiunea > 13 KB (13312 biti),
se va crea o copie în <cale-backup> (creată la runtime: mkdir),
cu fiecare al 10-lea caracter modificat în “*”
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define LIMIT 13312

void process(const char* src, const char* dest){
    int fd_in = open(src, O_RDONLY);
    if(fd_in < 0){
        perror("open error");
        exit(EXIT_FAILURE);
    }

    int fd_out = open(dest, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if(fd_out < 0){
        perror("open error");
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    while((bytes_read = read(fd_in, buffer, BUFFER_SIZE)) > 0){
        for(int i = 0; i < bytes_read; i++){
            if((i + 1) % 10 == 0){
                buffer[i] = '*';
            }
        }
        
        write(fd_out, buffer, bytes_read);
    }

    if (close(fd_in) < 0){
        perror("close error");
        exit(EXIT_FAILURE);
    }

    if (close(fd_out) < 0){
        perror("close error");
        exit(EXIT_FAILURE);
    }
}

void readFromDirectory(const char* directory_path, const char* backup_file){
    DIR* dir = opendir(directory_path);
    if(dir == NULL){
        perror("opendir error");
        exit(EXIT_FAILURE);
    }

    struct dirent* entry;
    char fullpath[BUFFER_SIZE];
    char dest[BUFFER_SIZE];
    while((entry = readdir(dir)) != NULL){
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){
            continue;
        }

        snprintf(fullpath, BUFFER_SIZE, "%s/%s", directory_path, entry->d_name);
        struct stat st;
        if(lstat(fullpath, &st) < 0){
            perror("lstat error");
            exit(EXIT_FAILURE);
        }

        if(S_ISDIR(st.st_mode)){
            readFromDirectory(fullpath,backup_file);
        }else if(S_ISREG(st.st_mode) && strstr(entry->d_name, ".c") != NULL){
            if (st.st_size > LIMIT){
                snprintf(dest, BUFFER_SIZE, "%s/%s", backup_file, entry->d_name);
                process(fullpath, dest);
                printf("Backup file: %s -> %s\n", fullpath,dest);
            }
        }
    }

    if (closedir(dir) < 0){
        perror("closedir error");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]){
    if (argc != 3){
        printf("Usage: ./ex3 <directory> <file>\n");
        exit(EXIT_FAILURE);
    }

    mkdir(argv[2],0777);
    readFromDirectory(argv[1],argv[2]);
    return 0;
}
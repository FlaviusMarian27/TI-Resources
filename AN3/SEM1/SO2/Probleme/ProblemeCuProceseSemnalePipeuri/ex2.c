/*
2. Să se scrie un program C care primește ca și argument în linie de comandă calea 
către un director de pe disc. Programul va parcurge directorul primit ca și argument, 
va procesa în paralel toate fișierele obișnuite identificate, le va selecta doar pe 
acelea care au extensia ''.c'' și va invoca gcc pentru a le compila.
INDICIU: se va crea câte un proces copil pentru fiecare fișier obișnuit identificat 
cu extensia cerută și se va folosi execlp/execvp pentru a invoca gcc.
*/

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define THE_WAY_FILEPATH_SIZE 1024

void readFromDirectory(const char* directory_path){
    DIR* directory = opendir(directory_path);
    if(directory == NULL){
        fprintf(stderr, "Error opening directory %s\n", directory_path);
        exit(EXIT_FAILURE);
    }

    struct dirent* entry;
    char file_path[THE_WAY_FILEPATH_SIZE];
    while((entry = readdir(directory)) != NULL){
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){
            continue;
        }

        snprintf(file_path, THE_WAY_FILEPATH_SIZE, "%s/%s", directory_path, entry->d_name);
        struct stat st;
        if(stat(file_path, &st) < 0){
            fprintf(stderr, "Error stat %s\n", file_path);
            exit(EXIT_FAILURE);
        }

        if(S_ISDIR(st.st_mode)){
            readFromDirectory(file_path);
        }else if(S_ISREG(st.st_mode) && strstr(entry->d_name, ".c") != NULL){
            pid_t pid = fork();
            if (pid < 0){
                fprintf(stderr, "Error fork %s\n", file_path);
                exit(EXIT_FAILURE);
            }

            if (pid == 0){
                char exe_name[THE_WAY_FILEPATH_SIZE];
                strcpy(exe_name, entry->d_name);
                exe_name[strlen(exe_name) - 1] = '\0';

                execlp("gcc","gcc", "-Wall", "-o", exe_name, file_path, NULL);
                perror("error execlp");
                exit(EXIT_FAILURE);
            }
        }
    }

    if (closedir(directory) < 0){
        fprintf(stderr, "Error closing directory %s\n", directory_path);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    readFromDirectory(argv[1]);
    int status;
    while (wait(&status) > 0){
        if (WIFEXITED(status)){
            printf("exited with code %d\n", WEXITSTATUS(status));
        }
    }

    printf("Toate fișierele .c au fost compilate.\n");
    return 0;
}

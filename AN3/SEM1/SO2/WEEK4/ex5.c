/*
2. Să se scrie un program C care primește ca și argument în linie de comandă calea către un director de pe disc.
Programul va parcurge directorul primit ca și argument, va procesa în paralel toate fișierele obișnuite identificate, le va selecta doar pe acelea care au extensia ''.c'' și va invoca gcc pentru a le compila.
INDICIU: se va crea câte un proces copil pentru fiecare fișier obișnuit identificat cu extensia cerută și se va folosi execlp/execvp pentru a invoca gcc.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

void processingFiles(const char* directory){
    DIR *dir = opendir(directory);
    if(dir == NULL){
        fprintf(stderr, "Error opening directory %s\n", directory);
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    while((entry = readdir(dir)) != NULL){
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){
            continue;
        }

        char buffer[BUFFER_SIZE];
        snprintf(buffer, BUFFER_SIZE, "%s/%s", directory, entry->d_name);

        struct stat st;
        if(stat(buffer, &st) < 0){
            fprintf(stderr, "Error opening directory %s\n", directory);
            exit(EXIT_FAILURE);
        }

        if(S_ISDIR(st.st_mode)){
            processingFiles(buffer);
        }else if(S_ISREG(st.st_mode) && strstr(entry->d_name, ".c") != NULL){
            pid_t pid = fork();
            if (pid < 0){
                fprintf(stderr, "Error forking process\n");
                exit(EXIT_FAILURE);
            }

            if (pid == 0){
                char executable[BUFFER_SIZE];
                strcpy(executable, entry->d_name);
                executable[strlen(executable) - 2] = '\0';

                execlp("gcc","gcc","-Wall","-o",executable,buffer,NULL);
                perror("Error executing command");
                exit(EXIT_FAILURE);
            }
        }
    }

    if (closedir(dir) != 0){
        fprintf(stderr, "Error closing directory %s\n", directory);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, const char * argv[]){
    if (argc != 2){
        fprintf(stderr, "Usage: %s directory\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    processingFiles(argv[1]);

    int status = 0;
    while (wait(&status) != -1){
        if (WIFEXITED(status)){
            printf("exited with code %d\n", WEXITSTATUS(status));
        }
    }

    printf("Toate fisierele .c au fost executate!\n");
    return 0;
}

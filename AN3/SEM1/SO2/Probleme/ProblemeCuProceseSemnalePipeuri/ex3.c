#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

char* generator_parole(int len){
    char* new_password = (char*)malloc(sizeof(char)*(len+1));
    if(new_password == NULL){
        fprintf(stderr,"Out of memory\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < len; i = i + 1){
        char character;

        do{
          character = rand() % 127;
        }while (character <= 32 || character >= 127);

        new_password[i] = character;
    }

    new_password[len] = '\0';
    return new_password;
}

int main(int argc, char *argv[]){
    if (argc != 5){
        fprintf(stderr,"Usage: ./%s <numar_procese> <numar_parole> <dimensiune_parola> <cale_fisier_iesire>\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    /*
    char* password = generator_parole(atoi(argv[1]));
    printf("Password: %s\n", password);
    free(password);*/

    int numar_procese = atoi(argv[1]);
    int numar_parole = atoi(argv[2]);
    int dimensiune_parole = atoi(argv[3]);

    int fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0){
        fprintf(stderr,"Error opening file\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < numar_procese; i++){
        int pfd[2];
        if (pipe(pfd) < 0){
            fprintf(stderr,"Error creating pipe\n");
            exit(EXIT_FAILURE);
        }

        pid_t pid = fork();
        if (pid < 0){
            fprintf(stderr,"Error forking\n");
            exit(EXIT_FAILURE);
        }

        if (pid == 0){
            srand(time(NULL) ^ getpid());
            close(pfd[0]);

            for (int j = 0; j < numar_parole; j++){
                char* password = generator_parole(dimensiune_parole);
                write(pfd[1], password, strlen(password));
                write(pfd[1], "\n", 1);
                free(password);
            }

            close(pfd[1]);
            exit(EXIT_SUCCESS);
        }else{
            close(pfd[1]);

            char buffer[1024];
            ssize_t bytes_read;
            while ((bytes_read = read(pfd[0], buffer, 1024)) > 0){
                write(fd, buffer, bytes_read);
            }

            close(pfd[0]);

            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status)){
                printf("Child exited with status %d\n", WEXITSTATUS(status));
            }
        }
    }

    if (close(fd) != 0){
        fprintf(stderr,"Error closing file\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}
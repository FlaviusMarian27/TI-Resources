/*
Scrie un program C apelat astfel:

./program <fisier1> <fisier2>


Pentru fiecare fișier se va crea un proces copil.

Fiecare copil procesează fișierul primit și trimite printr-un pipe către părinte un mesaj de forma:

<nr_litere_mici> <nr_litere_mari> <nr_cifre>

Părintele colectează mesajele de la copii și afișează statisticile totale 
(adică suma tuturor valorilor primite).

Se recomandă folosirea:

fdopen() + fscanf() / fgets()


pentru a citi din pipe.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#define BUFFER_SIZE 4096
#define SIZE_RESULT 128

void processPath(const char *path, int capat_scriere){
    int fd = open(path, O_RDONLY);
    if (fd < 0){
        fprintf(stderr, "open() failed\n");
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    int lower_case = 0;
    int upper_case = 0;
    int digits = 0;

    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0){
        for (int i = 0; i < bytes_read; i++){
            if (isdigit(buffer[i])){
                digits = digits + 1;
            }else if (islower(buffer[i])){
                lower_case = lower_case + 1;
            }else if (isupper(buffer[i])){
                upper_case = upper_case + 1;
            }
        }
    }

    if (close(fd) < 0){
        fprintf(stderr, "close() failed\n");
        exit(EXIT_FAILURE);
    }

    FILE *out = fdopen(capat_scriere, "w");
    if (out==NULL){
        fprintf(stderr, "fdopen() failed\n");
        exit(EXIT_FAILURE);
    }

    //fprintf(out, "File: %s -> Lowarcase: %d, Uppercase: %d, Digits: %d\n",path, lower_case, upper_case, digits);

    fprintf(out, "%d %d %d\n", lower_case, upper_case, digits);

    if (fclose(out) < 0)
    {
        fprintf(stderr, "close() failed\n");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]){
    if (argc != 3){
        fprintf(stderr, "Usage: ./ex3 <file1> <file2>\n");
        exit(EXIT_FAILURE);
    }

    int pipefd[2];
    if (pipe(pipefd) < 0){
        fprintf(stderr, "pipe() failed\n");
        exit(EXIT_FAILURE);
    }



    for (int i = 1; i < 3; i++){
        pid_t pid = fork();
        if (pid < 0){
            fprintf(stderr, "fork() failed\n");
            exit(EXIT_FAILURE);
        }

        if (pid == 0){
            close(pipefd[0]);
            processPath(argv[i], pipefd[1]);
            close(pipefd[1]);
            exit(0);
        }
    }

    // parinte
    close(pipefd[1]);
    FILE *in = fdopen(pipefd[0], "r");
    if (in == NULL){
        fprintf(stderr, "fdopen() failed\n");
        exit(EXIT_FAILURE);
    }

    int total_lower_case = 0;
    int total_upper_case = 0;
    int total_digits = 0;

    int l,u,d;

    while (fscanf(in, "%d %d %d", &l, &u, &d) == 3){
        printf("Lowarcase: %d, Uppercase: %d, Digits: %d\n",l,u,d);
        total_lower_case = total_lower_case + l;
        total_upper_case = total_upper_case + u;
        total_digits = total_digits + d;
    }

    if (fclose(in) != 0){
        fprintf(stderr, "fclose() failed\n");
        exit(EXIT_FAILURE);
    }

    close(pipefd[0]);

    int status = 0;
    while (wait(&status) > 0){
        if (WIFEXITED(status)){
            printf("Child exited with code %d\n", WEXITSTATUS(status));
        }
    }

    printf("\nTOTAL -> Lowercase=%d, Uppercase=%d, Digits=%d\n",
           total_lower_case, total_upper_case, total_digits);

    return 0;
}
/*
Programul primește ca argument o cale către un director.
Pentru fiecare fișier obișnuit, creează un proces copil care:

- citește fișierul;
- calculează câte litere mici, litere mari și cifre are;
- trimite rezultatele printr-un pipe părintelui.

Părintele:
    - afișează totalurile curente la fiecare 3 secunde (cu alarm() și SIGALRM);
    - la primirea semnalului SIGUSR1, afișează totalurile finale și se oprește.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>
#include <dirent.h>

#define MAX_SIZE_BUFFER 4096

int total_lower_case = 0;
int total_upper_case = 0;
int total_digits = 0;
int stop = 0;

void handler_alarm(int sig){
    printf("\n[ALARM] Curent total: lowercase=%d uppercase=%d digits=%d\n",
           total_lower_case, total_upper_case, total_digits);

    fflush(stdout);
    alarm(3);
}

void handler_sigusr1(int sig){
    stop = 1;
    printf("Am primit semnalul SIGURS1!\n");
}

void countCharacters(const char *path, int write_fd){
    int fd = open(path, O_RDONLY);
    if(fd < 0){
        fprintf(stderr, "Error opening file %s\n", path);
        exit(EXIT_FAILURE);
    }

    int lower_case = 0;
    int upper_case = 0;
    int digits = 0;
    char buffer[MAX_SIZE_BUFFER];
    ssize_t nread;
    while((nread = read(fd, buffer, MAX_SIZE_BUFFER)) > 0){
        for(int i = 0; i < nread; i++){
            if (islower(buffer[i])){
                lower_case = lower_case + 1;
            }else if(isupper(buffer[i])){
                upper_case = upper_case + 1;
            }else if (isdigit(buffer[i])){
                digits = digits + 1;
            }
        }
    }

    if (close(fd) != 0){
        fprintf(stderr, "Error closing file %s\n", path);
        exit(EXIT_FAILURE);
    }

    FILE *fin = fdopen(write_fd, "w");
    if(fin == NULL){
        fprintf(stderr, "Error opening file %s\n", path);
        exit(EXIT_FAILURE);
    }

    fprintf(fin,"%s %d %d %d\n",path,lower_case,upper_case,digits);

    if (fclose(fin) != 0){
        fprintf(stderr, "Error closing file %s\n", path);
        exit(EXIT_FAILURE);
    }
}

void readFromDirectory(const char* directory, int read_fd, int write_fd){
    DIR* dir = opendir(directory);
    if(dir == NULL){
        fprintf(stderr, "Error opening directory %s\n", directory);
        exit(EXIT_FAILURE);
    }

    struct dirent* entry;
    while((entry = readdir(dir)) != NULL){
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){
            continue;
        }

        char buffer[MAX_SIZE_BUFFER];
        snprintf(buffer, MAX_SIZE_BUFFER, "%s/%s", directory, entry->d_name);

        struct stat st;
        if(lstat(buffer, &st) < 0){
            fprintf(stderr, "Error stat %s\n", buffer);
            exit(EXIT_FAILURE);
        }

        if(S_ISDIR(st.st_mode)){
            readFromDirectory(buffer,read_fd,write_fd);
        }else if(S_ISREG(st.st_mode)){
            //printf("%s\n", buffer);
            pid_t pid = fork();
            if(pid < 0){
                fprintf(stderr, "Error fork %s\n", buffer);
                exit(EXIT_FAILURE);
            }

            if(pid == 0){
                close(read_fd);
                countCharacters(buffer,write_fd);
                close(write_fd);
                exit(0);
            }
        }
    }

    if (closedir(dir) != 0){
        fprintf(stderr, "Error closing directory %s\n", directory);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]){
    if (argc != 2){
        fprintf(stderr,"Usage: %s <directory>\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    int pipefd[2];
    if(pipe(pipefd) < 0){
        fprintf(stderr, "Error creating pipe\n");
        exit(EXIT_FAILURE);
    }

    struct sigaction sa_alarm;
    memset(&sa_alarm, 0, sizeof(struct sigaction));
    sa_alarm.sa_handler = handler_alarm;
    sigemptyset(&sa_alarm.sa_mask);
    sa_alarm.sa_flags = SA_RESTART;

    if (sigaction(SIGALRM, &sa_alarm, NULL) < 0){
        fprintf(stderr, "Error setting alarm\n");
        exit(EXIT_FAILURE);
    }

    struct sigaction sa_sigusr1;
    memset(&sa_sigusr1, 0, sizeof(struct sigaction));
    sa_sigusr1.sa_handler = handler_sigusr1;
    sigemptyset(&sa_sigusr1.sa_mask);
    sa_sigusr1.sa_flags = SA_RESTART;

    if (sigaction(SIGUSR1, &sa_sigusr1, NULL) < 0){
        fprintf(stderr, "Error setting alarm\n");
        exit(EXIT_FAILURE);
    }

    alarm(3); //prima alarma

    readFromDirectory(argv[1],pipefd[0],pipefd[1]);

    //parinte
    close(pipefd[1]);

    FILE* fin = fdopen(pipefd[0],"r");
    if(fin == NULL){
        fprintf(stderr, "Error opening file %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    char name_path[MAX_SIZE_BUFFER];
    int l,u,d;

    while (!stop ){
        if (fscanf(fin,"%s %d %d %d",name_path,&l,&u,&d) == 4){
            //printf("File %s -> Lowercase: %d Uppercase: %d Digits:%d\n",name_path,l,u,d);
            total_lower_case = total_lower_case + l;
            total_upper_case = total_upper_case + u;
            total_digits = total_digits + d;
        }else{
            pause();
        }
    }

    if (fclose(fin) != 0){
        fprintf(stderr, "Error closing file %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    close(pipefd[0]);
    int status;
    while (wait(&status) > 0){
        if (WIFEXITED(status)){
            fprintf(stderr, "Child exited with status %d\n", WEXITSTATUS(status));
        }
    }

    printf("\nTOTAL: -> Lowercase: %d Uppercase: %d Digits: %d\n",
        total_lower_case,total_upper_case,total_digits);

    return 0;
}
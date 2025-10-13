//Creează un proces nou, aproape identic cu cel curent.
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

int main(int argc, char* argv[]){
    printf("Inainte de fork!\n");

    pid_t pid = fork();
    if(pid < 0){
        perror("Eroare la fork!\n");
        exit(-1);
    }

    if(pid == 0){
        //este fiul
        printf("Sunt fiul! PID-ul meu este %d si PID-ul parintelui este %d!\n",getpid(),getppid());
    }else{
        printf("Sunt parintele! PID-ul fiului este %d si PID-ul meu este %d!\n",getppid(),getpid());
    }

    printf("Procesul %d se termina!\n",getpid());

    return 0;
}
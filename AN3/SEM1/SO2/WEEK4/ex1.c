/*
    int a = 5;
    procesul copil a++ printf
    procesul parinte a-- printf
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void){

    for (int i = 0; i < 10; i = i + 1){
        pid_t pid;
        if((pid = fork()) < 0){
            perror("fork");
            exit(EXIT_FAILURE);
        }
        int a = 5;
        if(pid == 0){
            a = a + 1;
            printf("Child\t%d\n",a);
            exit(0);
        }else{
            a = a - 1;
            printf("Parent\t%d\n",a);
            wait(NULL);
        }
    }

    return 0;
}
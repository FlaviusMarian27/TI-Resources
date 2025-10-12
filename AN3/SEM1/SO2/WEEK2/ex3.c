/*
student@b528b-16:~/lab2_so2$ ./p ex1.c ex2.c input.txt input1.txt input2.txt output.txt output2.txt p .
Permisiniu: rw-
Permisiniu: rw-
Permisiniu: rw-
Permisiniu: rw-
Permisiniu: rwx
Permisiniu: rwx
student@b528b-16:~/lab2_so2$ 
*/
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/stat.h>

#define MAX_SIZE 4096

int main(int argc, char* argv[]){
    if(argc < 2){
        perror("Trebuie program <file.in> ... \n");
        exit(1);
    }

    struct stat st;
    for(int i = 1; i < argc; i = i + 1){
        if(stat(argv[i],&st) < 0){
            perror("Eroare la stat!\n");
            exit(1);
        }

        if(st.st_size > 1024){
            printf("Permisiniu: ");
            if(st.st_mode & S_IRUSR){
                printf("r");
            }else{
                printf("-");
            }

            if(st.st_mode & S_IWUSR){
                printf("w");
            }else{
                printf("-");
            }

            if(st.st_mode & S_IXUSR){
                printf("x");
            }else{
                printf("-");
            }
            printf("\n");
        }
    }
    return 0;
}
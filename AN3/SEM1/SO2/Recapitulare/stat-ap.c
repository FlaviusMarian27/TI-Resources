//afisare informatii despre fisier
#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<time.h>

int main(int argc, char* argv[]){
    if(argc != 2){
        printf("Foloseste ./executabil <fisier>\n");
        exit(-1);
    }

    struct stat st;
    if(stat(argv[1],&st) < 0){
        perror("Eroare la stat\n");
        exit(-1);
    }

    printf("Dimensiune: %ld bytes\n",st.st_size);
    printf("User ID: %d\n",st.st_uid);
    printf("Group ID: %d\n",st.st_gid);
    printf("Numar linkuri: %ld\n",st.st_nlink);
    printf("Ultima modificare: %s\n",ctime(&st.st_mtime));

    if(S_ISREG(st.st_mode)){
        printf("Este fisier simplu!\n");
    }else if(S_ISDIR(st.st_mode)){
        printf("Este director!\n");
    }else if(S_ISLNK(st.st_mode)){
        printf("Este legatura simbolica!\n");
    }else{
        printf("Este altceva!\n");
    }

    return 0;
}
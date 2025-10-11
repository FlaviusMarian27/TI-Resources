/*
5. Să se scrie un program care primește un număr variabil de opțiuni din linia de comandă, urmate de minim 2 căii către intrări de pe disc.
În funcție de opțiunile primite, programul va afișa anumite informații pentru fiecare dintre intrările primite ca și argumente.
Opțiunile pot fi următoarele:
-i => afișare număr inode
-l => afișare număr de legături fizice
-u => afișare owner ID
-s => afișare dimensiune în octeți
-t => afișare data ultimei modificări
-a => afișare data ultimului acces
-pu => afișare permisiuni owner (format: rwx și -)
-pg => afișare permisiuni group (format: rwx și -)
-po => afișare permisiuni other (format: rwx și -)
-c => afișare conținut dacă este fișier, tipul intrării în caz contrar
Exemplu:
<program> -s -pu fisier1 fisier2 
fisier1   1234 bytes   rw-
fisier2   345 bytes     r--
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

#define SIZE 1024

int main(int argc, char* argv[]){
    
    int show_inode = 0;
    int show_links = 0;
    int show_ownerID = 0;
    int show_size = 0;
    int show_last_mod = 0;
    int show_last_access = 0;
    int show_per_owner = 0;
    int show_per_group = 0;
    int show_per_other = 0;
    int show_cont = 0;

    int first_file = 0;
    for(int i = 1; i < argc; i = i + 1){
        if(argv[i][0] != '-'){
            first_file = i;
            break;
        }
        if(strcmp(argv[i],"-i") == 0){
            show_inode = 1;
        }else if(strcmp(argv[i],"-l") == 0){
            show_links = 1;
        }else if(strcmp(argv[i],"-u") == 0){
            show_ownerID = 1;
        }else if(strcmp(argv[i],"-s") == 0){
            show_size = 1;
        }else if(strcmp(argv[i],"-t") == 0){
            show_last_mod = 1;
        }else if(strcmp(argv[i],"-a") == 0){
            show_last_access = 1;
        }else if(strcmp(argv[i],"-pu") == 0){
            show_per_owner = 1;
        }else if(strcmp(argv[i],"-pg") == 0){
            show_per_group = 1;
        }else if(strcmp(argv[i],"-po") == 0){
            show_per_other = 1;
        }else if(strcmp(argv[i],"-c") == 0){
            show_cont = 1;
        }else{
            fprintf(stderr, "Optiune necunoscuta: %s\n", argv[i]);
            exit(1);
        }
        
    }

    if(argc - first_file < 2){
        fprintf(stderr, "Trebuie cel putin doua cai catre fisiere/directoare!\n");
        exit(1);
    }

    for(int i = first_file; i < argc; i = i + 1){
        struct stat st;
        if(stat(argv[i],&st) < 0){
            fprintf(stderr, "Eroare la stat %s!\n",argv[i]);
            exit(1);
        }

        printf("%s\n",argv[i]);
        if(show_inode == 1){
            printf("Numar inod: %ld\n",st.st_ino);
        }
        
        if(show_links == 1){
            printf("Numar lnk: %ld\n",st.st_nlink);
        }
        
        if(show_ownerID == 1){
            printf("Owner id: %d\n",st.st_uid);
        }
        
        if(show_size == 1){
            printf("Dim: %ld bytes\n",st.st_size);
        }
        
        if(show_per_owner == 1){
            printf("Drp owner: ");
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
                printf("x\n");
            }else{
                printf("-\n");
            }
        }
        
        if(show_per_group == 1){
            printf("Drp group: ");
            if(st.st_mode & S_IRGRP){
                printf("r");
            }else{
                printf("-");
            }

            if(st.st_mode & S_IWGRP){
                printf("w");
            }else{
                printf("-");
            }

            if(st.st_mode & S_IXGRP){
                printf("x\n");
            }else{
                printf("-\n");
            }
        }
        
        if(show_per_other == 1){
            printf("Drp other: ");
            if(st.st_mode & S_IROTH){
                printf("r");
            }else{
                printf("-");
            }

            if(st.st_mode & S_IWOTH){
                printf("w");
            }else{
                printf("-");
            }

            if(st.st_mode & S_IXOTH){
                printf("x\n");
            }else{
                printf("-\n");
            }
        }
        
        if(show_last_mod == 1){
            printf("Ultima modificare: ");
            printf("%s ",ctime(&st.st_mtime));
        }
        
        if(show_last_access == 1){
            printf("Ultimul acces: ");
            printf("%s ",ctime(&st.st_atime));
        }

        if(show_cont == 1){
            if(S_ISREG(st.st_mode)){
                int fd = open(argv[i],O_RDONLY);
                if(fd < 0){
                    perror("Eroare la deschiderea fisierului!\n");
                    exit(1);
                }

                char buffer[SIZE];
                ssize_t bytesRead;
                while((bytesRead = read(fd,buffer,SIZE)) > 0){
                    write(STDOUT_FILENO,buffer,bytesRead);
                }

                printf("\n");
                if(close(fd) != 0){
                    perror("Eroare la inchiderea fisierului!\n");
                    exit(1);
                }
            }else if(S_ISDIR(st.st_mode)){
                printf("DIR\n");
            }else if(S_ISLNK(st.st_mode)){
                printf("LINK\n");
            }else if(S_ISFIFO(st.st_mode)){
                printf("PIPE\n");
            }else{
                printf("OTHER\n");
            }
        }

        printf("\n====================\n");
    }
    return 0;
}
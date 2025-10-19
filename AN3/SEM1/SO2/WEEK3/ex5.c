/*
sa se verifice toate regular files care au drepturi de read pt other si sa se
stearga de pe disk cu unlink
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

void setPermisions(const char* director_path){
    DIR* dir = opendir(director_path);
    if(dir == NULL){
        perror("opendir error");
        exit(EXIT_FAILURE);
    }

    struct dirent* entry;
    char buffer[BUFFER_SIZE];
    while((entry = readdir(dir)) != NULL){
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){
            continue;
        }

        snprintf(buffer, BUFFER_SIZE, "%s/%s", director_path, entry->d_name);

        struct stat st;
        if(stat(buffer, &st) == -1){
            perror("stat error");
            exit(EXIT_FAILURE);
        }

        if(S_ISDIR(st.st_mode)){
            setPermisions(buffer);
        }else if(S_ISREG(st.st_mode)){
            if (st.st_mode & S_IROTH){
                if (unlink(buffer) == 0){
                    printf("Delete %s!\n",buffer);
                }else{
                    printf("Error deleting %s!\n",buffer);
                }
            }
        }
    }

    if (closedir(dir) < 0)
    {
        perror("closedir error");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]){
    if(argc != 2){
        printf("Usage: %s <pathname>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    setPermisions(argv[1]);
    return 0;
}
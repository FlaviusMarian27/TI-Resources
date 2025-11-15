#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>

#define BUFSIZE 1024
#define SUPER_BUFSIZE 4096

int flag = 0;

void handler_sigusr1(int sig){
    (void)sig;
    flag = 1;
    printf("SIGUSR1 received\n");
    //exit(0);
}

void wc_funct(const char *path, int write_fd){
  if(dup2(write_fd, STDOUT_FILENO) < 0){
    fprintf(stderr, "dup2 failed\n");
    exit(EXIT_FAILURE);
  }

  execlp("wc", "wc", path,NULL);
  perror("execlp failed");
  exit(EXIT_FAILURE);
}

void readFromDirectory(const char* name_dir, int read_fd, int write_fd) {
  DIR* dir = opendir(name_dir);
  if(dir == NULL){
    fprintf(stderr, "Error opening directory %s\n", name_dir);
    exit(EXIT_FAILURE);
  }

  struct dirent* entry;
  while((entry = readdir(dir)) != NULL){
    if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){
      continue;
    }

    char filename[BUFSIZE];
    snprintf(filename, BUFSIZE, "%s/%s", name_dir, entry->d_name);

    struct stat st;
    if(lstat(filename, &st) != 0){
      fprintf(stderr, "Error stating file %s\n", filename);
      exit(EXIT_FAILURE);
    }

    if(S_ISDIR(st.st_mode)){
      readFromDirectory(filename,read_fd,write_fd);
    }else if(S_ISREG(st.st_mode) && strstr(entry->d_name, ".txt") != NULL){
       pid_t pid = fork();
       if(pid < 0){
         fprintf(stderr, "Error forking process\n");
         exit(EXIT_FAILURE);
       }

      if(pid == 0){
        close(read_fd);
        struct sigaction ing;
        memset(&ing, 0, sizeof(ing));
        ing.sa_handler = SIG_IGN;
        sigemptyset(&ing.sa_mask);

        if(sigaction(SIGUSR1, &ing, NULL) < 0){
            fprintf(stderr, "Error SIG_IGN handler\n");
            exit(EXIT_FAILURE);
        }

        wc_funct(filename, write_fd);
        close(write_fd);
        exit(EXIT_SUCCESS);
      }
    }
  }

  if(closedir(dir) != 0){
    fprintf(stderr, "Error closing directory %s\n", name_dir);
    exit(EXIT_FAILURE);
  }
}

int main(int argc, const char * argv[]) {
  if(argc != 2){
    fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int pipefd[2];
  if(pipe(pipefd) < 0){
    fprintf(stderr, "Error creating pipe\n");
    exit(EXIT_FAILURE);
  }

  struct sigaction sig1;
  memset(&sig1, 0, sizeof(sig1));
  sig1.sa_handler = handler_sigusr1;
  sigemptyset(&sig1.sa_mask);
  if(sigaction(SIGUSR1, &sig1, NULL) < 0){
    fprintf(stderr, "Error signaling handler\n");
    exit(EXIT_FAILURE);
  }

  while(flag != 1){
    printf("waiting for signal - %d\n",getpid());
    sleep(2);
  }

  int total_new_line = 0;
  int total_word = 0;
  int total_bytes = 0;

  int n,w,b;
  char name_path[BUFSIZE];

  readFromDirectory(argv[1], pipefd[0], pipefd[1]);
  close(pipefd[1]);

  FILE* fin = fdopen(pipefd[0], "r");
  if(fin == NULL){
    fprintf(stderr, "Error opening file\n");
    exit(EXIT_FAILURE);
  }

  while(fscanf(fin, "%d %d %d %s", &n, &w, &b, name_path) == 4){
    printf("%d\t%d\t%d\t%s\n", n, w, b, name_path);
    total_new_line += n;
    total_word += w;
    total_bytes += b;
  }

  if(fclose(fin) != 0){
    fprintf(stderr, "Error closing file\n");
    exit(EXIT_FAILURE);
  }

  close(pipefd[0]);
  int status = 0;
  while(wait(&status) > 0){
    if(WEXITSTATUS(status) != EXIT_SUCCESS){
      fprintf(stderr, "Error executing process\n");
      exit(EXIT_FAILURE);
    }

    if(WIFEXITED(status)){
      printf("Procesul fiu cu PID-ul %d s-a terminat cu valoarea returnata %d\n",
               getpid(), WEXITSTATUS(status));
    }
  }

  printf("numar total linii:\t%d\nnumar total cuvinte:\t%d\nnumar total bytes:\t%d\n",
           total_new_line, total_word, total_bytes);
  printf("Succes!\n");


  return 0;
}
/*
numar total linii:      3962
numar total cuvinte:    208840
numar total bytes:      1500458
Succes!
 */
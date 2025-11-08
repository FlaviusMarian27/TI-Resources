#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

int main(int argc, char *argv[]){
    if (argc != 3){
        fprintf(stderr, "Usage: ./producer <file> <num>\n");
        exit(EXIT_FAILURE);
    }

    const char* fifo_name = argv[1];
    int N = atoi(argv[2]);

    int fd = open(fifo_name, O_WRONLY);
    if (fd < 0){
        fprintf(stderr, "Error opening fifo %s\n", fifo_name);
        exit(EXIT_FAILURE);
    }


    srand(time(NULL));
    for (int i = 0; i < N; i++){
        int val = rand() % 100;
        write(fd, &val, sizeof(val));
        printf("Producer send %d\n", val);
    }

    if (close(fd) != 0){
        fprintf(stderr, "Error closing fifo %s\n", fifo_name);
        exit(EXIT_FAILURE);
    }
    return 0;
}
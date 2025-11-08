#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]){
    if (argc != 2){
        fprintf(stderr, "Usage: %s <fifo_name>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *fifo_name = argv[1];
    int fd = open(fifo_name, O_RDONLY);
    if (fd < 0){
        fprintf(stderr, "Error opening fifo %s\n", fifo_name);
        exit(EXIT_FAILURE);
    }

    int val = 0;
    int sum = 0;
    while (read(fd, &val, sizeof(val)) > 0)
    {
        printf("Consumer got %d\n", val);
        sum += val;
    }

    printf("Total sum = %d\n", sum);

    if (close(fd) != 0){
        fprintf(stderr, "Error closing fifo %s\n", fifo_name);
        exit(EXIT_FAILURE);
    }

    return 0;
}
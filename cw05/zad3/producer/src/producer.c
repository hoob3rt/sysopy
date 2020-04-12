#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char** argv) {
    if(argc != 4) {
        perror("Wrong number of arguments\n");
        exit(1);
    }
    srand(time(NULL));
    int fifo = open(argv[1], O_WRONLY);
    if(fifo == -1) {
        perror("opening of file failed");
        exit(1);
    }
    FILE* input_file = fopen(argv[2], "r");
    if(input_file == NULL) {
        perror("opening of file failed");
        exit(1);
    }
    char* file_buffer = calloc(atoi(argv[3]) + 1, sizeof(char));
    char* write_buffer = calloc(atoi(argv[3]) + 1, sizeof(char));
    while(fread(file_buffer, sizeof(char), atoi(argv[3]) + 1, input_file) ==
          atoi(argv[3]) + 1) {
        sprintf(write_buffer, "#%d#%s\n", getpid(), file_buffer);
        write(fifo, write_buffer, strlen(write_buffer));
        sleep(1+rand()%2);
    }
    free(file_buffer);
    free(write_buffer);
    close(fifo);
    fclose(input_file);

    return 0;
}

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

void file_error(FILE* f) {
    if(f == NULL) {
        perror("opening of file failed");
        exit(1);
    }
    return;
}

int main(int argc, char** argv) {
    if(argc != 4) {
        perror("Wrong number of arguments\n");
        exit(1);
    }
    FILE* fifo = fopen(argv[1], "r");
    file_error(fifo);
    FILE* output_file = fopen(argv[2], "w");
    file_error(output_file);
    char* buffer = calloc(atoi(argv[3]) + 1, sizeof(char));
    int ppid;
    while(fscanf(fifo, "#%d#%s\n", &ppid, buffer) != EOF) {
        fprintf(output_file, "%s\n", buffer);
    }
    free(buffer);
    fclose(fifo);
    fclose(output_file);
    return 0;
}

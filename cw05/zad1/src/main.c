#define _POSIX_C_SOURCE 2
#define MAX_LINE_LENGTH 100
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char** argv) {
    if(argc == 2) {
        char* buffer = calloc(MAX_LINE_LENGTH, sizeof(char));
        char* file_path;
        int line;
        sprintf(file_path =
                    calloc(strlen("sort %s") + strlen(argv[1]), sizeof(char)),
                "sort %s", argv[1]);
        FILE* file = popen(file_path, "r");
        while((line = fread(buffer, sizeof(char), MAX_LINE_LENGTH, file))) {
            write(STDOUT_FILENO, buffer, line);
        }
        free(file_path);
        free(buffer);
    }
    return 0;
}

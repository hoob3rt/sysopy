#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

char** parse_command(char* line) {
    static char* command[10];
    int i = 0;
    while(line != NULL && i < 10) {
        char* arg = strsep(&line, " \n");
        if(*arg) {
            command[i++] = arg;
        }
    }
    while(i < 10) {
        command[i++] = NULL;
    }
    return command;
}

int main(int argc, char** argv) {
    if(argc != 2) {
        exit(1);
    }
    FILE* commands_file = fopen(argv[1], "r");
    int current_commands[2], previous_commands[2] = {-1, -1};
    char *line = NULL, *process_info = NULL;
    size_t line_len = 0;
    while(getline(&line, &line_len, commands_file) > 0) {
        while((process_info = strsep(&line, "|")) != NULL) {
            pipe(current_commands);
            if(fork() == 0) {
                for(char** __x = parse_command(process_info);;
                    (execvp(__x[0], __x) != -1) || (exit(1), 1)) {
                    dup2((close(previous_commands[1]), previous_commands[0]),
                         STDIN_FILENO);
                    if(line != NULL) {
                        dup2((close(current_commands[0]), current_commands[1]),
                             STDOUT_FILENO);
                    }
                }
            }
            previous_commands[0] =
                (close(current_commands[1]), current_commands[0]);
            previous_commands[1] = current_commands[1];
        }
        while(wait(NULL) > 0) {
            ;
        }
    }
    return 0;
}

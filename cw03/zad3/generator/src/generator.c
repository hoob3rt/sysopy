#include "../../src/matrix_func.h"

#include <linux/limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void dump_matrices_to_files(char** argv, char* path) {
    srand(time(NULL));
    int min = atoi(argv[2]);
    int max = atoi(argv[3]);
    int count = atoi(argv[4]);
    char* call = calloc(15 + strlen(path), sizeof(char));
    sprintf(call, "mkdir -p %s/matrices", path);
    system(call);
    free(call);
    for(int i = 0; i < count; i++) {
        int first_rows = rand() % (max - min + 1) + min;
        int first_columns = rand() % (max - min + 1) + min;
        int second_columns = rand() % (max - min + 1) + min;
        char* first_name = calloc(100, sizeof(char));
        char* second_name = calloc(100, sizeof(char));
        char* result_name = calloc(100, sizeof(char));
        sprintf(first_name, "%s/matrices/first%d", path, i);
        sprintf(second_name, "%s/matrices/second%d", path, i);
        sprintf(result_name, "%s/matrices/result%d", path, i);
        generate_matrix(first_rows, first_columns, first_name);
        generate_matrix(first_columns, second_columns, second_name);
        char* command = calloc(1000, sizeof(char));
        sprintf(command, "echo \"%s %s %s\" >> %s/lista", first_name, second_name,
                result_name, path);
        system(command);
        free(command);
        free(first_name);
        free(second_name);
        free(result_name);
    }
}

int main(int argc, char** argv) {
    dump_matrices_to_files(argv, argv[1]);
    return 0;
}

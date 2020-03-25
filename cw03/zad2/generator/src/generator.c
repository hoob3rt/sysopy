#include "../../src/matrix_func.h"

#include <linux/limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
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
        int a_rows = rand() % (max - min + 1) + min;
        int a_cols = rand() % (max - min + 1) + min;
        int b_cols = rand() % (max - min + 1) + min;
        char* a_name = calloc(100, sizeof(char));
        char* b_name = calloc(100, sizeof(char));
        char* c_name = calloc(100, sizeof(char));
        sprintf(a_name, "%s/matrices/a%d.txt", path, i);
        sprintf(b_name, "%s/matrices/b%d.txt", path, i);
        sprintf(c_name, "%s/matrices/c%d.txt", path, i);

        generate_matrix(a_rows, a_cols, a_name);
        generate_matrix(a_cols, b_cols, b_name);

        char* command = calloc(1000, sizeof(char));
        sprintf(command, "echo \"%s %s %s\" >> %s/lista", a_name, b_name,
                c_name, path);
        system(command);
        free(command);
        free(a_name);
        free(b_name);
        free(c_name);
    }
}

int main(int argc, char** argv) {
    dump_matrices_to_files(argv, argv[1]);
    return 0;
}

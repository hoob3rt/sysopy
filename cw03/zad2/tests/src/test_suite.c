#include "../../src/matrix_func.h"

#include <linux/limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

bool check_multiply_correctness(char* first_filename, char* second_filename,
                                char* result_filename) {
    matrix a = load_matrix(first_filename);
    matrix b = load_matrix(second_filename);
    matrix result = load_matrix(result_filename);
    matrix correct_matrix = multiply_matrices(a, b);
    if(correct_matrix.columns != result.columns ||
       correct_matrix.rows != result.rows) {
        return false;
    }
    for(int i = 0; i < correct_matrix.rows; i++) {
        for(int j = 0; j < correct_matrix.columns; j++) {
            if(correct_matrix.values[i][j] != result.values[i][j])
                return false;
        }
    }
    return true;
}

int main(int argc, char** argv) {
    char first_filename[PATH_MAX + 1];
    char second_filename[PATH_MAX + 1];
    char result_filename[PATH_MAX + 1];
    strcpy(first_filename, argv[1]);
    strcpy(second_filename, argv[2]);
    strcpy(result_filename, argv[3]);
    if(!check_multiply_correctness(first_filename, second_filename,
                                   result_filename)) {
        return 1;
    }
    return 0;
}

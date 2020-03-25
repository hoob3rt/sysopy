#ifndef MATRIX_FUNC_H
#define MATRIX_FUNC_H value

#include <stdio.h>

typedef struct matrix {
    int** values;
    int rows;
    int columns;
} matrix;

matrix load_matrix(char* filename);
matrix multiply_matrices(matrix a, matrix b);
void generate_matrix(int rows, int columns, char* filename);
void dump_matrix_to_file(FILE* file, matrix a);

#endif /* ifndef MATRIX_FUNC_H */

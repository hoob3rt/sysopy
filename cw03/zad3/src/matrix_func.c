#define _XOPEN_SOURCE 500
#define MAX_COLS_NUMBER 1000
#define MAX_LINE_LENGTH (MAX_COLS_NUMBER * 5)

#include "matrix_func.h"

#include <linux/limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int get_columns_count(char* row) {
    int columns = 0;
    char* current = strtok(row, " ");
    while(current != NULL) {
        if(strcmp(current, "\n") != 0) {
            columns++;
        }
        current = strtok(NULL, " ");
    }
    return columns;
}

void set_columns_and_rows_number(FILE* file, int* rows, int* columns) {
    char line[MAX_LINE_LENGTH];
    (*rows) = 0;
    (*columns) = 0;
    while(fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        if((*columns) == 0) {
            (*columns) = get_columns_count(line);
        }
        (*rows) = (*rows) + 1;
    }
    fseek(file, 0, SEEK_SET);
}

matrix load_matrix(char* filename) {
    FILE* file = fopen(filename, "r");
    int rows, columns;
    set_columns_and_rows_number(file, &rows, &columns);
    int** values = calloc(rows, sizeof(int*));
    for(int y = 0; y < rows; y++) {
        values[y] = calloc(columns, sizeof(int));
    }
    int x_curr, y_curr = 0;
    char line[MAX_LINE_LENGTH];
    while(fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        x_curr = 0;
        char* number = strtok(line, " ");
        while(number != NULL) {
            values[y_curr][x_curr++] = atoi(number);
            number = strtok(NULL, " ");
        }
        y_curr++;
    }
    fclose(file);
    matrix result_matrix;
    result_matrix.values = values;
    result_matrix.rows = rows;
    result_matrix.columns = columns;
    return result_matrix;
}

matrix multiply_matrices(matrix a, matrix b) {
    int** values = calloc(a.rows, sizeof(int*));
    for(int y = 0; y < a.rows; y++) {
        values[y] = calloc(b.columns, sizeof(int));
    }
    for(int i = 0; i < a.rows; i++) {
        for(int j = 0; j < b.columns; j++) {
            int result = 0;
            for(int k = 0; k < a.columns; k++) {
                result += (a.values[i][k] * b.values[k][j]);
            }
            values[i][j] = result;
        }
    }
    matrix result_matrix;
    result_matrix.values = values;
    result_matrix.rows = a.rows;
    result_matrix.columns = b.columns;
    return result_matrix;
}

void generate_matrix(int rows, int columns, char* filename) {
    FILE* file = fopen(filename, "w+");
    for(int y = 0; y < rows; y++) {
        for(int x = 0; x < columns; x++) {
            if(x > 0) {
                fprintf(file, " ");
            }
            fprintf(file, "%d", rand() % (200 + 1) - 100);
        };
        fprintf(file, "\n");
    }
    fclose(file);
}

void dump_matrix_to_file(FILE* file, matrix m) {
    fseek(file, 0, SEEK_SET);
    for(int y = 0; y < m.rows; y++) {
        for(int x = 0; x < m.columns; x++) {
            if(x > 0) {
                fprintf(file, " ");
            }
            fprintf(file, "%d", m.values[y][x]);
        };
        fprintf(file, "\n");
    }
}

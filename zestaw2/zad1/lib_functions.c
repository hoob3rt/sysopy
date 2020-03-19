#include "lib_functions.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <zconf.h>

void copy_lib(char* src_path, char* dest_path, int64_t record_count,
              int64_t record_len) {
    FILE* src_file = fopen(src_path, "r");
    FILE* dest_file = fopen(dest_path, "w+");
    char* record = malloc(record_len * sizeof(char));
    for(int64_t i = 0; i < record_count; i++) {
        if(fread(record, sizeof(char), (size_t)(record_len + 1), src_file) !=
           (record_len + 1)) {
            free(record);
            exit(1);
        }
        if(fwrite(record, sizeof(char), (size_t)(record_len + 1), dest_file) !=
           (record_len + 1)) {
            free(record);
            exit(1);
        }
    }
    fclose(src_file);
    fclose(dest_file);
    free(record);
};

int64_t load_line_lib(FILE* file, int64_t line, int64_t line_len,
                      char* buffer) {
    int64_t offset = (line_len + 1) * sizeof(char);
    fseek(file, line * offset, 0);
    return fread(buffer, sizeof(char), line_len + 1, file);
}

int64_t write_line_lib(FILE* file, int64_t line, int64_t line_len,
                       char* buffer) {
    int64_t offset = (line_len + 1) * sizeof(char);
    fseek(file, line * offset, 0);
    return fwrite(buffer, sizeof(char), line_len + 1, file);
}

int64_t partition_lib(FILE* file, char* line, char* pivot, int64_t low,
                      int64_t high, int64_t line_len) {
    int64_t i = low - 1;
    load_line_lib(file, high, line_len, pivot);
    for(int64_t j = low; j < high; j++) {
        load_line_lib(file, j, line_len, line);
        if(line[0] < pivot[0]) {
            i++;
            load_line_lib(file, i, line_len, pivot);
            write_line_lib(file, j, line_len, pivot);
            write_line_lib(file, i, line_len, line);
            load_line_lib(file, high, line_len, pivot);
        }
    }
    load_line_lib(file, i + 1, line_len, line);
    write_line_lib(file, i + 1, line_len, pivot);
    write_line_lib(file, high, line_len, line);
    return i + 1;
}

void qsort_lib(FILE* file, char* line, char* pivot, int64_t low, int64_t high,
               int64_t line_len) {
    if(low < high) {
        int64_t pi = partition_lib(file, line, pivot, low, high, line_len);

        qsort_lib(file, line, pivot, low, pi - 1, line_len);
        qsort_lib(file, line, pivot, pi + 1, high, line_len);
    }
}

int64_t sort_wrapper_lib(char* path, int64_t lines, int64_t line_len) {
    FILE* file = fopen(path, "r+");
    char* line = calloc(line_len + 1, sizeof(char));
    char* pivot = calloc(line_len + 1, sizeof(char));
    qsort_lib(file, line, pivot, 0, lines - 1, line_len);
    free(line);
    free(pivot);
    return 0;
}

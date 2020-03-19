#include "sys_functions.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <zconf.h>

int64_t load_line_sys(int64_t file, int64_t line, int64_t line_len,
                      char* buf) {
    int64_t offset = (line_len + 1) * sizeof(char);
    lseek(file, line * offset, SEEK_SET);
    return read(file, buf, offset);
}

int64_t write_line_sys(int64_t file, int64_t line, int64_t line_len,
                       char* buf) {
    int64_t offset = (line_len + 1) * sizeof(char);
    lseek(file, line * offset, SEEK_SET);
    return write(file, buf, offset);
}

int64_t partition_sys(int64_t file, char* line, char* pivot, int64_t low,
                      int64_t high, int64_t line_len) {
    int64_t i = low - 1;
    load_line_sys(file, high, line_len, pivot);
    for(int64_t j = low; j < high; j++) {
        load_line_sys(file, j, line_len, line);
        if(line[0] < pivot[0]) {
            i++;
            load_line_sys(file, i, line_len, pivot);
            write_line_sys(file, j, line_len, pivot);
            write_line_sys(file, i, line_len, line);
            load_line_sys(file, high, line_len, pivot);
        }
    }
    load_line_sys(file, i + 1, line_len, line);
    write_line_sys(file, i + 1, line_len, pivot);
    write_line_sys(file, high, line_len, line);
    return i + 1;
}

void qsort_sys(int64_t file, char* line, char* pivot, int64_t low,
               int64_t high, int64_t line_len) {
    if(low < high) {
        int64_t pi = partition_sys(file, line, pivot, low, high, line_len);

        qsort_sys(file, line, pivot, low, pi - 1, line_len);
        qsort_sys(file, line, pivot, pi + 1, high, line_len);
    }
}

int64_t copy_sys(char* path, char* dest, int64_t amount, int64_t line_len) {
    int64_t source = open(path, O_RDONLY);
    int64_t target =
        open(dest, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
    char* tmp = calloc(line_len, sizeof(char));
    for(int64_t i = 0; i < amount; i++) {
        if(read(source, tmp, (size_t)(line_len + 1) * sizeof(char)) !=
           (line_len + 1)) {
            return 1;
        }
        if(write(target, tmp, (size_t)(line_len + 1) * sizeof(char)) !=
           (line_len + 1)) {
            return 1;
        }
    }
    close(source);
    close(target);
    free(tmp);
    return 0;
};

int64_t sort_wrapper_sys(char* path, int64_t lines, int64_t line_len) {
    int64_t file = open(path, O_RDWR);
    char* line = calloc(line_len + 1, sizeof(char));
    char* pivot = calloc(line_len + 1, sizeof(char));
    qsort_sys(file, line, pivot, 0, lines - 1, line_len);
    close(file);
    free(line);
    free(pivot);
    return 0;
}

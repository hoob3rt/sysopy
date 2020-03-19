#include "generator.h"

#include <stdio.h>
#include <stdlib.h>

void generate(char* path, int64_t record_count, int64_t record_len) {
    FILE* file = fopen(path, "w+");
    FILE* urandom = fopen("/dev/urandom", "r");
    char* record = malloc(record_len * sizeof(char) + 1);
    if(file == (FILE*)NULL || urandom == (FILE*)NULL) {
        free(record);
        exit(1);
    }
    for(int64_t i = 0; i < record_count; ++i) {
        if(fread(record, sizeof(char), (size_t)record_len + 1, urandom) !=
           record_len + 1) {
            free(record);
            exit(1);
        }
        for(int64_t j = 0; j < record_len; ++j) {
            record[j] = (char)(abs(record[j]) % 25 + 'a');
        }
        record[record_len] = 10;
        if(fwrite(record, sizeof(char), (size_t)record_len + 1, file) !=
           record_len + 1) {
            free(record);
            exit(1);
        }
    }
    fclose(file);
    fclose(urandom);
    free(record);
}

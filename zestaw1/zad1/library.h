#ifndef LIBRARY_H
#define LIBRARY_H

#include <stdint.h>

typedef struct block {
    int64_t size;  // operations count
    char** operations;
} block;

struct table {
    int64_t size;  // blocks count
    block** blocks;
};

struct file_sequence {
    int size;  // sequence pairs count
    char*** sequence;  // sequence of file pairs
};

void free_sequence(struct file_sequence* sequence);
void free_block(struct block* block);
void free_table(struct table* table);
void remove_block(struct table* main_table, int64_t block_index);
void remove_operation_from_block(struct block* block_index,
                                 int64_t operation_index);
char* get_diff_from_file();
void create_table(struct table* main_table, int64_t size);
int64_t get_operations_count_from_block(struct block* selected_block);
struct file_sequence* define_sequence_of_files();

#endif

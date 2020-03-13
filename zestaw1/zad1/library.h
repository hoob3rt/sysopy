#ifndef LIBRARY_H
#define LIBRARY_H

#include "./structs.h"

#include <stdint.h>

void free_sequence(struct file_sequence* sequence);
void free_table(struct table* table);

void remove_block(struct table* main_table, int64_t block_index);
void remove_operation_from_block(struct block* block_index,
                                 int64_t operation_index);

void dump_diff_to_file(char* first_file, char* second_file);
int64_t create_block(struct table* table, char* block_data);
char* get_diff_from_file();
struct table* create_table(size_t size);
int64_t get_operations_count_from_block(struct block* selected_block);
struct file_sequence* create_sequence_from_array(char** arr, size_t size);

#endif

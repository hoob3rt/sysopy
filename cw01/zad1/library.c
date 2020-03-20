#include "library.h"

#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void free_block(struct block* blk) {
    free(blk);
}

struct table* create_table(size_t size) {
    struct table* main_table = calloc(4, sizeof(*main_table));
    main_table->blocks = calloc(size, sizeof(block*));
    main_table->size = size;
    int64_t i = 0;
    for(i = 0; i < size; i++) {
        main_table->blocks[i] = (struct block*)NULL;
    }
    printf("\ntable created with %ld size\n", main_table->size);
    return main_table;
}

void free_sequence(struct file_sequence* seq) {
    int64_t i = 0;
    for(i = 0; i < seq->size; i++) {
        free(seq->sequence[i][0]);
        free(seq->sequence[i][1]);
        free(seq->sequence[i]);
    }
    free(seq->sequence);
    free(seq);
}

void free_table(struct table* tab) {
    int64_t i = 0;
    for(i = 0; i < tab->size; i++) {
        if(tab->blocks[i] != NULL) {
            free_block(tab->blocks[i]);
        }
    }
    free(tab);
}

void remove_block(struct table* main_table, int64_t block_index) {
    if(block_index >= main_table->size) {
        printf("provided index too big\n");
        exit(1);
    }
    if(main_table->blocks[block_index] == NULL) {
        return;
    } else {
        int64_t i = 0, j = 0;
        for(i = block_index; i < main_table->size - 1; i++) {
            main_table->blocks[i]->size = main_table->blocks[i + 1]->size;
            main_table->blocks[i]->operations =
                realloc(main_table->blocks[i]->operations,
                        main_table->blocks[i]->size * sizeof(char*));
            for(j = 0; j < main_table->blocks[i]->size; j++) {
                strcpy(main_table->blocks[i]->operations[j],
                       main_table->blocks[i + 1]->operations[j]);
            }
        }
        free_block(main_table->blocks[main_table->size - 1]);
    }
    main_table->size--;
    printf("removed block with index %ld\n", block_index);
}

void remove_operation_from_block(struct block* blk,
                                 int64_t operation_index) {
    // TODO if operation_index> block_index->size
    int64_t i = 0;
    for(i = operation_index; i < blk->size - 1; i++) {
        blk->operations[i] =
            realloc(blk->operations[i],
                    strlen(blk->operations[i + 1]));
        strcpy(blk->operations[i], blk->operations[i + 1]);
    }
    free(blk->operations[blk->size - 1]);
    blk->size--;
}

int64_t get_operations_count_from_block(struct block* selected_block) {
    return selected_block->size;
}

size_t get_file_size(int64_t fd) {
    off_t currentPos = lseek(fd, (size_t)0, SEEK_CUR);
    size_t file_size = lseek(fd, 0, SEEK_END);
    lseek(fd, currentPos, SEEK_SET);
    return file_size;
}

char* get_diff_from_file() {
    int64_t fd = open("tmp", O_RDONLY);
    if(fd == -1) {
        printf("%s", strerror(errno));
    }
    size_t file_size = get_file_size(fd);
    char* file_content = calloc(file_size + 1, sizeof(char));
    read(fd, file_content, (size_t)file_size);
    close(fd);
    return file_content;
}

char** split_diff_to_blocks_data(char* diff_string, size_t* size) {
    size_t result_size = 0;
    size_t current_size = 0;
    char** result = NULL;
    char* current = NULL;

    while(diff_string) {
        const char* nextLine = strchr(diff_string, '\n');
        int64_t curLineLen = 0;
        if(nextLine) {
            curLineLen = (nextLine - diff_string);
        } else {
            curLineLen = strlen(diff_string);
        }
        char* tempStr = calloc(curLineLen + 2, sizeof(char));
        memcpy(tempStr, diff_string, curLineLen);
        if(curLineLen != 0) {
            tempStr[curLineLen] = '\n';
        }
        if(tempStr) {
            if(strncmp(diff_string, "<", 1) != 0 &&
               strncmp(diff_string, "-", 1) != 0 &&
               strncmp(diff_string, ">", 1) != 0 &&
               strncmp(diff_string, "\0", 1) != 0) {
                if(curLineLen == 0) {
                }
                if(result_size != 0) {
                    result[result_size - 1] = current;
                }
                result_size++;
                result = realloc(result, result_size * sizeof(char*));
                current = NULL;
                current_size = curLineLen + 2;
                current = calloc(current_size, sizeof(char));
                strcat(current, tempStr);
            } else {
                if(strlen(tempStr) > 0) {
                    current_size += curLineLen + 2;
                    current = realloc(current, current_size * sizeof(char));
                    strcat(current, tempStr);
                }
            }
        }
        if(nextLine) {
            diff_string = nextLine + 1;
        } else {
            diff_string = NULL;
        }
    }
    result = realloc(result, result_size * sizeof(char*));
    result[result_size - 1] = current;
    (*size) = result_size;
    return result;
}

int64_t create_block(struct table* table, char* block_data) {
    int64_t i = 0;
    int64_t index = -1;
    for(i = 0; i < table->size; i++) {
        if(table->blocks[i] == (struct block*)NULL) {
            index = i;
            break;
        }
    }
    if(index == -1) {
        index = table->size;
        table->size++;
        table->blocks =
            realloc(table->blocks, table->size * sizeof(struct block*));
    }
    struct block* new_block = calloc(4, sizeof(*new_block));
    new_block->size = 0;
    new_block->operations =
        split_diff_to_blocks_data(block_data, &new_block->size);
    table->blocks[index] = new_block;
    printf("created block with index %ld\n", index);
    return index;
}

void dump_diff_to_file(char* first_file, char* second_file) {
    char* buffer =
        calloc((strlen(first_file) + strlen(second_file) + 12), sizeof(char));
    sprintf(buffer, "diff %s %s > tmp", first_file, second_file);
    system(buffer);
}


struct file_sequence* create_sequence_from_array(char** arr, size_t size) {
    struct file_sequence* new_sequence = calloc(4, sizeof(*new_sequence));
    new_sequence->size = size / 2;
    new_sequence->sequence = calloc(4, new_sequence->size * sizeof(char**));
    int64_t i = 0;
    for(i = 0; i < size; i++) {
        if(i % 2 == 0) {
            new_sequence->sequence[i / 2] = calloc(4, 2 * sizeof(char*));
        }
        new_sequence->sequence[i / 2][i % 2] =
            calloc(4, strlen(arr[i]) * sizeof(char));
        strcpy(new_sequence->sequence[i / 2][i % 2], arr[i]);
        if(i % 2 == 1) {
            printf("created sequence of: %s and %s\n",
                   new_sequence->sequence[i / 2][0],
                   new_sequence->sequence[i / 2][1]);
        }
    }
    return new_sequence;
}

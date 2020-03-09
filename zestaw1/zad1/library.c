#include "library.h"

#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// TODO if size then size_t

void create_table(struct table* main_table, int64_t size) {
    main_table->blocks = calloc(size, sizeof(block*));
    main_table->size = size;
    int64_t i = 0;
    for(i = 0; i < size; i++) {
        main_table->blocks[i] = (struct block*)NULL;
    }
}

void free_sequence(struct file_sequence* seq) {
    int i = 0;
    for(i = 0; i < seq->size; i++) {
        free(seq->sequence[i][0]);
        free(seq->sequence[i][1]);
    }
    free(seq->sequence);
    free(seq);
}

void free_block(struct block* blk) {
    int i = 0;
    for(i = 0; i < blk->size; i++) {
        free(blk->operations[i]);
    }
    free(blk->operations);
    free(blk);
}

void free_table(struct table* tab) {
    int i = 0;
    for(i = 0; i < tab->size; i++) {
        free_block(tab->blocks[i]);
    }
    free(tab);
}

void remove_block(struct table* main_table, int64_t block_index) {
    if(block_index >= main_table->size) {
        printf("provided index too big\n");
        exit(1);
    }
    if(main_table->blocks[block_index] == NULL) {
        // prompt is null, only shrinking size
    } else {
        int i = 0, j = 0;
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
    printf("removed block with index %d\n", block_index);
}

void remove_operation_from_block(struct block* block_index,
                                 int64_t operation_index) {
    // TODO if operation_index> block_index->size
    int i = 0;
    for(i = operation_index; i < block_index->size - 1; i++) {
        block_index->operations[i] =
            realloc(block_index->operations[i],
                    strlen(block_index->operations[i + 1]));
        strcpy(block_index->operations[i], block_index->operations[i + 1]);
    }
    free(block_index->operations[block_index->size - 1]);
    block_index->size--;
}

int64_t get_operations_count_from_block(struct block* selected_block) {
    return selected_block->size;
}

size_t get_file_size(int fd) {
    off_t currentPos = lseek(fd, (size_t)0, SEEK_CUR);
    size_t file_size = lseek(fd, 0, SEEK_END);
    lseek(fd, currentPos, SEEK_SET);
    return file_size;
}

char* get_diff_from_file() {
    int fd = open("tmp", O_RDONLY);
    if(fd == -1) {
        printf("%s", strerror(errno));
    }
    size_t file_size = get_file_size(fd);
    char* file_content = calloc(file_size, sizeof(char));
    read(fd, file_content, (size_t)file_size);
    close(fd);
    system("rm tmp");
    return file_content;
}

char** split_diff_to_blocks_data(char* diff_string, int* size) {
    size_t result_size = 0;
    size_t current_size = 0;
    char** result = NULL;
    char* current = NULL;

    while(diff_string) {
        const char* nextLine = strchr(diff_string, '\n');
        int curLineLen = 0;
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

int create_block(struct table* table, char* block_data) {
    int i = 0;
    int index = -1;
    for(i = 0; i < table->size; i++) {
        if(table->blocks[i] == NULL) {
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
    printf("created block with index %d\n", index);
    return index;
}

void dump_diff_to_file(char* first_file, char* second_file) {
    char* buffer =
        calloc(buffer,
               (strlen(first_file) + strlen(second_file) + 12) * sizeof(char));
    sprintf(buffer, "diff %s %s > tmp", first_file, second_file);
    system(buffer);
}

struct file_sequence* define_sequence_of_files() {
    struct file_sequence* new_sequence = calloc(4, sizeof(*new_sequence));
    int file_count = 0;
    int i = 0;
    printf("enter file count: ");
    scanf("%i", &file_count);
    if(file_count % 2 != 0) {
        fprintf(stderr, "odd number entered\n");
        exit(1);
    }
    new_sequence->size = file_count / 2;
    new_sequence->sequence = calloc(4, new_sequence->size * sizeof(char**));
    for(i = 0; i < file_count; i++) {
        if(i % 2 == 0) {
            new_sequence->sequence[i / 2] = calloc(4, 2 * sizeof(char*));
        }
        scanf("%ms", &new_sequence->sequence[i / 2][i % 2]);
        if(i % 2 == 1) {
            printf("created sequence of: %s and %s\n",
                   new_sequence->sequence[i / 2][0],
                   new_sequence->sequence[i / 2][1]);
        }
    }
    return new_sequence;
}

struct table* create_table_from_sequence(struct file_sequence* seq) {
    struct table* new_table = calloc(4, sizeof(*new_table));
    create_table(new_table, seq->size);
    int i = 0;
    for(i = 0; i < seq->size; i++) {
        dump_diff_to_file(seq->sequence[i][0], seq->sequence[i][1]);
        char* diff = get_diff_from_file();
        create_block(new_table, diff);
    }
    return new_table;
}

#ifndef DYNAMIC_LOADER_H
#define DYNAMIC_LOADER_H
#include <dlfcn.h>
#include <lzma.h>
#include <stdio.h>
#include <stdlib.h>

static void* handle = NULL;

typedef struct table table;
typedef struct block block;
typedef struct file_sequence file_sequence;
void (*free_sequence)(struct file_sequence*);
void (*free_table)(struct table*);
void (*remove_block)(struct table*, int64_t);
void (*remove_operation_from_block)(struct block*, int64_t);
void (*dump_diff_to_file)(char*, char*);
char* (*get_diff_from_file)();
int (*create_block)(struct table*, char*);
struct table* (*create_table)(size_t);
int64_t (*get_operations_count_from_block)(struct block*);
struct file_sequence* (*create_sequence_from_array)(char**, size_t);

void init_dynamic_handler() {
    handle = dlopen("./libsysopy.so", RTLD_LAZY);
    if(handle == NULL) {
        printf("ERROR WHILE LOADING DYNAMIC LIBRARY\n");
        exit(1);
    }
    printf("SUCCESSFUL LOADING OF DYNAMIC LIBRARY\n");

    *(void**)(&free_sequence) = dlsym(handle, "free_sequence");
    *(void**)(&free_table) = dlsym(handle, "free_table");
    *(void**)(&remove_block) = dlsym(handle, "remove_block");
    *(void**)(&remove_operation_from_block) =
        dlsym(handle, "remove_operation_from_block");

    *(void**)(&dump_diff_to_file) = dlsym(handle, "dump_diff_to_file");
    *(void**)(&get_diff_from_file) = dlsym(handle, "get_diff_from_file");

    *(void**)(&create_block) = dlsym(handle, "create_block");
    *(void**)(&create_table) = dlsym(handle, "create_table");
    *(void**)(&get_operations_count_from_block) =
        dlsym(handle, "get_operations_count_from_block");
    *(void**)(&create_sequence_from_array) =
        dlsym(handle, "create_sequence_from_array");

    if(dlerror() != NULL) {
        printf("COULD NOT LOAD CREATE TABLE METHOD\n");
    }
}
void free_handler_memory() {
    dlclose(handle);
}
#endif

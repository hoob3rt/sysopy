#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdio.h>

typedef struct block {
    size_t size;  // operations count
    char** operations;
} block;

typedef struct table {
    size_t size;  // blocks count
    block** blocks;
}table;

typedef struct file_sequence {
    size_t size;  // sequence pairs count
    char*** sequence;  // sequence of file pairs
}file_sequence;

#endif

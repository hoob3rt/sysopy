#ifndef LIB_FUNCTIONS_H
#define LIB_FUNCTIONS_H value

#include <stdint.h>

void copy_lib(char* src_path, char* dest_path, int64_t record_count,
              int64_t record_len);
int64_t sort_wrapper_lib(char* path, int64_t lines, int64_t line_len);

#endif /* ifndef LIB_FUNCTIONS_H */

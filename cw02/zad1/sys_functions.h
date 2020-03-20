#ifndef SYS_FUNCTIONS_H
#define SYS_FUNCTIONS_H value

#include <stdint.h>

int64_t copy_sys(char* path, char* dest, int64_t amount, int64_t len);
int64_t sort_wrapper_sys(char* path, int64_t high, int64_t len);

#endif /* ifndef SYS_FUNCTIONS_H */

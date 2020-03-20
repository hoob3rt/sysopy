#ifndef TEST_SUITE_H
#define TEST_SUITE_H

#include "../zad1/library.h"

#include <inttypes.h>
#include <stdio.h>

void start_timer();
void stop_timer();
char** parse_file_names(int64_t argc, char* argv[], int64_t* current_index,
                        size_t* size);
void handle_stdin(int64_t argc, char* argv[], int64_t* current_index,
                  struct table** tab, struct file_sequence** fs);

#endif

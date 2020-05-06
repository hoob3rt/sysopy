#ifndef UTILS_H
#define UTILS_H value

#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/times.h>
#include <unistd.h>

typedef enum mode { SIGN, BLOCK, INTERLEAVED } mode;

void free_memory(int** image, int image_height, int** histograms,
                 int threads_count);

int get_runtime(struct timeval start_time);

void dump_runtime_to_file(char* file_name, char* method, pthread_t* threads,
                          struct timeval start_time, int threads_count);

int** load_image(char* file_name, int max_line_len, int* image_width,
                 int* image_height) ;

int dump_result_to_file(char* file_name, int** histograms, int image_width,
                        int image_height, int pixel_count, int threads_count);

#endif  // UTILS_H

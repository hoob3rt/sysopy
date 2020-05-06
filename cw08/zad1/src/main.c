#include "utils.h"

#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/times.h>
#include <unistd.h>

const int MAX_LINE_LEN = 2048;
const int PIXEL_COUNT = 256;

struct arg_struct {
    int** image;
    int** histograms;
    int index;
    int image_width;
    int image_height;
    int threads_count;
};

void* sign_thread(void* arguments) {
    struct arg_struct* args = arguments;
    struct timeval start_time;
    gettimeofday(&start_time, NULL);
    int min_value =
        args->index * ceil((double)PIXEL_COUNT / args->threads_count);
    int max_value =
        (args->index + 1) * ceil((double)PIXEL_COUNT / args->threads_count) -
        1;
    if(max_value > PIXEL_COUNT - 1) {
        max_value = PIXEL_COUNT - 1;
    }
    for(int row = 0; row < args->image_height; row++) {
        for(int col = 0; col < args->image_width; col++) {
            if(args->image[row][col] >= min_value &&
               args->image[row][col] <= max_value) {
                args->histograms[args->index][args->image[row][col]]++;
            }
        }
    }
    int* result_time = (int*)malloc(sizeof(int));
    *result_time = get_runtime(start_time);
    return result_time;
}

void* block_thread(void* arguments) {
    struct arg_struct* args = arguments;
    struct timeval start_time;
    gettimeofday(&start_time, NULL);
    int start_col =
        args->index * ceil((double)args->image_width / args->threads_count);
    int end_col = (args->index + 1) *
                  ceil((double)args->image_width / args->threads_count);
    if(end_col > args->image_width) {
        end_col = args->image_width;
    }
    for(int row = 0; row < args->image_height; row++) {
        for(int col = start_col; col < end_col; col++) {
            args->histograms[args->index][args->image[row][col]]++;
        }
    }
    int* result_time = (int*)malloc(sizeof(int));
    *result_time = get_runtime(start_time);
    return result_time;
}

void* interleaved_thread(void* arguments) {
    struct arg_struct* args = arguments;
    struct timeval start_time;
    gettimeofday(&start_time, NULL);
    int start_col = args->index;
    int step = args->threads_count;
    for(int row = 0; row < args->image_height; row++) {
        for(int col = start_col; col < args->image_width; col += step) {
            args->histograms[args->index][args->image[row][col]]++;
        }
    }
    int* result_time = (int*)malloc(sizeof(int));
    *result_time = get_runtime(start_time);
    return result_time;
}

int main(int argc, char** argv) {
    if(argc != 5) {
        exit(1);
    }
    mode split_mode;
    if(!strcmp("sign", argv[2])) {
        split_mode = SIGN;
    } else if(!strcmp("block", argv[2])) {
        split_mode = BLOCK;
    } else if(!strcmp("interleaved", argv[2])) {
        split_mode = INTERLEAVED;
    } else {
        exit(1);
    }
    char* input_file = argv[3];
    char* output_file = argv[4];

    struct arg_struct args;
    args.threads_count = atoi(argv[1]);
    args.image = load_image(input_file, MAX_LINE_LEN, &args.image_width,
                            &args.image_height);
    args.histograms = (int**)calloc(args.threads_count, sizeof(int*));
    for(int i = 0; i < args.threads_count; i++) {
        args.histograms[i] = (int*)calloc(PIXEL_COUNT, sizeof(int));
    }
    struct timeval start_time;
    gettimeofday(&start_time, NULL);
    pthread_t* threads =
        (pthread_t*)calloc(args.threads_count, sizeof(pthread_t));
    for(int i = 0; i < args.threads_count; i++) {
        args.index = i;
        if(split_mode == SIGN) {
            pthread_create(&threads[i], NULL, &sign_thread, &args);
        } else if(split_mode == BLOCK) {
            pthread_create(&threads[i], NULL, &block_thread, &args);
        } else {
            pthread_create(&threads[i], NULL, &interleaved_thread, &args);
        }
    }
    dump_runtime_to_file("Times.txt", argv[2], threads, start_time,
                         args.threads_count);
    dump_result_to_file(output_file, args.histograms, args.image_width,
                        args.image_height, PIXEL_COUNT, args.threads_count);
    free_memory(args.image, args.image_height, args.histograms,
                args.threads_count);

    return 0;
}

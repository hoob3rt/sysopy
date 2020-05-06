#include "utils.h"

void free_memory(int** image, int image_height, int** histograms,
                 int threads_count) {
    for(int i = 0; i < image_height; i++) {
        free(image[i]);
    }
    free(image);
    for(int i = 0; i < threads_count; i++) {
        free(histograms[i]);
    }
    free(histograms);
}

int get_runtime(struct timeval start_time) {
    struct timeval end_time;
    gettimeofday(&end_time, NULL);
    return ((end_time.tv_sec - start_time.tv_sec) * 1e6) +
           (end_time.tv_usec - start_time.tv_usec);
}

void log_runtime(long int thread, int microseconds, FILE* times, int i) {
    int seconds = microseconds / 1e6;
    microseconds -= seconds * 1e6;
    int miliseconds = microseconds / 1e3;
    microseconds -= miliseconds * 1e3;
    if(thread == -1) {
        printf("Total time: %d seconds %d miliseconds %d microseconds\n\n",
               seconds, miliseconds, microseconds);
        fprintf(times,
                "Total time: %d seconds %d miliseconds %d microseconds\n",
                seconds, miliseconds, microseconds);
    } else {
        printf(
            "Thread: %ld, time: %d seconds %d miliseconds %d microseconds\n",
            thread, seconds, miliseconds, microseconds);
        fprintf(
            times,
            "Thread: %d, time: %d seconds %d miliseconds %d microseconds\n", i,
            seconds, miliseconds, microseconds);
    }
}

void dump_runtime_to_file(char* file_name, char* mode, pthread_t* threads,
                          struct timeval start_time, int threads_count) {
    FILE* times = fopen(file_name, "a");
    if(times == NULL) {
        exit(1);
    }
    fprintf(times, "\n%s - %d threads\n", mode,
            threads_count);
    for(int i = 0; i < threads_count; i++) {
        int* time;
        pthread_join(threads[i], (void**)&time);
        log_runtime(threads[i], *time, times, i + 1);
    }
    log_runtime(-1, get_runtime(start_time), times, -1);
    fclose(times);
}

int** load_image(char* file_name, int max_line_len, int* image_width,
                 int* image_height) {
    FILE* file = fopen(file_name, "r");
    if(file == NULL) {
        exit(1);
    }
    int** IMAGE;
    char buffer[max_line_len];
    int line_number = 0;
    int image_max_value;
    while(line_number < 3 && fgets(buffer, max_line_len, file) != NULL) {
        if(buffer[0] == '#') {
            continue;
        }
        if(line_number == 0 && strncmp("P2", buffer, 2)) {
            fclose(file);
            exit(1);
        }
        else if(line_number == 1) {
            if(sscanf(buffer, "%d %d\n", &(*image_width), &(*image_height)) !=
               2) {
                fclose(file);
                exit(1);
            }
            IMAGE = (int**)calloc((*image_height), sizeof(int*));
            for(int i = 0; i < (*image_height); i++) {
                IMAGE[i] = (int*)calloc((*image_width), sizeof(int));
            }
        }
        else if(line_number == 2) {
            if(sscanf(buffer, "%d\n", &image_max_value) != 1) {
                fclose(file);
                exit(1);
            }
        }
        line_number++;
    }
    if(line_number != 3) {
        fclose(file);
        exit(1);
    }
    int val;
    for(int row = 0; row < (*image_height); row++) {
        for(int col = 0; col < *(image_width); col++) {
            fscanf(file, "%d", &val);
            IMAGE[row][col] = val;
        }
    }
    fclose(file);
    return IMAGE;
}

int dump_result_to_file(char* file_name, int** histograms, int image_width,
                        int image_height, int pixel_count, int threads_count) {
    FILE* file = fopen(file_name, "w+");
    if(file == NULL) {
        return 1;
    }
    fprintf(file, "P2\n");
    for(int i = 0; i < pixel_count; i++) {
        int value = 0;
        for(int j = 0; j < threads_count; j++) {
            value += histograms[j][i];
        }
        fprintf(file, "%d %d\n", i, value);
    }
    fclose(file);
    return 0;
}

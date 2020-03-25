#define _XOPEN_SOURCE 500

#include "matrix_func.h"
#include "task.h"

#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int PAIRS_COUNT = 0;
int AX_COLUMNS_NUMBER = 10000;
int MAX_LINE_LENGTH = 50000;

void multiply_column(char* first_filename, char* second_filename,
                     int column_index, int pair_index) {
    matrix a = load_matrix(first_filename);
    matrix b = load_matrix(second_filename);
    char* filename = calloc(20, sizeof(char));
    sprintf(filename, ".tmp/part%d%04d", pair_index, column_index);
    FILE* part_file = fopen(filename, "w+");
    for(int y = 0; y < a.rows; y++) {
        int result = 0;
        for(int x = 0; x < a.columns; x++) {
            result += a.values[y][x] * b.values[x][column_index];
        }
        if(y == a.rows - 1)
            fprintf(part_file, "%d ", result);
        else
            fprintf(part_file, "%d \n", result);
    }
    fclose(part_file);
}

void multiply_column_to_one_file(char* first_filename, char* second_filename,
                                 int column_index, char* result_file) {
    matrix a = load_matrix(first_filename);
    matrix b = load_matrix(second_filename);
    FILE* file = fopen(result_file, "r+");
    int fd = fileno(file);
    flock(fd, LOCK_EX);
    matrix res = load_matrix(result_file);
    for(int y = 0; y < a.rows; y++) {
        int result = 0;
        for(int x = 0; x < a.columns; x++) {
            result += a.values[y][x] * b.values[x][column_index];
        }
        res.values[y][column_index] = result;
    }
    dump_matrix_to_file(file, res);
    flock(fd, LOCK_UN);
    fclose(file);
}

int worker_function(char** a, char** b, int timeout, int mode,
                    char** result_file) {
    time_t start_time = time(NULL);
    int multiplications_count = 0;
    while(1) {
        if((time(NULL) - start_time) >= timeout) {
            puts("timeout");
            break;
        }
        struct task new_task = get_task(PAIRS_COUNT);
        if(new_task.column_index == -1) {
            break;
        }
        if(mode == 1) {
            multiply_column_to_one_file(
                a[new_task.pair_index], b[new_task.pair_index],
                new_task.column_index, result_file[new_task.pair_index]);
        } else {
            multiply_column(a[new_task.pair_index], b[new_task.pair_index],
                            new_task.column_index, new_task.pair_index);
        }
        multiplications_count++;
    }
    return multiplications_count;
}

int main(int argc, char* argv[]) {
    if(argc != 5) {
        return 1;
    }

    int processes_number = atoi(argv[2]);
    int timeout = atoi(argv[3]);
    int mode = atoi(argv[4]);

    char** first_filenames = calloc(100, sizeof(char*));
    char** second_filenames = calloc(100, sizeof(char*));
    char** result_filenames = calloc(100, sizeof(char*));

    system("rm -rf .tmp");
    system("mkdir -p .tmp");

    FILE* input_file = fopen(argv[1], "r");
    char input_line[PATH_MAX * 3 + 3];
    int pair_counter = 0;
    while(fgets(input_line, PATH_MAX * 3 + 3, input_file) != NULL) {

        first_filenames[pair_counter] = calloc(PATH_MAX, sizeof(char));
        second_filenames[pair_counter] = calloc(PATH_MAX, sizeof(char));
        result_filenames[pair_counter] = calloc(PATH_MAX, sizeof(char));

        strcpy(first_filenames[pair_counter], strtok(input_line, " "));
        strcpy(second_filenames[pair_counter], strtok(NULL, " "));
        strcpy(result_filenames[pair_counter], strtok(NULL, " "));

        matrix a = load_matrix(first_filenames[pair_counter]);
        matrix b = load_matrix(second_filenames[pair_counter]);
        if(mode == 1) {
            generate_matrix(a.rows, b.columns, result_filenames[pair_counter]);
        }
        char* task_filename = calloc(100, sizeof(char));
        sprintf(task_filename, ".tmp/tasks%d", pair_counter);
        FILE* tasks_file = fopen(task_filename, "w+");
        char* tasks = calloc(b.columns + 1, sizeof(char));
        sprintf(tasks, "%0*d", b.columns, 0);
        fwrite(tasks, 1, b.columns, tasks_file);
        free(tasks);
        free(task_filename);
        fclose(tasks_file);
        pair_counter++;
    }
    PAIRS_COUNT = pair_counter;

    pid_t* processes = calloc(processes_number, sizeof(int));
    for(int i = 0; i < processes_number; i++) {
        pid_t worker = fork();
        if(worker == 0) {
            return worker_function(first_filenames, second_filenames, timeout,
                                   mode, result_filenames);
        } else {
            processes[i] = worker;
        }
    }
    for(int i = 0; i < processes_number; i++) {
        int status;
        waitpid(processes[i], &status, 0);
        printf("Proces %d wykonal %d mnozen macierzy\n", processes[i],
               WEXITSTATUS(status));
    }
    free(processes);
    if(mode == 2) {
        for(int i = 0; i < pair_counter; i++) {
            char* buffer = calloc(1000, sizeof(char));
            sprintf(buffer, "paste .tmp/part%d* > %s", i, result_filenames[i]);
            system(buffer);
        }
    }
    return 0;
}

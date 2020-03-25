#define _XOPEN_SOURCE 500

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

struct task get_task(int pair_count) {
    struct task new_task;
    new_task.column_index = -1;
    new_task.pair_index = -1;
    for(int i = 0; i < pair_count; i++) {
        char* task_filename = calloc(100, sizeof(char));
        sprintf(task_filename, ".tmp/tasks%d", i);
        FILE* tasks_file = fopen(task_filename, "r+");
        int fd = fileno(tasks_file);
        flock(fd, LOCK_EX);

        char* tasks = calloc(1000, sizeof(char));
        fseek(tasks_file, 0, SEEK_SET);
        fread(tasks, 1, 1000, tasks_file);

        char* task_first_zero = strchr(tasks, '0');
        int task_index =
            task_first_zero != NULL ? task_first_zero - tasks : -1;

        if(task_index >= 0) {
            char* end_of_line = strchr(tasks, '\0');
            int size = end_of_line - tasks;

            char* tasks_with_good_size = calloc(size + 1, sizeof(char));
            for(int j = 0; j < size; j++) {
                tasks_with_good_size[j] = tasks[j];
            }
            tasks_with_good_size[task_index] = '1';
            fseek(tasks_file, 0, SEEK_SET);
            fwrite(tasks_with_good_size, 1, size, tasks_file);
            new_task.pair_index = i;
            new_task.column_index = task_index;
            flock(fd, LOCK_UN);
            fclose(tasks_file);
            break;
        }
        flock(fd, LOCK_UN);
        fclose(tasks_file);
    }

    return new_task;
}

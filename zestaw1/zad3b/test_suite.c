#include "test_suite.h"

#include <stdlib.h>
#include <string.h>
#include <sys/times.h>
/*#include <time.h>*/
#include <zconf.h>

clock_t START_TIME;
clock_t END_TIME;
struct tms START_CPU;
struct tms END_CPU;
double TOTAL_REAL = 0.0;
double TOTAL_USER = 0.0;
double TOTAL_SYSTEM = 0.0;

void start_timer() {
    START_TIME = times(&START_CPU);
}

void stop_timer() {
    END_TIME = times(&END_CPU);
    int64_t clk_tck = sysconf(_SC_CLK_TCK);

    char* statement = calloc(strlen("Real time: 000.0000,\nUser time: "
                                    "000.0000,\nSystem time: 000.0000\n") +
                                 1,
                             sizeof(char));

    double real_time = (double)(END_TIME - START_TIME) / clk_tck;
    double user_time =
        (double)(END_CPU.tms_utime - START_CPU.tms_utime) / clk_tck +
        (double)(END_CPU.tms_cutime - START_CPU.tms_cutime) / clk_tck;
    double system_time =
        (double)(END_CPU.tms_stime - START_CPU.tms_stime) / clk_tck +
        (double)(END_CPU.tms_cstime - START_CPU.tms_cstime) / clk_tck;
    TOTAL_REAL += real_time;
    TOTAL_USER += user_time;
    TOTAL_SYSTEM += system_time;

    sprintf(statement,
            "Real time: %.4fs,\nUser time: %.4fs,\nSystem time: %.4fs\n",
            real_time, user_time, system_time);
    printf("\n%s\n", statement);
}

char** parse_file_names(int64_t argc, char* argv[], int64_t* current_index,
                        size_t* size) {
    char** files = (char**)NULL;
    while(strcmp(argv[*current_index], "create_table") != 0 &&
          strcmp(argv[*current_index], "compare_pairs") != 0 &&
          strcmp(argv[*current_index], "remove_block") != 0 &&
          strcmp(argv[*current_index], "remove_operation") != 0) {
        char* token = strtok(argv[*current_index], ":");
        while(token != (char*)NULL) {
            (*size)++;
            files = realloc(files, (*size) * sizeof(char*));
            files[(*size) - 1] = calloc(strlen(token), sizeof(char));
            strcpy(files[(*size) - 1], token);
            token = strtok((char*)NULL, ":");
        }
        (*current_index)++;
        if(*current_index == argc) {
            break;
        }
    }
    if(files != (char**)NULL) {
        return files;
    }
    exit(1);
}

void handle_stdin(int64_t argc, char* argv[], int64_t* current_index,
                  struct table** tab, struct file_sequence** fs) {
    if(strcmp(argv[*current_index], "create_table") == 0) {
        start_timer();
        (*current_index)++;
        int64_t tab_size = atoi(argv[*current_index]);
        *tab = create_table(tab_size);
        (*current_index)++;
        stop_timer();
    } else if(strcmp(argv[*current_index], "compare_pairs") == 0) {
        start_timer();
        (*current_index)++;
        size_t size = 0;
        char** files = parse_file_names(argc, argv, current_index, &size);
        *fs = create_sequence_from_array(files, size);
        int64_t i = 0;

        for(i = 0; i < (*fs)->size; i++) {
            dump_diff_to_file((*fs)->sequence[i][0], (*fs)->sequence[i][1]);
            char* diff = get_diff_from_file();
            create_block((*tab), diff);
        }
        stop_timer();
    } else if(strcmp(argv[*current_index], "remove_block") == 0) {
        start_timer();
        (*current_index)++;
        remove_block(*tab, atoi(argv[*current_index]));
        (*current_index)++;
        stop_timer();
    } else if(strcmp(argv[*current_index], "remove_operation") == 0) {
        start_timer();
        (*current_index)++;
        remove_operation_from_block((*tab)->blocks[atoi(argv[*current_index])],
                                    (*current_index)+1);
        (*current_index)++;
        stop_timer();
    }
}

int main(int argc, char* argv[]) {
#ifdef DYNAMIC
    init_dynamic_handler();
#endif

    /*init*/
    struct table* tab = (struct table*)NULL;
    struct file_sequence* fs = (struct file_sequence*)NULL;
    if(argc <= 2) {
        printf("enter more cl arguments\n");
        exit(1);
    }
    int64_t current_index = 1;
    while(current_index < argc) {
        handle_stdin(argc, argv, &current_index, &tab, &fs);
    }
    if(tab != NULL) {
        free_table(tab);
    }
    if(fs != NULL) {
        free_sequence(fs);
    }
    char* statement =
        calloc(strlen("Total real time: 000.0000,\nTotal user time: "
                      "000.0000,\nTotal system time: 000.0000\n") +
                   1,
               sizeof(char));
    sprintf(statement,
            "Total real time: %.4fs,\nTotal user time: %.4fs,\nTotal system "
            "time: %.4fs\n",
            TOTAL_REAL, TOTAL_USER, TOTAL_SYSTEM);
    printf("\n%s\n", statement);
    return 0;
}

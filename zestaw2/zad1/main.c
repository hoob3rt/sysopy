#include "generator.h"
#include "lib_functions.h"
#include "sys_functions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/times.h>
#include <zconf.h>

clock_t START_TIME;
clock_t END_TIME;
struct tms START_CPU;
struct tms END_CPU;

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
    sprintf(statement,
            "Real time: %.4fs,\nUser time: %.4fs,\nSystem time: %.4fs\n",
            real_time, user_time, system_time);
    printf("\n%s\n", statement);
    free(statement);
}

int main(int argc, char* argv[]) {
    if(argc < 5) {
        exit(1);
    }
    start_timer();
    if(strcmp(argv[1], "generate") == 0) {
        generate(argv[2], atoi(argv[3]), atoi(argv[4]));
    } else if(strcmp(argv[1], "sort") == 0) {
        if(argc < 6) {
            exit(1);
        }
        if(strcmp(argv[5], "sys") == 0) {
            sort_wrapper_sys(argv[2], atoi(argv[3]), atoi(argv[4]));
        } else if(strcmp(argv[5], "lib") == 0) {
            sort_wrapper_lib(argv[2], atoi(argv[3]), atoi(argv[4]));
        }
    } else if(strcmp(argv[1], "copy") == 0) {
        if(argc < 7) {
            exit(1);
        }
        if(strcmp(argv[6], "sys") == 0) {
            copy_sys(argv[2], argv[3], atoi(argv[4]), atoi(argv[5]));
        } else if(strcmp(argv[6], "lib") == 0) {
            copy_lib(argv[2], argv[3], atoi(argv[4]), atoi(argv[5]));
        }
    }
    stop_timer();
    return 0;
}

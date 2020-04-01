#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 700
#endif

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

void segv_action(int sig, siginfo_t* info, void* ucontext) {
    printf("si_addr %p\n", info->si_addr);
    exit(0);
}

void int_action(int sig, siginfo_t* info, void* ucontext) {
    printf("si_uid %u\n", info->si_uid);
    exit(0);
}

void fpe_action(int sig, siginfo_t* info, void* ucontext) {
    printf("si_code %u\n", info->si_code);
    exit(0);
}

int main() {
    if(fork() == 0) {
        struct sigaction act = {.sa_flags = SA_SIGINFO,
                                .sa_sigaction = fpe_action};
        sigaction(SIGFPE, &act, NULL);
        int division_by_zero = 1 / 0;
    }
    wait(NULL);
    if(fork() == 0) {
        struct sigaction act = {.sa_flags = SA_SIGINFO,
                                .sa_sigaction = segv_action};
        sigaction(SIGSEGV, &act, NULL);
        int* memory_validation = (int*)12345;  // segfault inc
        *memory_validation = 213;
    }
    wait(NULL);
    if(fork() == 0) {
        struct sigaction act = {.sa_flags = SA_SIGINFO,
                                .sa_sigaction = int_action};
        sigaction(SIGINT, &act, NULL);
        raise(SIGINT);
    }
    wait(NULL);
}

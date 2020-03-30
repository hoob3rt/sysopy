#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 700
#endif

#include <dirent.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int running = 1;

void process_signal(int signal) {
    if(running == 1) {
        printf("\nCaptured signal %d\npress CTRL+Z to continue\n"
               "press CTRL+C to exit\n",
               signal);
        running = 0;
    } else {
        running = 1;
    }
}

void init_sig(int signal) {
    printf("\ncaptured SIGINT %d\n", signal);
    exit(EXIT_SUCCESS);
}

int main(int argc, char const* argv[]) {
    int iteration = 0;
    struct sigaction signals;
    signals.sa_flags = 0;
    signals.sa_handler = process_signal;
    sigemptyset(&signals.sa_mask);
    while(1) {
        sigaction(SIGTSTP, &signals, NULL);
        signal(SIGINT, init_sig);
        if(running) {
            printf("iteration: %d\n", iteration);
            system("ls -la");
            printf("\n");
            iteration++;
        }
        sleep(1);
    }
    return 0;
}

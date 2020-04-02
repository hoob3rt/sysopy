#define _POSIX_C_SOURCE 1
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

void signalHandle(int sig) {
    printf("Signal received\n");
}

int main(int argc, char** argv) {
    if(!strcmp("ignore", argv[1])) {
        puts("IGNORE");
        signal(SIGUSR1, SIG_IGN);
    } else if(!strcmp("handler", argv[1])) {
        puts("HANDLER");
        signal(SIGUSR1, signalHandle);
    } else {
        if(!strcmp("mask", argv[1])) {
            puts("MASK");
        } else {
            puts("PENDING");
        }
        sigset_t mask;
        sigemptyset(&mask);
        sigaddset(&mask, SIGUSR1);
        if(sigprocmask(SIG_BLOCK, &mask, NULL) < 0) {
            puts("signals cant be blocked");
            exit(-1);
        }
    }
    sigset_t mask;
    raise(SIGUSR1);
    if(strcmp("mask", argv[1]) || strcmp("pending", argv[1])) {
        sigpending(&mask);
        if(sigismember(&mask, SIGUSR1)) {
            puts("signal pending in parent.");
        }
    }
    if(!strcmp(argv[2], "fork")) {
        pid_t childPid = fork();
        if(childPid == 0) {
            if(strcmp("pending", argv[1])) {
                raise(SIGUSR1);
            }
            if(strcmp("mask", argv[1]) || strcmp("pending", argv[1])) {
                sigpending(&mask);
                if(sigismember(&mask, SIGUSR1)) {
                    puts("signal pending in child.");
                } else {
                    puts("signal not pending in child.");
                }
            }
        }
    } else if(!strcmp(argv[2], "exec") && strcmp("handler", argv[1]) != 0) {
        execl("./exec", "./exec", argv[1], NULL);
    }
    wait(0);
    return 0;
}

#define _POSIX_C_SOURCE 1
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv){
    if (strcmp("pending", argv[1]) != 0){
        raise(SIGUSR1);
    }
    if (!strcmp("pending", argv[1]) || !strcmp("mask", argv[1])){
        sigset_t mask;
        sigpending(&mask);
        if(sigismember(&mask, SIGUSR1)){
            printf("signal pending in child.\n\n");
        }
        else{
            printf("signal not pending in child.\n\n");
        }
    }
    return 0;
}

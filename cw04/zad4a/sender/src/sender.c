#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

pid_t PID;
bool WAIT = true;
int RECEIVED_SIGNALS = 0;

void on_sigusr1(int _) {
    RECEIVED_SIGNALS++;
}
void on_sigusr2(int _) {
    WAIT = false;
}

int main(int argc, char** argv) {
    PID = atoi(argv[1]);
    int total = atoi(argv[2]);
    bool using_sigqueue = strcmp(argv[3], "sigqueue") == 0;
    bool using_kill = strcmp(argv[3], "kill") == 0;
    bool using_sigrt = strcmp(argv[3], "sigrt") == 0;
    union sigval sig = {.sival_ptr = NULL};
    int sig1 = using_sigrt ? SIGRTMIN : SIGUSR1;
    int sig2 = using_sigrt ? sig1 + 1 : SIGUSR2;
    signal(sig1, on_sigusr1);
    signal(sig2, on_sigusr2);
    if(using_kill || using_sigrt) {
        for(int i = 0; i < total; i++) {
            kill(PID, sig1);
        }
        kill(PID, sig2);
    } else if(using_sigqueue) {
        for(int i = 0; i < total; i++) {
            sigqueue(PID, sig1, sig);
        }
        sigqueue(PID, sig2, sig);
    }
    WAIT = true;
    while(WAIT)
        ;
    printf("sender got: %d signals\n", RECEIVED_SIGNALS);
}

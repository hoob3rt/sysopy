#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

pid_t PID;
bool WAIT = true;
int RECEIVED_SIGNALS = 0;

void on_sigusr1(int _) {
    RECEIVED_SIGNALS++;
}

void on_sigusr2(int sig, siginfo_t* info, void* uncontext) {
    printf("catcher got: %d signals\n", RECEIVED_SIGNALS);
    PID = info->si_pid;
    WAIT = false;
}

int main(int argc, char** argv) {
    bool using_sigqueue = strcmp(argv[1], "sigqueue") == 0;
    bool using_kill = strcmp(argv[1], "kill") == 0;
    bool using_sigrt = strcmp(argv[1], "sigrt") == 0;
    union sigval sig = {.sival_ptr = NULL};
    int sig1 = using_sigrt ? SIGRTMIN : SIGUSR1;
    int sig2 = using_sigrt ? sig1 + 1 : SIGUSR2;
    signal(sig1, on_sigusr1);
    struct sigaction act;
    act.sa_flags = SA_SIGINFO;
    act.sa_sigaction = on_sigusr2;
    sigaction(sig2, &act, NULL);
    while(WAIT)
        ;
    printf("PID: %d\n", getpid());
    if(using_kill || using_sigrt) {
        for(int i = 0; i < RECEIVED_SIGNALS; i++) {
            kill(PID, sig1);
        }
        kill(PID, sig2);
    } else if(using_sigqueue) {
        for(int i = 0; i < RECEIVED_SIGNALS; i++) {
            sigqueue(PID, sig1, sig);
        }
        sigqueue(PID, sig2, sig);
    }
    return 0;
}

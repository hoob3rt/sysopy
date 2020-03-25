#define _XOPEN_SOURCE 500

#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int call_ls(const char* fpath, const struct stat* sb, int typeflag) {
    if(typeflag == FTW_D) {
        printf("\ndirectory: %s/\n", fpath);
        pid_t process;
        if((process = fork()) == 0) {
            printf("current pid: %d\n", (int)getpid());
            printf("parent ppid: %d\n", (int)getppid());
            execl("/bin/ls", "ls", "-l", fpath, NULL);
            exit(0);
        }
        int status;
        waitpid(process, &status, 0);
    }
    return 0;
}

int main(int argc, char** argv) {
    if(argc != 2) {
        exit(1);
    }
    int max_depth = 10;
    ftw(argv[1], call_ls, max_depth);
    return 0;
}

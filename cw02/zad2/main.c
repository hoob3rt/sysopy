#define _XOPEN_SOURCE 500

#include <dirent.h>
#include <ftw.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

static int64_t MAX = 200;  // maximal length of real path
int64_t DAYS = -1;
int64_t MAX_DEPTH = 1;
bool MTIME_PROVIDED = false;
bool ATIME_PROVIDED = false;
enum sign { minus, plus, zero };
enum sign SGN = zero;

char* get_file_type(unsigned char type) {
    if(type == 1) {
        return "pipe";
    } else if(type == 2) {
        return "character device";
    } else if(type == 4) {
        return "directory";
    } else if(type == 6) {
        return "block device";
    } else if(type == 8) {
        return "regular file";
    } else if(type == 10) {
        return "symbolic link";
    } else if(type == 12) {
        return "socket";
    }
    return "???";
}

char* get_file_type_nftw(int64_t type) {
    if(type == FTW_F) {
        return "regular file";
    } else if(type == FTW_D) {
        return "directory";
    } else if(type == FTW_SL) {
        return "symbolic link";
    }
    return "???";
}

void log_info(char* absolute_path, struct stat* stats, unsigned char type,
              char* tm_mod_time, char* tm_acc_time) {
    printf("%s\n", absolute_path);
    printf("Type: %s\n", get_file_type(type));
    printf("Links: %lu\n", stats->st_nlink);
    printf("Byes: %ld\n", stats->st_size);
    printf("Moddified: %s\n", tm_mod_time);
    printf("Accessed: %s\n\n", tm_acc_time);
}

void log_info_nftw(char* absolute_path, const struct stat* stats, int64_t type,
                   char* tm_mod_time, char* tm_acc_time) {
    printf("%s\n", absolute_path);
    printf("Type: %s\n", get_file_type_nftw(type));
    printf("Links: %lu\n", stats->st_nlink);
    printf("Bytes: %ld\n", stats->st_size);
    printf("Moddified: %s\n", tm_mod_time);
    printf("Accessed: %s\n\n", tm_acc_time);
}

bool time_matches_constraint(struct tm* time_to_verify) {
    int64_t time_to_verify_days = time_to_verify->tm_mday +
                                  31 * time_to_verify->tm_mon +
                                  365 * time_to_verify->tm_year;
    time_t currentTime;
    struct tm* currTime;
    currentTime = time(NULL);
    currTime = localtime(&currentTime);
    int64_t days = 0;
    days = currTime->tm_mday + 31 * currTime->tm_mon + 365 * currTime->tm_year;
    if(SGN == zero) {
        if(days - time_to_verify_days == DAYS) {
            return true;
        }
    } else if(SGN == plus) {
        if(days - time_to_verify_days > DAYS) {
            return true;
        }
    } else if(SGN == minus) {
        if(days - time_to_verify_days < DAYS) {
            return true;
        }
    }
    return false;
}

void stat_find(char* dir_path, int64_t depth) {
    if(depth < 0) {
        return;
    }
    DIR* dir = opendir(dir_path);
    if(dir == NULL) {
        perror("failed to open directory");
        exit(1);
    }
    struct dirent* fptr;
    struct tm* tm_mod_time;
    struct tm* tm_acc_time;
    char* absolute_path = calloc(MAX, sizeof(char));
    char* next_path = calloc(MAX, sizeof(char));
    struct stat stats;
    while((fptr = readdir(dir)) != NULL) {
        if(strcmp(fptr->d_name, ".") == 0 || strcmp(fptr->d_name, "..") == 0) {
            continue;
        }
        strcpy(next_path, dir_path);
        strcat(next_path, "/");
        strcat(next_path, fptr->d_name);
        realpath(next_path, absolute_path);
        if(lstat(next_path, &stats) < 0) {
            perror("lstat error");
            exit(1);
        }
        tm_mod_time = localtime(&stats.st_mtime);
        char* mod_time = calloc(40, sizeof(char));
        if(strftime(mod_time, 40, "%d.%m.%Y", tm_mod_time) == 0) {
            printf("Error during convering date to string!\n");
        }

        char* acc_time = calloc(40, sizeof(char));
        tm_acc_time = localtime(&stats.st_atime);
        if(strftime(acc_time, 40, "%d.%m.%Y", tm_acc_time) == 0) {
            printf("Error during convering date to string!\n");
        }

        if(ATIME_PROVIDED == true) {
            if(time_matches_constraint(tm_acc_time)) {
                log_info(absolute_path, &stats, fptr->d_type, mod_time,
                         acc_time);
            }
        } else if(MTIME_PROVIDED == true) {
            if(time_matches_constraint(tm_mod_time)) {

                log_info(absolute_path, &stats, fptr->d_type, mod_time,
                         acc_time);
            }
        } else if(MTIME_PROVIDED == ATIME_PROVIDED) {
            log_info(absolute_path, &stats, fptr->d_type, mod_time, acc_time);
        } else {
            exit(1);
        }
        if(fptr->d_type == 4) {  // DT_DIT
            stat_find(next_path, depth - 1);
        }
        free(acc_time);
        free(mod_time);
    }
    free(absolute_path);
    free(next_path);
    free(fptr);
    closedir(dir);
}

int nftw_wrapper(const char* fpath, const struct stat* sb, int tflag,
                 struct FTW* ftwbuf) {
    if(ftwbuf->level > MAX_DEPTH + 1 || strcmp(fpath, ".") == 0) {
        return 0;
    }
    struct tm* tm_mod_time;
    struct tm* tm_acc_time;
    char* absolute_path = calloc(MAX, sizeof(char));
    tm_mod_time = localtime(&sb->st_mtime);
    char* mod_time = calloc(40, sizeof(char));
    if(strftime(mod_time, 40, "%d.%m.%Y", tm_mod_time) == 0) {
        printf("Error during convering date to string!\n");
    }
    char* acc_time = calloc(40, sizeof(char));
    tm_acc_time = localtime(&sb->st_atime);
    if(strftime(acc_time, 40, "%d.%m.%Y", tm_acc_time) == 0) {
        printf("Error during convering date to string!\n");
    }
    realpath(fpath, absolute_path);

    if(ATIME_PROVIDED == true) {
        if(time_matches_constraint(tm_acc_time)) {
            log_info_nftw(absolute_path, sb, tflag, mod_time, acc_time);
        }
    } else if(MTIME_PROVIDED == true) {
        if(time_matches_constraint(tm_mod_time)) {

            log_info_nftw(absolute_path, sb, tflag, mod_time, acc_time);
        }
    } else if(MTIME_PROVIDED == ATIME_PROVIDED) {
        log_info_nftw(absolute_path, sb, tflag, mod_time, acc_time);
    } else {
        exit(1);
    }
    free(acc_time);
    free(mod_time);
    return 0;
}

void handle_stdin(int64_t argc, char* argv[], int64_t* current_index,
                  bool* path_provided, char** path) {
    if(strcmp(argv[*current_index], "-maxdepth") == 0) {
        (*current_index)++;
        MAX_DEPTH = atoi(argv[*current_index]);
        (*current_index)++;
    } else if(strcmp(argv[*current_index], "-atime") == 0) {
        (*current_index)++;
        ATIME_PROVIDED = true;
        if(argv[3][0] == 43) {  // +
            SGN = plus;
            char* ptr = (char*)(&argv[3][1]);
            DAYS = atoi(ptr);
        } else if(argv[3][0] == 45)  // -
        {
            SGN = minus;
            char* ptr = (char*)(&argv[3][1]);
            DAYS = atoi(ptr);
        }
        (*current_index)++;
    } else if(strcmp(argv[*current_index], "-mtime") == 0) {
        (*current_index)++;
        MTIME_PROVIDED = true;
        if(argv[3][0] == 43) {  // +
            SGN = plus;
            char* ptr = (char*)(&argv[3][1]);
            DAYS = atoi(ptr);
        } else if(argv[3][0] == 45)  // -
        {
            SGN = minus;
            char* ptr = (char*)(&argv[3][1]);
            DAYS = atoi(ptr);
        }
        (*current_index)++;
    } else {
        if((*path_provided) == false) {
            (*path_provided) = true;
            (*path) = realloc((*path), strlen(argv[(*current_index)]));
            strcpy((*path), argv[(*current_index)]);
            (*current_index)++;
        } else {
            printf("wrong arguments\n");
            exit(1);
        }
    }
}

void wrapper(char* path) {
    printf("Using stat: \n\n");
    stat_find(path, MAX_DEPTH);
    printf("Using nftw: \n\n");
    nftw(path, nftw_wrapper, 10, FTW_PHYS);
}

int main(int argc, char* argv[]) {
    if(argc < 2) {
        printf("enter more cl arguments\n");
        exit(1);
    }
    char* path = calloc(0, sizeof(char));
    int64_t current_index = 1;
    bool path_provided = false;
    while(current_index < argc) {
        handle_stdin(argc, argv, &current_index, &path_provided, &path);
    }
    if(path_provided == false) {
        printf("no path provided\n");
        exit(1);
    }
    wrapper(path);
    return 0;
}

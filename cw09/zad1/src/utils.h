#ifndef UTILS_H
#define UTILS_H value

#include <pthread.h>
#include <stdbool.h>

struct arg_struct {
    pthread_mutex_t mutex;
    pthread_cond_t condition;
    pthread_t current_client_id;
    pthread_t* chair_thread_array;
    int free_chairs_count;
    int next_chair;
    int next_free_chair;
    int chairs_count;
    int total_clients_count;
    int total_clients_cut;
    bool is_sleeping;
};
#endif /* ifndef UTILS_H */

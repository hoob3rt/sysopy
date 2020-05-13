#ifndef CLIENT_H
#define CLIENT_H value

#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <time.h>
#include <unistd.h>

void* client(void* arguments) {
    struct arg_struct* args = arguments;
    pthread_t clientId = pthread_self();
    sleep(rand() % 3 + 1);
    while(true) {
        pthread_mutex_lock(&args->mutex);
        if(args->is_sleeping) {
            args->current_client_id = clientId;
            printf("Client %ld is waking up barber\n", clientId);
            pthread_cond_broadcast(&args->condition);
            break;
        } else if(args->free_chairs_count > 0) {
            args->chair_thread_array[args->next_free_chair] = clientId;
            args->next_free_chair =
                (args->next_free_chair + 1) % args->chairs_count;
            args->free_chairs_count--;
            printf("Client %ld is in waiting with %d free spaces remaining\n",
                   clientId, args->free_chairs_count);
            break;
        } else {
            printf("There are no free spaces remaining, client %ld is waiting "
                   "for free space\n",
                   clientId);
            pthread_mutex_unlock(&args->mutex);
            sleep((rand() % 3 + 1) * 2);
        }
    }
    pthread_mutex_unlock(&args->mutex);
    pthread_exit((void*)0);
}

#endif /* ifndef CLIENT_H */

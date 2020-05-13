#ifndef BARBER_H
#define BARBER_H value

#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <time.h>
#include <unistd.h>

void* barber(void* arguments) {
    struct arg_struct* args = arguments;
    while(true) {
        pthread_mutex_lock(&args->mutex);
        if(args->free_chairs_count == args->chairs_count) {
            printf("Barber is going to sleep\n");
            args->is_sleeping = true;
            pthread_cond_wait(&args->condition, &args->mutex);
            args->is_sleeping = false;
        } else {
            args->current_client_id =
                args->chair_thread_array[args->next_chair];
            args->next_chair = (args->next_chair + 1) % args->chairs_count;
            args->free_chairs_count++;
        }
        printf("Barber is cutting client %ld, there are %d clients waiting in "
               "line\n",
               args->current_client_id,
               args->chairs_count - args->free_chairs_count);
        pthread_mutex_unlock(&args->mutex);
        sleep((rand() % 3 + 1) * 2);
        pthread_mutex_lock(&args->mutex);
        args->total_clients_cut++;
        if(args->total_clients_cut == args->total_clients_count) {
            printf("Barber finished cutting");
            pthread_mutex_unlock(&args->mutex);
            break;
        }
        printf("Barber finished cutting client %ld\n",
               args->current_client_id);
        pthread_mutex_unlock(&args->mutex);
    }
    pthread_exit((void*)0);
}

#endif /* ifndef BARBER_H */

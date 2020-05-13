#include "barber.h"
#include "client.h"
#include "utils.h"

int main(int argc, char** argv) {
    if(argc != 3) {
        exit(1);
    }
    struct arg_struct args = {
        .mutex = PTHREAD_MUTEX_INITIALIZER,
        .condition = PTHREAD_COND_INITIALIZER,
        .chairs_count = atoi(argv[1]),
        .total_clients_count = atoi(argv[2]),
        .next_free_chair = 0,
        .free_chairs_count = atoi(argv[1]),
        .chair_thread_array = calloc(atoi(argv[1]), sizeof(pthread_t)),
        .next_chair = 0,
        .total_clients_cut = 0,
        .is_sleeping = 0,
    };
    srand(time(NULL));
    pthread_t* clients =
        calloc(args.total_clients_count + 1, sizeof(pthread_t));
    pthread_create(&clients[0], NULL, barber, &args);
    for(int i = 1; i < args.total_clients_count + 1; i++) {
        pthread_create(&clients[i], NULL, client, &args);
    }
    for(int i = 0; i < args.total_clients_count + 1; i++) {
        pthread_join(clients[i], NULL);
    }
    return 0;
}

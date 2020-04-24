#ifndef COMMON_H
#define COMMON_H
#define MAX_CLIENTS 5
#define MAX_SIZE 1024
#include <sys/types.h>

typedef enum message_type {
    STOP = 1,
    DISCONNECT = 2,
    INIT = 3,
    LIST = 4,
    CONNECT = 5
} message_type;

typedef struct message {
    long message_type;
    char text[MAX_SIZE];
    int client_id;
    int connected_client_id;
    key_t queue_key;
} message;

const int MESSAGE_SIZE = sizeof(message) - sizeof(long);
const int SERVER_KEY_ID = 5;

#endif

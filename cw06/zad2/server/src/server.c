#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200112L
#endif /*  _POSIX_C_SOURCE  */
#include "common.h"

#include <mqueue.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>

char* CLIENT_QUEUES[MAX_CLIENTS];
int AVAILABLE_CLIENTS[MAX_CLIENTS];
int SERVER_QUEUE;
mqd_t SERVER_QUEUE_ID;

int get_next_client_id() {
    int i = 0;
    while(i < MAX_CLIENTS && CLIENT_QUEUES[i] != NULL)
        i++;
    if(i < MAX_CLIENTS) {
        return (i + 1);
    } else {
        return -1;
    }
}

void init(char* msg) {
    printf("INIT\n");
    int new_id = get_next_client_id();
    if(new_id < 0) {
        return;
    }
    char* reply_message = (char*)calloc(MAX_SIZE, sizeof(char));
    mqd_t client_queue_id = mq_open(msg, O_RDWR, 0666, NULL);
    if(client_queue_id < 0) {
        exit(EXIT_FAILURE);
    }
    AVAILABLE_CLIENTS[new_id - 1] = 1;
    CLIENT_QUEUES[new_id - 1] = (char*)calloc(NAME_LEN, sizeof(char));
    strcpy(CLIENT_QUEUES[new_id - 1], msg);
    if(mq_send(client_queue_id, reply_message, MAX_SIZE, new_id) < 0) {
        exit(EXIT_FAILURE);
    }
    if(mq_close(client_queue_id) < 0) {
        exit(EXIT_FAILURE);
    }
}

void list(char* msg) {
    printf("LIST\n");
    char* reply_message = (char*)calloc(MAX_SIZE, sizeof(char));
    int client_id = (int)msg[0];
    for(int i = 0; i < MAX_CLIENTS; i++) {
        if(CLIENT_QUEUES[i] != NULL) {
            sprintf(reply_message + strlen(reply_message),
                    "ID: %d, new client is up: %d\n", i + 1,
                    AVAILABLE_CLIENTS[i] == 1);
        }
    }
    mqd_t client_queue_id =
        mq_open(CLIENT_QUEUES[client_id - 1], O_RDWR, 0666, NULL);
    if(client_queue_id < 0) {
        exit(EXIT_FAILURE);
    }
    if(mq_send(client_queue_id, reply_message, MAX_SIZE, LIST) < 0) {
        exit(EXIT_FAILURE);
    }
    if(mq_close(client_queue_id) < 0) {
        exit(EXIT_FAILURE);
    }
}

void connect(char* msg) {
    printf("CONNECT\n");
    char* reply_message = (char*)calloc(MAX_SIZE, sizeof(char));
    int client_id = (int)msg[0];
    int second_client_id = (int)msg[1];
    mqd_t client_queue_id =
        mq_open(CLIENT_QUEUES[client_id - 1], O_RDWR, 0666, NULL);
    if(client_queue_id < 0) {
        exit(EXIT_FAILURE);
    }
    reply_message[0] = second_client_id;
    strcat(reply_message, CLIENT_QUEUES[second_client_id - 1]);
    if(mq_send(client_queue_id, reply_message, MAX_SIZE, CONNECT) < 0) {
        exit(EXIT_FAILURE);
    }
    memset(reply_message, 0,
           strlen(reply_message));  // kopiuje do reply stringa 0
    client_queue_id =
        mq_open(CLIENT_QUEUES[second_client_id - 1], O_RDWR, 0666, NULL);
    if(client_queue_id < 0) {
        exit(EXIT_FAILURE);
    }
    reply_message[0] = client_id;
    strcat(reply_message, CLIENT_QUEUES[client_id - 1]);
    if(mq_send(client_queue_id, reply_message, MAX_SIZE, CONNECT) < 0) {
        exit(EXIT_FAILURE);
    }
    if(mq_close(client_queue_id) < 0) {
        exit(EXIT_FAILURE);
    }
    AVAILABLE_CLIENTS[second_client_id - 1] = 0;
    AVAILABLE_CLIENTS[client_id - 1] = 0;
}

void disconnect(char* msg) {
    printf("DISCONNECT\n");
    char* reply_message = (char*)calloc(MAX_SIZE, sizeof(char));
    int client_id = (int)msg[0];
    int second_client_id = (int)msg[1];
    mqd_t client_queue_id =
        mq_open(CLIENT_QUEUES[second_client_id - 1], O_RDWR, 0666, NULL);
    if(client_queue_id < 0) {
        exit(EXIT_FAILURE);
    }
    if(mq_send(client_queue_id, reply_message, MAX_SIZE, DISCONNECT) < 0) {
        exit(EXIT_FAILURE);
    }
    if(mq_close(client_queue_id) < 0) {
        exit(EXIT_FAILURE);
    }

    AVAILABLE_CLIENTS[second_client_id - 1] = 1;
    AVAILABLE_CLIENTS[client_id - 1] = 1;
}

void stop(char* msg) {
    printf("Stop\n");
    AVAILABLE_CLIENTS[(int)msg[0] - 1] = 0;
    CLIENT_QUEUES[(int)msg[0] - 1] = NULL;
}

void quit(int signum) {
    char* reply_message = (char*)calloc(MAX_SIZE, sizeof(char));
    for(int i = 0; i < MAX_CLIENTS; i++) {
        if(CLIENT_QUEUES[i] != NULL) {
            mqd_t client_queue_id =
                mq_open(CLIENT_QUEUES[i], O_RDWR, 0666, NULL);
            if(client_queue_id < 0) {
                exit(EXIT_FAILURE);
            }
            if(mq_send(client_queue_id, reply_message, MAX_SIZE, STOP) < 0) {
                exit(EXIT_FAILURE);
            }
            if(mq_receive(SERVER_QUEUE_ID, reply_message, MAX_SIZE, NULL) <
               0) {
                exit(EXIT_FAILURE);
            }
            if(mq_close(client_queue_id) < 0) {
                exit(EXIT_FAILURE);
            }
        }
    }
    if(mq_close(SERVER_QUEUE_ID) < 0) {
        exit(EXIT_FAILURE);
    }
    if(mq_unlink(SERVER_QUEUE_NAME) < 0) {
        exit(EXIT_FAILURE);
    }
    exit(0);
}

void process_message(char* msg, int type) {
    if(type == INIT) {
        init(msg);
    } else if(type == LIST) {
        list(msg);
    } else if(type == CONNECT) {
        connect(msg);
    } else if(type == STOP) {
        stop(msg);
    } else {
        printf("Wrong message: %d\n", type);
    }
}

int main() {
    for(int i = 0; i < MAX_CLIENTS; i++) {
        CLIENT_QUEUES[i] = NULL;
    }
    SERVER_QUEUE_ID = mq_open(SERVER_QUEUE_NAME, O_RDWR | O_CREAT, 0666, NULL);
    if(SERVER_QUEUE_ID < 0) {
        exit(EXIT_FAILURE);
    }
    signal(SIGINT, quit);
    char* msg = (char*)calloc(MAX_SIZE, sizeof(char));
    unsigned int type;
    while(1) {
        if(mq_receive(SERVER_QUEUE_ID, msg, MAX_SIZE, &type) < 0) {
            exit(EXIT_FAILURE);
        }
        process_message(msg, type);
    }
    return 0;
}

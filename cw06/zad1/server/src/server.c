#include "common.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

key_t CLIENT_QUEUES[MAX_CLIENTS];
int AVAILABLE_CLIENTS[MAX_CLIENTS];
int SERVER_QUEUE;

int get_next_client_id() {
    int i = 0;
    while(i < MAX_CLIENTS && CLIENT_QUEUES[i] != -1) {
        i++;
    }
    if(i < MAX_CLIENTS) {
        return (i + 1);
    } else {
        return -1;
    }
}

void init(message* msg) {
    printf("INIT\n");
    int new_id = get_next_client_id();
    if(new_id < 0) {
        return;
    }
    message* reply_message = (message*)malloc(sizeof(message));
    reply_message->message_type = new_id;
    int client_queue_id = msgget(msg->queue_key, 0);
    if(client_queue_id < 0) {
        exit(EXIT_FAILURE);
    }
    AVAILABLE_CLIENTS[new_id - 1] = 1;
    CLIENT_QUEUES[new_id - 1] = msg->queue_key;
    if(msgsnd(client_queue_id, reply_message, MESSAGE_SIZE, 0) < 0) {
        exit(EXIT_FAILURE);
    }
}

void list(message* msg) {
    printf("LIST\n");
    message* reply_message = (message*)malloc(sizeof(message));
    strcpy(reply_message->text, "");
    for(int i = 0; i < MAX_CLIENTS; i++) {
        if(CLIENT_QUEUES[i] != -1) {
            sprintf(reply_message->text + strlen(reply_message->text),
                    "ID: %d, new client id up: %d\n", i + 1,
                    AVAILABLE_CLIENTS[i] == 1);
        }
    }
    int client_queue_id = msgget(CLIENT_QUEUES[msg->client_id - 1], 0);
    if(client_queue_id < 0) {
        exit(EXIT_FAILURE);
    }
    reply_message->message_type = msg->client_id;
    if(msgsnd(client_queue_id, reply_message, MESSAGE_SIZE, 0) < 0) {
        exit(EXIT_FAILURE);
    }
}

void connect(message* msg) {
    printf("CONNECT\n");
    message* reply_message = (message*)malloc(sizeof(message));
    reply_message->message_type = CONNECT;
    reply_message->queue_key = CLIENT_QUEUES[msg->connected_client_id - 1];
    int client_queue_id = msgget(CLIENT_QUEUES[msg->client_id - 1], 0);
    if(client_queue_id < 0) {
        exit(EXIT_FAILURE);
    }
    if(msgsnd(client_queue_id, reply_message, MESSAGE_SIZE, 0) < 0) {
        exit(EXIT_FAILURE);
    }
    reply_message->message_type = CONNECT;
    reply_message->queue_key = CLIENT_QUEUES[msg->client_id - 1];
    reply_message->client_id = msg->client_id;
    client_queue_id = msgget(CLIENT_QUEUES[msg->connected_client_id - 1], 0);
    if(client_queue_id < 0) {
        exit(EXIT_FAILURE);
    }
    if(msgsnd(client_queue_id, reply_message, MESSAGE_SIZE, 0) < 0) {
        exit(EXIT_FAILURE);
    }
    AVAILABLE_CLIENTS[msg->connected_client_id - 1] = 0;
    AVAILABLE_CLIENTS[msg->client_id - 1] = 0;
}

void disconnnect(message* msg) {
    printf("DISCONNECT\n");
    message* reply_message = (message*)malloc(sizeof(message));
    reply_message->message_type = DISCONNECT;
    int client_queue_id =
        msgget(CLIENT_QUEUES[msg->connected_client_id - 1], 0);
    if(client_queue_id < 0) {
        exit(EXIT_FAILURE);
    }
    if(msgsnd(client_queue_id, reply_message, MESSAGE_SIZE, 0) < 0) {
        exit(EXIT_FAILURE);
    }
    AVAILABLE_CLIENTS[msg->connected_client_id - 1] = 1;
    AVAILABLE_CLIENTS[msg->client_id - 1] = 1;
}

void stop(message* msg) {
    printf("Stop\n");
    AVAILABLE_CLIENTS[msg->client_id - 1] = 0;
    CLIENT_QUEUES[msg->client_id - 1] = -1;
}

void quit(int signum) {
    message* reply_message = (message*)malloc(sizeof(message));
    for(int i = 0; i < MAX_CLIENTS; i++) {
        key_t queue_key = CLIENT_QUEUES[i];
        if(queue_key != -1) {
            reply_message->message_type = STOP;
            int client_queue_id = msgget(queue_key, 0);
            if(client_queue_id < 0) {
                exit(EXIT_FAILURE);
            }
            if(msgsnd(client_queue_id, reply_message, MESSAGE_SIZE, 0) < 0) {
                exit(EXIT_FAILURE);
            }
            if(msgrcv(SERVER_QUEUE, reply_message, MESSAGE_SIZE, STOP, 0) <
               0) {
                exit(EXIT_FAILURE);
            }
        }
    }
    msgctl(SERVER_QUEUE, IPC_RMID, NULL);
    exit(0);
}

void process_message(message* msg) {
    if(msg->message_type == INIT) {
        init(msg);
    } else if(msg->message_type == LIST) {
        list(msg);
    } else if(msg->message_type == CONNECT) {
        connect(msg);
    } else if(msg->message_type == DISCONNECT) {
        disconnnect(msg);
    } else if(msg->message_type == STOP) {
        stop(msg);
    } else {
        printf("Wrong message: %ld\n", msg->message_type);
    }
}

int main() {
    for(int i = 0; i < MAX_CLIENTS; i++) {
        CLIENT_QUEUES[i] = -1;
    }
    key_t queue_key = ftok(getenv("HOME"), SERVER_KEY_ID);
    printf("Server queue key: %d\n", queue_key);
    SERVER_QUEUE = msgget(queue_key, IPC_CREAT | 0666);
    printf("Server queue id: %d\n", SERVER_QUEUE);
    signal(SIGINT, quit);
    message* msg = (message*)malloc(sizeof(message));
    while(1) {
        if(msgrcv(SERVER_QUEUE, msg, MESSAGE_SIZE, -6, 0) < 0) {
            exit(EXIT_FAILURE);
        }
        process_message(msg);
    }
    return 0;
}

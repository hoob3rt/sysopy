#include "common.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <unistd.h>

int CLIENT_ID;
int QUEUE_ID;
int SERVER_QUEUE_ID;
key_t KEY;

void stop() {
    message* msg = (message*)malloc(sizeof(message));
    msg->client_id = CLIENT_ID;
    msg->message_type = STOP;
    if(msgsnd(SERVER_QUEUE_ID, msg, MESSAGE_SIZE, 0) < 0) {
        exit(EXIT_FAILURE);
    }
    if(msgctl(QUEUE_ID, IPC_RMID, NULL) < 0) {
        exit(EXIT_FAILURE);
    }
    msgctl(QUEUE_ID, IPC_RMID, NULL);
    exit(0);
}

void chat(int second_client_id, int second_queue_id) {
    message* msg = (message*)malloc(sizeof(message));
    char* command = NULL;
    size_t length = 0;
    ssize_t rread = 0;
    while(1) {
        printf("Enter message or dc: ");
        rread = getline(&command, &length, stdin);
        command[rread - 1] = '\0';
        if(msgrcv(QUEUE_ID, msg, MESSAGE_SIZE, STOP, IPC_NOWAIT) >= 0) {
            stop();
        }
        if(msgrcv(QUEUE_ID, msg, MESSAGE_SIZE, DISCONNECT, IPC_NOWAIT) >= 0) {
            break;
        }
        while(msgrcv(QUEUE_ID, msg, MESSAGE_SIZE, 0, IPC_NOWAIT) >= 0) {
            printf("Client %d: %s\n", second_client_id, msg->text);
        }
        if(strcmp(command, "DISCONNECT") == 0) {
            msg->client_id = CLIENT_ID;
            msg->connected_client_id = second_client_id;
            msg->message_type = DISCONNECT;
            if(msgsnd(SERVER_QUEUE_ID, msg, MESSAGE_SIZE, 0) < 0) {
                exit(EXIT_FAILURE);
            }
            break;
        } else if(strcmp(command, "") != 0) {
            msg->message_type = CONNECT;
            strcpy(msg->text, command);
            if(msgsnd(second_queue_id, msg, MESSAGE_SIZE, 0) < 0) {
                exit(EXIT_FAILURE);
            }
        }
    }
}

void connect(int second_client_id) {
    message* msg = (message*)malloc(sizeof(message));
    msg->message_type = CONNECT;
    msg->client_id = CLIENT_ID;
    msg->connected_client_id = second_client_id;
    if(msgsnd(SERVER_QUEUE_ID, msg, MESSAGE_SIZE, 0) < 0) {
        exit(EXIT_FAILURE);
    }
    message* reply_msg = (message*)malloc(sizeof(message));
    if(msgrcv(QUEUE_ID, reply_msg, MESSAGE_SIZE, 0, 0) < 0) {
        exit(EXIT_FAILURE);
    }
    key_t second_queue_key = reply_msg->queue_key;
    int second_queue_id = msgget(second_queue_key, 0);
    if(second_queue_id < 0) {
        exit(EXIT_FAILURE);
    }
    chat(second_client_id, second_queue_id);
}

void list() {
    message* msg = (message*)malloc(sizeof(message));
    msg->client_id = CLIENT_ID;
    msg->message_type = LIST;
    if(msgsnd(SERVER_QUEUE_ID, msg, MESSAGE_SIZE, 0) < 0) {
        exit(EXIT_FAILURE);
    }
    message* reply_msg = (message*)malloc(sizeof(message));
    if(msgrcv(QUEUE_ID, reply_msg, MESSAGE_SIZE, 0, 0) < 0) {
        exit(EXIT_FAILURE);
    }
    printf("%s\n", reply_msg->text);
}

void look_for_server_message() {
    message* msg = (message*)malloc(sizeof(message));
    if(msgrcv(QUEUE_ID, msg, MESSAGE_SIZE, 0, IPC_NOWAIT) < 0) {
        return;
    }
    if(msg->message_type == CONNECT) {
        int second_queue_id = msgget(msg->queue_key, 0);
        if(second_queue_id < 1) {
            exit(EXIT_FAILURE);
        }
        chat(msg->client_id, second_queue_id);
    } else if(msg->message_type == STOP) {
        stop();
    }
}

void quit(int signum) { stop(); }

int main() {
    srand(time(NULL));
    KEY = ftok(getenv("HOME"), rand() % 255 + 6);
    printf("KEY: %d\n", KEY);
    QUEUE_ID = msgget(KEY, IPC_CREAT | 0666);
    if(QUEUE_ID < 0) {
        exit(EXIT_FAILURE);
    } else {
        printf("Queue ID: %d\n", QUEUE_ID);
    }
    key_t server_key = ftok(getenv("HOME"), SERVER_KEY_ID);
    SERVER_QUEUE_ID = msgget(server_key, 0);
    if(SERVER_QUEUE_ID < 0) {
        exit(EXIT_FAILURE);
    } else {
        printf("Server Queue ID: %d\n", SERVER_QUEUE_ID);
    }
    message* msg = (message*)malloc(sizeof(message));
    msg->queue_key = KEY;
    msg->message_type = INIT;
    if(msgsnd(SERVER_QUEUE_ID, msg, MESSAGE_SIZE, 0) < 0) {
        exit(EXIT_FAILURE);
    }
    message* reply_msg = (message*)malloc(sizeof(message));
    if(msgrcv(QUEUE_ID, reply_msg, MESSAGE_SIZE, 0, 0) < 0) {
        exit(EXIT_FAILURE);
    }
    CLIENT_ID = reply_msg->message_type;
    printf("Client ID: %d\n", CLIENT_ID);
    signal(SIGINT, quit);
    char* command = NULL;
    size_t length = 0;
    ssize_t rread = 0;
    while(1) {
        printf("Command ");
        rread = getline(&command, &length, stdin);
        command[rread - 1] = '\0';
        look_for_server_message();
        if(strcmp(command, "") == 0) {
            continue;
        }
        char* buffer = strtok(command, " ");
        if(!strcmp(buffer, "STOP"))
            stop();
        else if(!strcmp(buffer, "LIST")) {
            list();
        } else if(!strcmp(buffer, "CONNECT")) {
            buffer = strtok(NULL, " ");
            int id = atoi(buffer);
            connect(id);
        } else {
            printf("Wrong command %s\n", command);
        }
    }
    return 0;
}

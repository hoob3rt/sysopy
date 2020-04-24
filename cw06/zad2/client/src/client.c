#include "common.h"

#include <mqueue.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

char* QUEUE_NAME;
int CLIENT_ID;
mqd_t QUEUE_ID;
mqd_t SERVER_QUEUE_ID;

void stop() {
    char* msg = (char*)calloc(MAX_SIZE, sizeof(char));
    msg[0] = CLIENT_ID;
    if(mq_send(SERVER_QUEUE_ID, msg, MAX_SIZE, STOP) < 0) {
        exit(EXIT_FAILURE);
    }
    if(mq_close(SERVER_QUEUE_ID) < 0) {
        exit(EXIT_FAILURE);
    }
}

void chat(int second_client_id, mqd_t second_queue) {
    char* msg = (char*)calloc(MAX_SIZE, sizeof(char));
    char* command = NULL;
    size_t length = 0;
    ssize_t rread = 0;
    while(1) {
        printf("Enter message or dc: ");
        rread = getline(&command, &length, stdin);
        command[rread - 1] = '\0';
        struct timespec* timespec =
            (struct timespec*)malloc(sizeof(struct timespec));
        unsigned int type;
        int is_disconnect = 0;
        while(mq_timedreceive(QUEUE_ID, msg, MAX_SIZE, &type, timespec) >= 0) {
            if(type == STOP) {
                stop();
            } else if(type == DISCONNECT) {
                is_disconnect = 1;
                break;
            } else {
                printf("[Client %d]: %s\n", second_client_id, msg);
            }
        }
        if(is_disconnect == 1)
            break;
        if(strcmp(command, "DISCONNECT") == 0) {
            msg[0] = CLIENT_ID;
            msg[1] = second_client_id;
            if(mq_send(SERVER_QUEUE_ID, msg, MAX_SIZE, DISCONNECT) < 0) {
                exit(EXIT_FAILURE);
            }
            break;
        } else if(strcmp(command, "") != 0) {
            strcpy(msg, command);
            if(mq_send(second_queue, msg, MAX_SIZE, CONNECT) < 0) {
                exit(EXIT_FAILURE);
            }
        }
    }
}

void connect(int second_client_id) {
    char* msg = (char*)calloc(MAX_SIZE, sizeof(char));
    msg[0] = CLIENT_ID;
    msg[1] = second_client_id;
    if(mq_send(SERVER_QUEUE_ID, msg, MAX_SIZE, CONNECT) < 0) {
        exit(EXIT_FAILURE);
    }
    if(mq_receive(QUEUE_ID, msg, MAX_SIZE, NULL) < 0) {
        exit(EXIT_FAILURE);
    }
    char* second_queue_name = (char*)calloc(MAX_SIZE, sizeof(char));
    strncpy(second_queue_name, msg + 1, strlen(msg) - 1);
    printf("second queue name: %s\n", second_queue_name);
    mqd_t second_queue =
        mq_open(second_queue_name, O_RDWR, 0666,
                NULL); 
    if(second_queue < 0) {
        exit(EXIT_FAILURE);
    }
    chat(second_client_id, second_queue);
}

void list() {
    char* msg = (char*)calloc(MAX_SIZE, sizeof(char));
    msg[0] = CLIENT_ID;
    if(mq_send(SERVER_QUEUE_ID, msg, MAX_SIZE, LIST) < 0) {
        exit(EXIT_FAILURE);
    }
    if(mq_receive(QUEUE_ID, msg, MAX_SIZE, NULL) < 0) {
        exit(EXIT_FAILURE);
    }
    printf("%s\n", msg);
}

void look_for_server_message() {
    char* msg = (char*)calloc(MAX_SIZE, sizeof(char));
    struct timespec* timespec =
        (struct timespec*)malloc(sizeof(struct timespec));
    unsigned int type;
    if(mq_timedreceive(QUEUE_ID, msg, MAX_SIZE, &type, timespec) < 0)
        return;
    if(type == CONNECT) {
        char* second_queue_name = (char*)calloc(NAME_LEN, sizeof(char));
        strncpy(second_queue_name, msg + 1, strlen(msg) - 1);
        printf("Other name: %s\n", second_queue_name);
        mqd_t second_queue_id = mq_open(second_queue_name, O_RDWR, 0666, NULL);
        if(second_queue_id < 0) {
            exit(EXIT_FAILURE);
        }
        chat((int)msg[0], second_queue_id);
    } else if(type == STOP) {
        stop();
    }
}

void quit(int signum) { stop(); }

int main(int argc, char** argv) {
    QUEUE_NAME = argv[1];
    QUEUE_ID = mq_open(QUEUE_NAME, O_RDWR | O_CREAT, 0666, NULL);
    if(QUEUE_ID < 0) {
        exit(EXIT_FAILURE);
    }
    SERVER_QUEUE_ID = mq_open(SERVER_QUEUE_NAME, O_RDWR, 0666, NULL);
    if(SERVER_QUEUE_ID < 0) {
        exit(EXIT_FAILURE);
    }
    char* msg = (char*)calloc(MAX_SIZE, sizeof(char));
    strcpy(msg, QUEUE_NAME);
    if(mq_send(SERVER_QUEUE_ID, msg, MAX_SIZE, INIT) < 0) {
        exit(EXIT_FAILURE);
    }
    unsigned int c_id;
    if(mq_receive(QUEUE_ID, msg, MAX_SIZE, &c_id) < 0) {
        exit(EXIT_FAILURE);
    }
    printf("Received client ID: %d\n", c_id);
    CLIENT_ID = c_id;
    signal(SIGINT, quit);
    while(1) {
        printf("Command ");
        char* command = NULL;
        size_t length = 0;
        ssize_t rread = 0;
        rread = getline(&command, &length, stdin);
        command[rread - 1] = '\0';
        look_for_server_message();
        if(strcmp(command, "") == 0) {
            continue;
        }
        char* buffer = strtok(command, " ");
        if(!strcmp(buffer, "STOP"))
            stop();
        else if(!strcmp(buffer, "LIST"))
            list();
        else if(!strcmp(buffer, "CONNECT")) {
            buffer = strtok(NULL, " ");
            int id = atoi(buffer);
            connect(id);
        } else {
            printf("Wrong command %s\n", command);
        }
    }
    return 0;
}

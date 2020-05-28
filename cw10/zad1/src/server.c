#define _POSIX_C_SOURCE 200112L

#include "utils.h"

#include <netdb.h>
#include <poll.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <time.h>
#include <unistd.h>

typedef struct client_struct {
  char *nick;
  int fd;
  int is_playing;
} client_struct;

pthread_mutex_t CLIENT_MUTEX = PTHREAD_MUTEX_INITIALIZER;
client_struct *CLIENTS[MAX_PLAYERS] = {NULL};
int CLIENTS_COUNT = 0;

int get_player_by_nick(char *nick) {
  for(int id = 0; id < MAX_PLAYERS; id++) {
    if(CLIENTS[id] != NULL && strcmp(CLIENTS[id]->nick, nick) == 0) {
      return id;
    }
  }
  return -1;
}

int get_opponent_id(int id) { return id % 2 == 0 ? id + 1 : id - 1; }

int add_player(char *nick, int fd) {
  if(get_player_by_nick(nick) != -1) {
    return -1;
  }
  int index = -1;
  for(int id = 0; id < MAX_PLAYERS; id += 2) {
    if(CLIENTS[id] != NULL && CLIENTS[id + 1] == NULL) {
      index = id + 1;
      break;
    }
  }
  for(int id = 0; id < MAX_PLAYERS && index == -1; id++) {
    if(CLIENTS[id] == NULL) {
      index = id;
    }
  }
  if(index != -1) {
    client_struct *new_client = calloc(1, sizeof(client_struct));
    new_client->nick = calloc(MAX_MESSAGE_LENGTH, sizeof(char));
    strcpy(new_client->nick, nick);
    new_client->fd = fd;
    new_client->is_playing = 1;
    CLIENTS[index] = new_client;
    CLIENTS_COUNT++;
  }
  return index;
}

void free_client(int client_id) {
  free(CLIENTS[client_id]->nick);
  free(CLIENTS[client_id]);
  CLIENTS[client_id] = NULL;
  CLIENTS_COUNT--;
}

int remove_client(char *nick) {
  printf("usuwam: %s\n", nick);
  int client_id = get_player_by_nick(nick);
  if(client_id == -1) {
    printf("brak gracza: %s\n", nick);
    return -1;
  }
  free_client(client_id);
  int opponent_id = get_opponent_id(client_id);
  if(CLIENTS[opponent_id] != NULL) {
    puts("usuwam przeciwnika");
    free_client(opponent_id);
  }
  return 0;
}

void ping() {
  puts("pinging");
  pthread_mutex_lock(&CLIENT_MUTEX);
  for(int i = 0; i < MAX_PLAYERS; i++) {
    if(CLIENTS[i] != NULL && !CLIENTS[i]->is_playing) {
      printf("Usuwam ping: %s\n", CLIENTS[i]->nick);
      remove_client(CLIENTS[i]->nick);
    }
  }
  for(int i = 0; i < MAX_PLAYERS; i++) {
    if(CLIENTS[i] != NULL) {
      puts("pinguje");
      send(CLIENTS[i]->fd, "ping: ", MAX_MESSAGE_LENGTH, 0);
      CLIENTS[i]->is_playing = 0;
    }
  }
  pthread_mutex_unlock(&CLIENT_MUTEX);
  sleep(1);
  ping();
}

int poll_sockets(int local_socket, int network_socket) {
  struct pollfd *pfds = calloc(2 + CLIENTS_COUNT, sizeof(struct pollfd));
  pfds[0].fd = local_socket;
  pfds[0].events = POLLIN;
  pfds[1].fd = network_socket;
  pfds[1].events = POLLIN;
  pthread_mutex_lock(&CLIENT_MUTEX);
  for(int i = 0; i < CLIENTS_COUNT; i++) {
    pfds[i + 2].fd = CLIENTS[i]->fd;
    pfds[i + 2].events = POLLIN;
  }
  pthread_mutex_unlock(&CLIENT_MUTEX);
  poll(pfds, CLIENTS_COUNT + 2, -1);
  int fd;
  for(int i = 0; i < CLIENTS_COUNT + 2; i++) {
    if(pfds[i].revents & POLLIN) {
      fd = pfds[i].fd;
      break;
    }
  }
  if(fd == local_socket || fd == network_socket) {
    fd = accept(fd, NULL, NULL);
  }
  free(pfds);
  return fd;
}

int init_local_socket(char *path) {
  int local_socket = socket(AF_UNIX, SOCK_STREAM, 0);
  struct sockaddr_un sockaddr;
  memset(&sockaddr, 0, sizeof(struct sockaddr_un));
  sockaddr.sun_family = AF_UNIX;
  strcpy(sockaddr.sun_path, path);
  unlink(path);
  bind(local_socket, (struct sockaddr *)&sockaddr,
    sizeof(struct sockaddr_un));
  listen(local_socket, MAX_BACKLOG);
  return local_socket;
}

int init_net_socket(char *port) {
  struct addrinfo *info;
  struct addrinfo hints;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  getaddrinfo(NULL, port, &hints, &info);
  int network_socket =
    socket(info->ai_family, info->ai_socktype, info->ai_protocol);
  bind(network_socket, info->ai_addr, info->ai_addrlen);
  listen(network_socket, MAX_BACKLOG);
  freeaddrinfo(info);
  return network_socket;
}

int main(int argc, char *argv[]) {
  srand(time(NULL));
  if(argc != 3) {
    return 1;
  }
  char *port = argv[1];
  char *socket_path = argv[2];
  int local_socket = init_local_socket(socket_path);
  int network_socket = init_net_socket(port);
  pthread_t t;
  pthread_create(&t, NULL, (void *(*)(void *))ping, NULL);
  while(1) {
    int client_fd = poll_sockets(local_socket, network_socket);
    char buffer[MAX_MESSAGE_LENGTH + 1];
    recv(client_fd, buffer, MAX_MESSAGE_LENGTH, 0);
    puts(buffer);
    char *cmd = strtok(buffer, ":");
    char *arg = strtok(NULL, ":");
    char *nick = strtok(NULL, ":");
    pthread_mutex_lock(&CLIENT_MUTEX);
    if(strcmp(cmd, "add") == 0) {
      int index = add_player(nick, client_fd);
      if(index == -1) {
        send(client_fd, "add:name_taken", MAX_MESSAGE_LENGTH, 0);
        close(client_fd);
      } else if(index % 2 == 0) {
        send(client_fd, "add:no_enemy", MAX_MESSAGE_LENGTH, 0);
      } else {
        int waiting_client_goes_first = rand() % 2;
        int first_player_index = index - waiting_client_goes_first;
        int second_player_index = get_opponent_id(first_player_index);
        send(CLIENTS[first_player_index]->fd, "add:O", MAX_MESSAGE_LENGTH, 0);
        send(CLIENTS[second_player_index]->fd, "add:X", MAX_MESSAGE_LENGTH, 0);
      }
    }
    if(strcmp(cmd, "move") == 0) {
      int move = atoi(arg);
      int player = get_player_by_nick(nick);
      sprintf(buffer, "move:%d", move);
      send(CLIENTS[get_opponent_id(player)]->fd, buffer, MAX_MESSAGE_LENGTH, 0);
    }
    if(strcmp(cmd, "quit") == 0) {
      remove_client(nick);
    }
    if(strcmp(cmd, "pinged") == 0) {
      int player = get_player_by_nick(nick);
      if(player != -1) {
        CLIENTS[player]->is_playing = 1;
      }
    }
    pthread_mutex_unlock(&CLIENT_MUTEX);
  }
}

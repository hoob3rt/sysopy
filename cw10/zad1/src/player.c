#define _POSIX_C_SOURCE 200112L

#include "utils.h"

#include <netdb.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>

typedef enum state_struct {
  START,
  WAIT_FOR_ENEMY,
  WAIT_FOR_MOVE,
  PROCESS_ENEMY_MOVE,
  MOVE,
  QUIT
} state_struct;

pthread_mutex_t REPLY_MUTEX = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t REPLY_CONDITION = PTHREAD_COND_INITIALIZER;
int SERVER_SOCKET;
bool IS_O;
char BUFFER[MAX_MESSAGE_LENGTH + 1];
char *NICK;
board_struct BOARD;
state_struct STATE = START;
char *CMD, *ARG;

void quit_game() {
  sprintf(BUFFER, "quit: :%s", NICK);
  send(SERVER_SOCKET, BUFFER, MAX_MESSAGE_LENGTH, 0);
  exit(0);
}

void check_win_draw() {
  bool finished = false;
  board_field winner = get_winner(&BOARD);
  if(winner != EMPTY) {
    if((IS_O && winner == O) || (!IS_O && winner == X)) {
      puts("GGEZ");
    } else {
      puts("L");
    }
    finished = true;
  }
  bool is_drawn = true;
  for(int i = 0; i < 9 && is_drawn == 1; i++) {
    if(BOARD.fields[i] == EMPTY) {
      is_drawn = false;
    }
  }
  if(is_drawn && !finished) {
    puts("Remis");
  }
  if(finished || is_drawn) {
    STATE = QUIT;
  }
}

void blit() {
  char symbols[3] = {'-', 'O', 'X'};
  for(int y = 0; y < 3; y++) {
    for(int x = 0; x < 3; x++) {
      symbols[0] = y * 3 + x + 1 + '0';
      printf("|%c|", symbols[BOARD.fields[y * 3 + x]]);
    }
    puts("\n---------");
  }
}

void game_loop() {
  IS_O = ARG[0] == 'O';
  if(STATE == START) {
    if(strcmp(ARG, "name_taken") == 0) {
      puts("nazwa zajeta");
      exit(1);
    } else if(strcmp(ARG, "no_enemy") == 0) {
      puts("brak przeciwnika");
      STATE = WAIT_FOR_ENEMY;
    } else {
      BOARD = get_empty_board();
      if(IS_O) {
        STATE = MOVE;
      } else {
        STATE = WAIT_FOR_MOVE;
      }
    }
  } else if(STATE == WAIT_FOR_ENEMY) {
    pthread_mutex_lock(&REPLY_MUTEX);
    while(STATE != START && STATE != QUIT) {
      pthread_cond_wait(&REPLY_CONDITION, &REPLY_MUTEX);
    }
    pthread_mutex_unlock(&REPLY_MUTEX);
    BOARD = get_empty_board();
    STATE = IS_O ? MOVE : WAIT_FOR_MOVE;
  } else if(STATE == WAIT_FOR_MOVE) {
    puts("czekam na przeciwnika");
    pthread_mutex_lock(&REPLY_MUTEX);
    while(STATE != PROCESS_ENEMY_MOVE && STATE != QUIT) {
      pthread_cond_wait(&REPLY_CONDITION, &REPLY_MUTEX);
    }
    pthread_mutex_unlock(&REPLY_MUTEX);
  } else if(STATE == PROCESS_ENEMY_MOVE) {
    int field_index = atoi(ARG);
    move(&BOARD, field_index);
    check_win_draw();
    if(STATE != QUIT) {
      STATE = MOVE;
    }
  } else if(STATE == MOVE) {
    blit();
    int field_index;
    while(!move(&BOARD, field_index)) {
      printf("przeciwnik wykonal ruch (%c): ", IS_O ? 'O' : 'X');
      scanf("%d", &field_index);
      field_index--;
    }
    blit();
    sprintf(BUFFER, "move:%d:%s", field_index, NICK);
    send(SERVER_SOCKET, BUFFER, MAX_MESSAGE_LENGTH, 0);
    check_win_draw();
    if(STATE != QUIT) {
      STATE = WAIT_FOR_MOVE;
    }
  } else if(STATE == QUIT) {
    quit_game();
  }
  game_loop();
}

int main(int argc, char *argv[]) {
  if(argc != 4) {
    return 1;
  }
  NICK = argv[1];
  char *type = argv[2];
  char *destination = argv[3];
  signal(SIGINT, quit_game);
  if(strcmp(type, "local") == 0) {
    SERVER_SOCKET = socket(AF_UNIX, SOCK_STREAM, 0);
    struct sockaddr_un local_sockaddr;
    memset(&local_sockaddr, 0, sizeof(struct sockaddr_un));
    local_sockaddr.sun_family = AF_UNIX;
    strcpy(local_sockaddr.sun_path, destination);
    connect(SERVER_SOCKET, (struct sockaddr *)&local_sockaddr,
      sizeof(struct sockaddr_un));
  } else {
    struct addrinfo *info;
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    getaddrinfo("localhost", destination, &hints, &info);
    SERVER_SOCKET =
      socket(info->ai_family, info->ai_socktype, info->ai_protocol);
    connect(SERVER_SOCKET, info->ai_addr, info->ai_addrlen);
    freeaddrinfo(info);
  }
  sprintf(BUFFER, "add: :%s", NICK);
  send(SERVER_SOCKET, BUFFER, MAX_MESSAGE_LENGTH, 0);
  bool game_running = false;

  while(1) {
    recv(SERVER_SOCKET, BUFFER, MAX_MESSAGE_LENGTH, 0);
    CMD = strtok(BUFFER, ":");
    ARG = strtok(NULL, ":");
    pthread_mutex_lock(&REPLY_MUTEX);
    if(strcmp(CMD, "add") == 0) {
      STATE = START;
      if(!game_running) {
        pthread_t t;
        pthread_create(&t, NULL, (void *(*)(void *))game_loop, NULL);
        game_running = true;
      }
    } else if(strcmp(CMD, "move") == 0) {
      STATE = PROCESS_ENEMY_MOVE;
    } else if(strcmp(CMD, "quit") == 0) {
      STATE = QUIT;
      exit(0);
    } else if(strcmp(CMD, "ping") == 0) {
      sprintf(BUFFER, "pinged: :%s", NICK);
      send(SERVER_SOCKET, BUFFER, MAX_MESSAGE_LENGTH, 0);
    }
    pthread_cond_signal(&REPLY_CONDITION);
    pthread_mutex_unlock(&REPLY_MUTEX);
  }
  return 0;
}

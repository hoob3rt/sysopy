#ifndef CLIENT_UTILS_H
#define CLIENT_UTILS_H

#define MAX_PLAYERS 15
#define MAX_BACKLOG 10
#define MAX_MESSAGE_LENGTH 256

typedef enum { EMPTY, O, X } board_field;

typedef struct board_struct {
  int is_o_turn;
  board_field fields[9];
} board_struct;

board_struct get_empty_board() {
  board_struct board = {1, {EMPTY}};
  return board;
}

int move(board_struct *board, int position) {
  if(position < 0 || position > 9 || board->fields[position] != EMPTY) {
    return 0;
  }
  if(board->is_o_turn) {
    board->fields[position] = O;
  } else {
    board->fields[position] = X;
  }
  board->is_o_turn = !board->is_o_turn;
  return 1;
}

board_field check_columns_for_winner(board_struct *board) {
  for(int x = 0; x < 3; x++) {
    if(board->fields[x] == board->fields[x + 3] &&
       board->fields[x] == board->fields[x + 6] && board->fields[x] != EMPTY)
      return board->fields[x];
  }
  return EMPTY;
}

board_field check_rows_for_winner(board_struct *board) {
  for(int i = 0; i < 3; i++) {
    if(board->fields[3 * i] == board->fields[3 * i + 1] &&
       board->fields[3 * i] == board->fields[3 * i + 2] &&
       board->fields[3 * i] != EMPTY) {
      return board->fields[3 * i];
    }
  }
  return EMPTY;
}

board_field check_diagonal_for_winner(board_struct *board) {
  if(board->fields[0] == board->fields[4] &&
     board->fields[0] == board->fields[8] && board->fields[0] != EMPTY) {
    return board->fields[0];
  }
  if(board->fields[2] == board->fields[4] &&
     board->fields[2] == board->fields[7] && board->fields[2] != EMPTY) {
    return board->fields[2];
  }
  return EMPTY;
}

board_field get_winner(board_struct *board) {
  board_field winner = check_columns_for_winner(board);
  if(winner != EMPTY) {
    return winner;
  }
  winner = check_rows_for_winner(board);
  if(winner != EMPTY) {
    return winner;
  }
  return check_diagonal_for_winner(board);
}

#endif

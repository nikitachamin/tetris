#include "board.h"

#include <stdbool.h>

void clean_board(Board* board) {
  for (int row = 0; row < FIELD_HEIGHT; row++) {
    for (int column = 0; column < FIELD_WIDTH; column++) {
      board->cells[row][column] = 0;
    }
  }
}

static bool is_row_full(const Board* board, int row) {
  bool full = true;

  for (int column = 0; column < FIELD_WIDTH && full; column++) {
    if (board->cells[row][column] == 0) {
      full = false;
    }
  }

  return full;
}

static void shift_rows_down(Board* board, int start_row) {
  for (int row = start_row; row > 0; row--) {
    for (int column = 0; column < FIELD_WIDTH; column++) {
      board->cells[row][column] = board->cells[row - 1][column];
    }
  }

  for (int column = 0; column < FIELD_WIDTH; column++) {
    board->cells[0][column] = 0;
  }
}

int clear_full_lines(Board* board) {
  int cleared_lines = 0;
  int row = FIELD_HEIGHT - 1;

  while (row >= 0) {
    if (is_row_full(board, row)) {
      shift_rows_down(board, row);
      cleared_lines++;
    } else {
      row--;
    }
  }

  return cleared_lines;
}

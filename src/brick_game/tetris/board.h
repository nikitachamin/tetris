#ifndef BOARD_H
#define BOARD_H

#define FIELD_WIDTH 10
#define FIELD_HEIGHT 20

typedef struct {
  int cells[FIELD_HEIGHT][FIELD_WIDTH];
} Board;

void clean_board(Board* board);
int clear_full_lines(Board* board);

#endif  // BOARD_H

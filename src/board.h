#ifndef BOARD_H
#define BOARD_H

typedef struct {
    int cells[FIELD_HEIGHT][FIELD_WIDTH];
} Board;

void clean_board(Board *board);
#endif // BOARD_H

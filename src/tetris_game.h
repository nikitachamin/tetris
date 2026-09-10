
#ifndef TETRIS_GAME_H
#define TETRIS_GAME_H


#define FIELD_WIDTH 10
#define FIELD_HEIGHT 20

#include "board.h"

#include <stdlib.h>
#include <time.h>

srand(time(NULL));

typedef struct {
    int cells[FIELD_HEIGHT][FIELD_WIDTH];
} Board;

int current_piece;
int next_piece;

#endif // TETRIS_GAME_H

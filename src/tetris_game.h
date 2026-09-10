
#ifndef TETRIS_GAME_H
#define TETRIS_GAME_H


#define FIELD_WIDTH 10
#define FIELD_HEIGHT 20

#include "board.h"

#include <stdlib.h>
#include <time.h>

srand(time(NULL));

int level;
int score;
int current_piece;
int next_piece;

typedef enum {
    START,
    SPAWN,
    MOVE,
    SHIFT,
    ATTACH,
    GAME_OVER,
} GameState;

#endif // TETRIS_GAME_H

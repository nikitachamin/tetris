#ifndef FSM_H
#define FSM_H

#include "tetris_game.h"

typedef enum {
    START,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    ROTATE_LEFT,
    ROTATE_RIGHT,
    STOP
} signals;

#endif

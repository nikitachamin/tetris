#ifndef FSM_H
#define FSM_H

#include "game.h"
#include "tetris.h"

void sigact(Game* game, UserAction_t action);
void fsm_tick(Game* game);

#endif  // FSM_H

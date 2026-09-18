#ifndef TETRIS_GAME_H
#define TETRIS_GAME_H

#include <stdbool.h>
#include <time.h>

#include "board.h"
#include "piece.h"

typedef enum {
  STATE_START,
  STATE_SPAWN,
  STATE_MOVE,
  STATE_SHIFT,
  STATE_ATTACH,
  STATE_GAME_OVER,
  STATE_PAUSE
} GameState;

typedef struct {
  Board board;
  Piece current_piece;
  Piece next_piece;
  int level;
  int high_score;
  int score;
  struct timespec last_fall_time;
  GameState state;
} Game;

void init_game(Game* game);
void start_game(Game* g);
bool spawn_piece(Game* g);
int calculate_score(int cleared_lines);
bool fall_timer_expired(Game* g);
void update_high_score(Game* game);
int load_high_score(void);
bool save_high_score(int score);

#endif  // TETRIS_GAME_H

#include "game.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { kInitialDelayMs = 500, kDelayStepMs = 40 };

static const char* high_score_path(void) {
  const char* path = getenv("BRICKGAME_HIGH_SCORE_FILE");

  if (path == NULL || path[0] == '\0') {
    path = "tetris_high_score.dat";
  }

  return path;
}

void init_game(Game* game) {
  memset(game, 0, sizeof(*game));
  game->state = STATE_START;
  game->high_score = load_high_score();
  srand((unsigned int)time(NULL));
}

void start_game(Game* g) {
  g->score = 0;
  g->level = 1;
  g->state = STATE_SPAWN;
  clean_board(&g->board);
  PieceType next_piece_type = (PieceType)(rand() % PIECE_COUNT);
  init_piece(&g->next_piece, next_piece_type);
  timespec_get(&g->last_fall_time, TIME_UTC);
}

bool spawn_piece(Game* g) {
  g->current_piece = g->next_piece;
  PieceType next_piece_type = (PieceType)(rand() % PIECE_COUNT);
  init_piece(&g->next_piece, next_piece_type);

  return can_place_piece(&g->board, &g->current_piece, g->current_piece.x,
                         g->current_piece.y, g->current_piece.rotation);
}

int calculate_score(int cleared_lines) {
  const int score_table[] = {0, 100, 300, 700, 1500};
  int score = 0;

  if (cleared_lines >= 0 && cleared_lines <= 4) {
    score = score_table[cleared_lines];
  }

  return score;
}

bool fall_timer_expired(Game* g) {
  struct timespec now = {0};
  bool expired = false;

  if (timespec_get(&now, TIME_UTC) == TIME_UTC) {
    long long elapsed_ms =
        (long long)(now.tv_sec - g->last_fall_time.tv_sec) * 1000LL +
        (now.tv_nsec - g->last_fall_time.tv_nsec) / 1000000LL;
    int delay = kInitialDelayMs - (g->level - 1) * kDelayStepMs;

    if (elapsed_ms < 0) {
      g->last_fall_time = now;
    } else if (elapsed_ms >= delay) {
      g->last_fall_time = now;
      expired = true;
    }
  }

  return expired;
}

void update_high_score(Game* game) {
  if (game->score > game->high_score) {
    game->high_score = game->score;
    save_high_score(game->high_score);
  }
}

int load_high_score(void) {
  int high_score = 0;
  FILE* file = fopen(high_score_path(), "r");

  if (file != NULL) {
    int stored_score = 0;
    if (fscanf(file, "%d", &stored_score) == 1 && stored_score >= 0) {
      high_score = stored_score;
    }
    fclose(file);
  }

  return high_score;
}

bool save_high_score(int score) {
  bool saved = false;
  FILE* file = fopen(high_score_path(), "w");

  if (file != NULL) {
    bool written = fprintf(file, "%d\n", score) > 0;
    saved = fclose(file) == 0 && written;
  }

  return saved;
}

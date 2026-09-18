#include "tetris.h"

#include "board.h"
#include "fsm.h"
#include "game.h"
#include "piece.h"

typedef struct {
  Game game;
  GameInfo_t info;
  int field[FIELD_HEIGHT][FIELD_WIDTH];
  int next[PIECE_BLOCK_COUNT][PIECE_BLOCK_COUNT];
  int* field_rows[FIELD_HEIGHT];
  int* next_rows[PIECE_BLOCK_COUNT];
  bool initialized;
} TetrisContext;

static TetrisContext* get_context(void) {
  static TetrisContext context;

  if (!context.initialized) {
    init_game(&context.game);
    for (int row = 0; row < FIELD_HEIGHT; row++) {
      context.field_rows[row] = context.field[row];
    }
    for (int row = 0; row < PIECE_BLOCK_COUNT; row++) {
      context.next_rows[row] = context.next[row];
    }
    context.initialized = true;
  }

  return &context;
}

static void copy_board(TetrisContext* context) {
  for (int row = 0; row < FIELD_HEIGHT; row++) {
    for (int column = 0; column < FIELD_WIDTH; column++) {
      context->field[row][column] = context->game.board.cells[row][column];
    }
  }
}

static void clear_next(TetrisContext* context) {
  for (int row = 0; row < PIECE_BLOCK_COUNT; row++) {
    for (int column = 0; column < PIECE_BLOCK_COUNT; column++) {
      context->next[row][column] = 0;
    }
  }
}

static void overlay_piece(TetrisContext* context) {
  const Piece* piece = &context->game.current_piece;

  for (int i = 0; i < PIECE_BLOCK_COUNT; i++) {
    int x = piece->x + kPieceShapes[piece->type][piece->rotation][i].x;
    int y = piece->y + kPieceShapes[piece->type][piece->rotation][i].y;
    if (x >= 0 && x < FIELD_WIDTH && y >= 0 && y < FIELD_HEIGHT) {
      context->field[y][x] = 1;
    }
  }
}

static void build_next(TetrisContext* context) {
  const Piece* piece = &context->game.next_piece;

  for (int i = 0; i < PIECE_BLOCK_COUNT; i++) {
    int x = kPieceShapes[piece->type][piece->rotation][i].x;
    int y = kPieceShapes[piece->type][piece->rotation][i].y;
    if (x >= 0 && x < PIECE_BLOCK_COUNT && y >= 0 && y < PIECE_BLOCK_COUNT) {
      context->next[y][x] = 1;
    }
  }
}

void userInput(UserAction_t action, bool hold) {
  TetrisContext* context = get_context();
  (void)hold;
  sigact(&context->game, action);
}

GameInfo_t updateCurrentState(void) {
  TetrisContext* context = get_context();

  if (context->game.state == STATE_MOVE && fall_timer_expired(&context->game)) {
    fsm_tick(&context->game);
  }

  copy_board(context);
  clear_next(context);
  bool game_active =
      context->game.state == STATE_MOVE || context->game.state == STATE_PAUSE;
  if (game_active) {
    overlay_piece(context);
    build_next(context);
  }

  context->info.field = context->field_rows;
  context->info.next = context->next_rows;
  context->info.score = context->game.score;
  context->info.high_score = context->game.high_score;
  context->info.level = context->game.level;
  context->info.speed = context->game.level;
  context->info.pause = context->game.state == STATE_PAUSE;

  return context->info;
}

#include "fsm.h"

static void on_spawn_state(Game* game) {
  if (spawn_piece(game)) {
    game->state = STATE_MOVE;
  } else {
    game->state = STATE_GAME_OVER;
  }
}

static void on_attach_state(Game* game) {
  bool reached_top = piece_reached_top(&game->current_piece);

  attach_piece(&game->board, &game->current_piece);
  game->score += calculate_score(clear_full_lines(&game->board));
  update_high_score(game);
  game->level = game->score / 600 + 1;
  if (game->level > 10) {
    game->level = 10;
  }

  if (reached_top) {
    game->state = STATE_GAME_OVER;
  } else {
    game->state = STATE_SPAWN;
    on_spawn_state(game);
  }
}

static void on_move_state(Game* game, UserAction_t action) {
  switch (action) {
    case Left:
      move_piece(&game->board, &game->current_piece, -1, 0);
      break;
    case Right:
      move_piece(&game->board, &game->current_piece, 1, 0);
      break;
    case Down:
      while (move_piece(&game->board, &game->current_piece, 0, 1)) {
      }
      game->state = STATE_ATTACH;
      on_attach_state(game);
      break;
    case Action:
      rotate_piece(&game->board, &game->current_piece);
      break;
    case Pause:
      game->state = STATE_PAUSE;
      break;
    default:
      break;
  }
}

static void on_shift_state(Game* game) {
  if (move_piece(&game->board, &game->current_piece, 0, 1)) {
    game->state = STATE_MOVE;
  } else {
    game->state = STATE_ATTACH;
  }
}

void fsm_tick(Game* game) {
  if (game->state == STATE_MOVE) {
    game->state = STATE_SHIFT;
  }
  if (game->state == STATE_SHIFT) {
    on_shift_state(game);
  }
  if (game->state == STATE_ATTACH) {
    on_attach_state(game);
  }
  if (game->state == STATE_SPAWN) {
    on_spawn_state(game);
  }
}

void sigact(Game* game, UserAction_t action) {
  if (action == Terminate) {
    game->state = STATE_GAME_OVER;
  } else {
    switch (game->state) {
      case STATE_START:
      case STATE_GAME_OVER:
        if (action == Start) {
          start_game(game);
          on_spawn_state(game);
        }
        break;
      case STATE_SPAWN:
        on_spawn_state(game);
        break;
      case STATE_MOVE:
        on_move_state(game, action);
        break;
      case STATE_SHIFT:
        on_shift_state(game);
        break;
      case STATE_ATTACH:
        on_attach_state(game);
        break;
      case STATE_PAUSE:
        if (action == Pause) {
          game->state = STATE_MOVE;
          timespec_get(&game->last_fall_time, TIME_UTC);
        }
        break;
    }
  }
}

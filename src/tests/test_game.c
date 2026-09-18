#include <stdio.h>
#include <stdlib.h>

#include "fsm.h"
#include "game.h"
#include "test.h"

static void prepare_active_game(Game* game, PieceType current, PieceType next) {
  clean_board(&game->board);
  init_piece(&game->current_piece, current);
  init_piece(&game->next_piece, next);
  game->score = 0;
  game->high_score = 0;
  game->level = 1;
  game->state = STATE_MOVE;
  timespec_get(&game->last_fall_time, TIME_UTC);
}

START_TEST(test_score_table) {
  ck_assert_int_eq(calculate_score(-1), 0);
  ck_assert_int_eq(calculate_score(0), 0);
  ck_assert_int_eq(calculate_score(1), 100);
  ck_assert_int_eq(calculate_score(2), 300);
  ck_assert_int_eq(calculate_score(3), 700);
  ck_assert_int_eq(calculate_score(4), 1500);
  ck_assert_int_eq(calculate_score(5), 0);
}
END_TEST

START_TEST(test_start_pause_resume_and_terminate) {
  Game game = {0};
  game.state = STATE_START;
  game.high_score = 42;

  sigact(&game, Start);
  ck_assert_int_eq(game.state, STATE_MOVE);
  ck_assert_int_eq(game.level, 1);
  ck_assert_int_eq(game.high_score, 42);

  sigact(&game, Pause);
  ck_assert_int_eq(game.state, STATE_PAUSE);
  sigact(&game, Pause);
  ck_assert_int_eq(game.state, STATE_MOVE);

  sigact(&game, Terminate);
  ck_assert_int_eq(game.state, STATE_GAME_OVER);
  sigact(&game, Start);
  ck_assert_int_eq(game.state, STATE_MOVE);
}
END_TEST

START_TEST(test_horizontal_move_and_rotation) {
  Game game = {0};
  prepare_active_game(&game, PIECE_T, PIECE_I);
  int initial_x = game.current_piece.x;

  sigact(&game, Left);
  ck_assert_int_eq(game.current_piece.x, initial_x - 1);
  sigact(&game, Right);
  ck_assert_int_eq(game.current_piece.x, initial_x);
  sigact(&game, Action);
  ck_assert_int_eq(game.current_piece.rotation, 1);
  sigact(&game, Up);
  ck_assert_int_eq(game.current_piece.rotation, 1);
}
END_TEST

START_TEST(test_tick_moves_piece_one_row) {
  Game game = {0};
  prepare_active_game(&game, PIECE_O, PIECE_I);

  fsm_tick(&game);

  ck_assert_int_eq(game.current_piece.y, 1);
  ck_assert_int_eq(game.state, STATE_MOVE);
}
END_TEST

START_TEST(test_hard_drop_attaches_and_spawns) {
  Game game = {0};
  prepare_active_game(&game, PIECE_O, PIECE_I);

  sigact(&game, Down);

  ck_assert_int_eq(game.state, STATE_MOVE);
  ck_assert_int_eq(game.current_piece.type, PIECE_I);
  ck_assert_int_eq(game.board.cells[FIELD_HEIGHT - 1][4], 1);
  ck_assert_int_eq(game.board.cells[FIELD_HEIGHT - 1][5], 1);
}
END_TEST

START_TEST(test_line_clear_updates_score_and_level) {
  Game game = {0};
  prepare_active_game(&game, PIECE_I, PIECE_O);
  game.score = 500;
  for (int column = 0; column < FIELD_WIDTH; column++) {
    if (column < 3 || column > 6) {
      game.board.cells[FIELD_HEIGHT - 1][column] = 1;
    }
  }

  sigact(&game, Down);

  ck_assert_int_eq(game.score, 600);
  ck_assert_int_eq(game.level, 2);
  ck_assert_int_eq(game.high_score, 600);
}
END_TEST

START_TEST(test_top_lock_ends_game) {
  Game game = {0};
  prepare_active_game(&game, PIECE_O, PIECE_I);
  game.board.cells[2][4] = 1;
  game.board.cells[2][5] = 1;

  fsm_tick(&game);

  ck_assert_int_eq(game.state, STATE_GAME_OVER);
}
END_TEST

START_TEST(test_spawn_collision_ends_game) {
  Game game = {0};
  clean_board(&game.board);
  init_piece(&game.next_piece, PIECE_O);
  game.board.cells[0][4] = 1;
  game.state = STATE_SPAWN;

  fsm_tick(&game);

  ck_assert_int_eq(game.state, STATE_GAME_OVER);
}
END_TEST

START_TEST(test_fall_timer) {
  Game game = {0};
  game.level = 1;
  timespec_get(&game.last_fall_time, TIME_UTC);
  ck_assert(!fall_timer_expired(&game));

  game.last_fall_time.tv_sec -= 1;
  ck_assert(fall_timer_expired(&game));
  ck_assert(!fall_timer_expired(&game));
}
END_TEST

START_TEST(test_high_score_storage) {
  const char* path = getenv("BRICKGAME_HIGH_SCORE_FILE");
  ck_assert_ptr_nonnull(path);
  remove(path);
  ck_assert_int_eq(load_high_score(), 0);
  ck_assert(save_high_score(321));
  ck_assert_int_eq(load_high_score(), 321);

  FILE* file = fopen(path, "w");
  ck_assert_ptr_nonnull(file);
  ck_assert_int_gt(fprintf(file, "invalid\n"), 0);
  ck_assert_int_eq(fclose(file), 0);
  ck_assert_int_eq(load_high_score(), 0);
  remove(path);
}
END_TEST

Suite* game_suite(void) {
  Suite* suite = suite_create("game");
  TCase* test_case = tcase_create("core");

  tcase_add_test(test_case, test_score_table);
  tcase_add_test(test_case, test_start_pause_resume_and_terminate);
  tcase_add_test(test_case, test_horizontal_move_and_rotation);
  tcase_add_test(test_case, test_tick_moves_piece_one_row);
  tcase_add_test(test_case, test_hard_drop_attaches_and_spawns);
  tcase_add_test(test_case, test_line_clear_updates_score_and_level);
  tcase_add_test(test_case, test_top_lock_ends_game);
  tcase_add_test(test_case, test_spawn_collision_ends_game);
  tcase_add_test(test_case, test_fall_timer);
  tcase_add_test(test_case, test_high_score_storage);
  suite_add_tcase(suite, test_case);

  return suite;
}

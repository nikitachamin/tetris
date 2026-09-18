#include "board.h"
#include "piece.h"
#include "test.h"
#include "tetris.h"

static int count_matrix(int** matrix, int rows, int columns) {
  int count = 0;
  for (int row = 0; row < rows; row++) {
    for (int column = 0; column < columns; column++) {
      count += matrix[row][column] != 0;
    }
  }
  return count;
}

START_TEST(test_initial_public_state) {
  GameInfo_t info = updateCurrentState();

  ck_assert_ptr_nonnull(info.field);
  ck_assert_ptr_nonnull(info.next);
  ck_assert_int_eq(count_matrix(info.field, FIELD_HEIGHT, FIELD_WIDTH), 0);
  ck_assert_int_eq(
      count_matrix(info.next, PIECE_BLOCK_COUNT, PIECE_BLOCK_COUNT), 0);
  ck_assert_int_eq(info.score, 0);
  ck_assert_int_eq(info.level, 0);
  ck_assert_int_eq(info.speed, 0);
  ck_assert_int_eq(info.pause, 0);
}
END_TEST

START_TEST(test_start_and_pause_public_state) {
  userInput(Start, false);
  GameInfo_t info = updateCurrentState();
  ck_assert_int_eq(count_matrix(info.field, FIELD_HEIGHT, FIELD_WIDTH), 4);
  ck_assert_int_eq(
      count_matrix(info.next, PIECE_BLOCK_COUNT, PIECE_BLOCK_COUNT), 4);
  ck_assert_int_eq(info.level, 1);
  ck_assert_int_eq(info.speed, 1);

  userInput(Pause, false);
  info = updateCurrentState();
  ck_assert_int_eq(info.pause, 1);
  userInput(Pause, true);
  info = updateCurrentState();
  ck_assert_int_eq(info.pause, 0);
}
END_TEST

START_TEST(test_public_actions_and_terminate) {
  userInput(Start, false);
  userInput(Left, false);
  userInput(Right, false);
  userInput(Action, false);
  userInput(Up, false);
  userInput(Down, true);
  GameInfo_t info = updateCurrentState();
  ck_assert_int_ge(count_matrix(info.field, FIELD_HEIGHT, FIELD_WIDTH), 8);

  userInput(Terminate, false);
  info = updateCurrentState();
  ck_assert_int_eq(
      count_matrix(info.next, PIECE_BLOCK_COUNT, PIECE_BLOCK_COUNT), 0);
}
END_TEST

Suite* api_suite(void) {
  Suite* suite = suite_create("api");
  TCase* test_case = tcase_create("public");

  tcase_add_test(test_case, test_initial_public_state);
  tcase_add_test(test_case, test_start_and_pause_public_state);
  tcase_add_test(test_case, test_public_actions_and_terminate);
  suite_add_tcase(suite, test_case);

  return suite;
}

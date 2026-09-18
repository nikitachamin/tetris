#include "board.h"
#include "test.h"

START_TEST(test_clean_board) {
  Board board;
  for (int row = 0; row < FIELD_HEIGHT; row++) {
    for (int column = 0; column < FIELD_WIDTH; column++) {
      board.cells[row][column] = 1;
    }
  }

  clean_board(&board);

  for (int row = 0; row < FIELD_HEIGHT; row++) {
    for (int column = 0; column < FIELD_WIDTH; column++) {
      ck_assert_int_eq(board.cells[row][column], 0);
    }
  }
}
END_TEST

START_TEST(test_clear_no_lines) {
  Board board;
  clean_board(&board);
  board.cells[FIELD_HEIGHT - 1][0] = 1;

  ck_assert_int_eq(clear_full_lines(&board), 0);
  ck_assert_int_eq(board.cells[FIELD_HEIGHT - 1][0], 1);
}
END_TEST

START_TEST(test_clear_single_line) {
  Board board;
  clean_board(&board);
  for (int column = 0; column < FIELD_WIDTH; column++) {
    board.cells[FIELD_HEIGHT - 1][column] = 1;
  }
  board.cells[FIELD_HEIGHT - 2][3] = 2;

  ck_assert_int_eq(clear_full_lines(&board), 1);
  ck_assert_int_eq(board.cells[FIELD_HEIGHT - 1][3], 2);
}
END_TEST

START_TEST(test_clear_adjacent_lines) {
  Board board;
  clean_board(&board);
  for (int column = 0; column < FIELD_WIDTH; column++) {
    board.cells[FIELD_HEIGHT - 1][column] = 1;
    board.cells[FIELD_HEIGHT - 2][column] = 1;
  }
  board.cells[FIELD_HEIGHT - 3][0] = 7;

  ck_assert_int_eq(clear_full_lines(&board), 2);
  ck_assert_int_eq(board.cells[FIELD_HEIGHT - 1][0], 7);
}
END_TEST

Suite* board_suite(void) {
  Suite* suite = suite_create("board");
  TCase* test_case = tcase_create("core");

  tcase_add_test(test_case, test_clean_board);
  tcase_add_test(test_case, test_clear_no_lines);
  tcase_add_test(test_case, test_clear_single_line);
  tcase_add_test(test_case, test_clear_adjacent_lines);
  suite_add_tcase(suite, test_case);

  return suite;
}

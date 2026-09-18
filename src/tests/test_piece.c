#include "piece.h"
#include "test.h"

static int count_cells(const Board* board) {
  int count = 0;
  for (int row = 0; row < FIELD_HEIGHT; row++) {
    for (int column = 0; column < FIELD_WIDTH; column++) {
      count += board->cells[row][column] != 0;
    }
  }
  return count;
}

START_TEST(test_all_pieces_can_spawn_and_attach) {
  for (int type = PIECE_I; type < PIECE_COUNT; type++) {
    Board board;
    Piece piece;
    clean_board(&board);
    init_piece(&piece, (PieceType)type);

    ck_assert(
        can_place_piece(&board, &piece, piece.x, piece.y, piece.rotation));
    attach_piece(&board, &piece);
    ck_assert_int_eq(count_cells(&board), PIECE_BLOCK_COUNT);
  }
}
END_TEST

START_TEST(test_piece_stops_at_walls_and_floor) {
  Board board;
  Piece piece;
  clean_board(&board);
  init_piece(&piece, PIECE_O);

  while (move_piece(&board, &piece, -1, 0)) {
  }
  int left_x = piece.x;
  ck_assert(!move_piece(&board, &piece, -1, 0));
  ck_assert_int_eq(piece.x, left_x);

  while (move_piece(&board, &piece, 0, 1)) {
  }
  int bottom_y = piece.y;
  ck_assert(!move_piece(&board, &piece, 0, 1));
  ck_assert_int_eq(piece.y, bottom_y);
}
END_TEST

START_TEST(test_rotation_success_and_collision) {
  Board board;
  Piece piece;
  clean_board(&board);
  init_piece(&piece, PIECE_T);

  ck_assert(rotate_piece(&board, &piece));
  ck_assert_int_eq(piece.rotation, 1);

  piece.rotation = 0;
  board.cells[piece.y + 2][piece.x + 1] = 1;
  ck_assert(!rotate_piece(&board, &piece));
  ck_assert_int_eq(piece.rotation, 0);
}
END_TEST

START_TEST(test_collision_with_board) {
  Board board;
  Piece piece;
  clean_board(&board);
  init_piece(&piece, PIECE_O);
  board.cells[2][piece.x + 1] = 1;

  ck_assert(!move_piece(&board, &piece, 0, 1));
  ck_assert_int_eq(piece.y, 0);
}
END_TEST

START_TEST(test_invalid_piece_placement) {
  Board board;
  Piece piece;
  clean_board(&board);
  init_piece(&piece, PIECE_I);

  ck_assert(!can_place_piece(&board, &piece, piece.x, piece.y, -1));
  piece.type = PIECE_COUNT;
  ck_assert(!can_place_piece(&board, &piece, piece.x, piece.y, 0));
}
END_TEST

START_TEST(test_piece_reaches_top) {
  Piece piece;
  init_piece(&piece, PIECE_O);
  ck_assert(piece_reached_top(&piece));
  piece.y = 1;
  ck_assert(!piece_reached_top(&piece));
}
END_TEST

Suite* piece_suite(void) {
  Suite* suite = suite_create("piece");
  TCase* test_case = tcase_create("core");

  tcase_add_test(test_case, test_all_pieces_can_spawn_and_attach);
  tcase_add_test(test_case, test_piece_stops_at_walls_and_floor);
  tcase_add_test(test_case, test_rotation_success_and_collision);
  tcase_add_test(test_case, test_collision_with_board);
  tcase_add_test(test_case, test_invalid_piece_placement);
  tcase_add_test(test_case, test_piece_reaches_top);
  suite_add_tcase(suite, test_case);

  return suite;
}

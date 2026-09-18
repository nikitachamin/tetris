#include "test.h"

int main(void) {
  SRunner* runner = srunner_create(board_suite());
  srunner_add_suite(runner, piece_suite());
  srunner_add_suite(runner, game_suite());
  srunner_add_suite(runner, api_suite());

  srunner_run_all(runner, CK_NORMAL);
  int failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return failed == 0 ? 0 : 1;
}

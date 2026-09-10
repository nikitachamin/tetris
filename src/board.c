void clean_board(Board *board) {
    for (int i = 0; i < FIELD_HEIGHT; i++) {
        for (int j = 0; j < FIELD_WIDTH; j++) {
            board->cells[i][j] = 0;
        }
    }
  }

void start_game() {
    clean_board(&board);
    next_piece = rand() % 7;
}

void spawn_piece() {
    current_piece = next_piece;
    next_piece = rand() % 7;
}

void start_game() {
    score = 0;
    level = 1;
    clean_board(&board);
    next_piece = rand() % 7;
    state = SPAWN;
}

void spawn_piece() {
    current_piece = next_piece;
    next_piece = rand() % 7;
}

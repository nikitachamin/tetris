#include "piece.h"

const Point kPieceShapes[PIECE_COUNT][PIECE_ROTATION_COUNT][PIECE_BLOCK_COUNT] =
    {
        [PIECE_I] =
            {
                {{0, 1}, {1, 1}, {2, 1}, {3, 1}},
                {{2, 0}, {2, 1}, {2, 2}, {2, 3}},
                {{0, 2}, {1, 2}, {2, 2}, {3, 2}},
                {{1, 0}, {1, 1}, {1, 2}, {1, 3}},
            },
        [PIECE_J] =
            {
                {{0, 0}, {0, 1}, {1, 1}, {2, 1}},
                {{1, 0}, {2, 0}, {1, 1}, {1, 2}},
                {{0, 1}, {1, 1}, {2, 1}, {2, 2}},
                {{1, 0}, {1, 1}, {0, 2}, {1, 2}},
            },
        [PIECE_L] =
            {
                {{2, 0}, {0, 1}, {1, 1}, {2, 1}},
                {{1, 0}, {1, 1}, {1, 2}, {2, 2}},
                {{0, 1}, {1, 1}, {2, 1}, {0, 2}},
                {{0, 0}, {1, 0}, {1, 1}, {1, 2}},
            },
        [PIECE_O] =
            {
                {{1, 0}, {2, 0}, {1, 1}, {2, 1}},
                {{1, 0}, {2, 0}, {1, 1}, {2, 1}},
                {{1, 0}, {2, 0}, {1, 1}, {2, 1}},
                {{1, 0}, {2, 0}, {1, 1}, {2, 1}},
            },
        [PIECE_S] =
            {
                {{1, 0}, {2, 0}, {0, 1}, {1, 1}},
                {{1, 0}, {1, 1}, {2, 1}, {2, 2}},
                {{1, 1}, {2, 1}, {0, 2}, {1, 2}},
                {{0, 0}, {0, 1}, {1, 1}, {1, 2}},
            },
        [PIECE_T] =
            {
                {{1, 0}, {0, 1}, {1, 1}, {2, 1}},
                {{1, 0}, {1, 1}, {2, 1}, {1, 2}},
                {{0, 1}, {1, 1}, {2, 1}, {1, 2}},
                {{1, 0}, {0, 1}, {1, 1}, {1, 2}},
            },
        [PIECE_Z] =
            {
                {{0, 0}, {1, 0}, {1, 1}, {2, 1}},
                {{2, 0}, {1, 1}, {2, 1}, {1, 2}},
                {{0, 1}, {1, 1}, {1, 2}, {2, 2}},
                {{1, 0}, {0, 1}, {1, 1}, {0, 2}},
            },
};

void init_piece(Piece* p, PieceType type) {
  p->type = type;
  p->rotation = 0;
  p->x = (FIELD_WIDTH - PIECE_BLOCK_COUNT) / 2;
  p->y = 0;
}

bool can_place_piece(const Board* b, const Piece* p, int candidate_x,
                     int candidate_y, int candidate_rotation) {
  bool can_place = p->type >= PIECE_I && p->type < PIECE_COUNT &&
                   candidate_rotation >= 0 &&
                   candidate_rotation < PIECE_ROTATION_COUNT;

  for (int i = 0; i < PIECE_BLOCK_COUNT && can_place; i++) {
    int x = candidate_x + kPieceShapes[p->type][candidate_rotation][i].x;
    int y = candidate_y + kPieceShapes[p->type][candidate_rotation][i].y;
    if (x < 0 || x >= FIELD_WIDTH || y < 0 || y >= FIELD_HEIGHT ||
        b->cells[y][x] != 0) {
      can_place = false;
    }
  }

  return can_place;
}

bool move_piece(const Board* b, Piece* p, int dx, int dy) {
  int candidate_x = p->x + dx;
  int candidate_y = p->y + dy;
  bool moved = can_place_piece(b, p, candidate_x, candidate_y, p->rotation);

  if (moved) {
    p->x = candidate_x;
    p->y = candidate_y;
  }

  return moved;
}

bool rotate_piece(const Board* b, Piece* p) {
  int candidate_rotation = (p->rotation + 1) % PIECE_ROTATION_COUNT;
  bool rotated = can_place_piece(b, p, p->x, p->y, candidate_rotation);

  if (rotated) {
    p->rotation = candidate_rotation;
  }

  return rotated;
}

void attach_piece(Board* b, const Piece* p) {
  for (int i = 0; i < PIECE_BLOCK_COUNT; i++) {
    int x = p->x + kPieceShapes[p->type][p->rotation][i].x;
    int y = p->y + kPieceShapes[p->type][p->rotation][i].y;
    if (x >= 0 && x < FIELD_WIDTH && y >= 0 && y < FIELD_HEIGHT) {
      b->cells[y][x] = 1;
    }
  }
}

bool piece_reached_top(const Piece* p) {
  bool reached_top = false;

  for (int i = 0; i < PIECE_BLOCK_COUNT && !reached_top; i++) {
    if (p->y + kPieceShapes[p->type][p->rotation][i].y == 0) {
      reached_top = true;
    }
  }

  return reached_top;
}

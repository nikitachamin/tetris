#ifndef PIECE_H
#define PIECE_H

#include <stdbool.h>

#include "board.h"

typedef enum {
  PIECE_I,
  PIECE_J,
  PIECE_L,
  PIECE_O,
  PIECE_S,
  PIECE_T,
  PIECE_Z,
  PIECE_COUNT
} PieceType;

enum { PIECE_ROTATION_COUNT = 4, PIECE_BLOCK_COUNT = 4 };

typedef struct {
  int x;
  int y;
} Point;

typedef struct {
  PieceType type;
  int x;
  int y;
  int rotation;
} Piece;

extern const Point kPieceShapes[PIECE_COUNT][PIECE_ROTATION_COUNT]
                               [PIECE_BLOCK_COUNT];

void init_piece(Piece* p, PieceType type);
bool can_place_piece(const Board* b, const Piece* p, int candidate_x,
                     int candidate_y, int candidate_rotation);
bool move_piece(const Board* b, Piece* p, int dx, int dy);
bool rotate_piece(const Board* b, Piece* p);
void attach_piece(Board* b, const Piece* p);
bool piece_reached_top(const Piece* p);

#endif  // PIECE_H

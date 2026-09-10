#ifndef PIECE_H
#define PIECE_H

typedef enum {
    I,
    J,
    L,
    O,
    S,
    T,
    Z,
} PieceType;

typedef struct {
    PieceType type;
    int x;
    int y;
    int rotation;
} Piece;


#endif // PIECE_H

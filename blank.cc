#include "blank.h"
const int divisor = 2;

Blank::Blank(int f, int r, Board& b): Piece{f, r, 'n', ((f+r) % divisor == 0 ? ' ' : '_'), b} {}

void Blank::findMoves() {}

std::unique_ptr<Piece> Blank::clonePiece(Board& b) {
    return std::make_unique<Blank>(file, rank, b);
}

int Blank::score() {return 0;}

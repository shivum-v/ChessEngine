#ifndef Rook_H
#define Rook_H

#include "piece.h"
#include "board.h"

class Rook: public Piece {
        static const int heatmap[boardsize][boardsize];
    public:
        Rook(int, int, char, Board& b, int = 0);
        ~Rook() = default;
        void findMoves();
        bool canAttackKing();
        void eliminateIllegalMoves();
        std::unique_ptr<Piece> clonePiece(Board&);
        int score();
};

#endif

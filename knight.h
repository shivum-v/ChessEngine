#ifndef Knight_H
#define Knight_H

#include "piece.h"
#include "board.h"

class Knight: public Piece {
        static const int heatmap[boardsize][boardsize];
    public:
        Knight(int, int, char, Board& b, int = 0);
        ~Knight() = default;
        void findMoves();
        bool canAttackKing();
        void eliminateIllegalMoves();
        std::unique_ptr<Piece> clonePiece(Board&);
        int score();
};

#endif

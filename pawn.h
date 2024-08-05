#ifndef Pawn_H
#define Pawn_H

#include "piece.h"
#include "board.h"

class Pawn: public Piece {
        static const int heatmap[boardsize][boardsize];
    public:
        Pawn(int, int, char, Board&, bool = false);
        ~Pawn() = default;
        void findMoves();
        bool canAttackKing();
        void eliminateIllegalMoves();
        std::unique_ptr<Piece> clonePiece(Board&);
        int score();
};

#endif

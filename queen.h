#ifndef Queen_H
#define Queen_H

#include "piece.h"
#include "board.h"

class Queen: public Piece {
        static const int heatmap[boardsize][boardsize];
    public:
        Queen(int, int, char, Board&, int = 0);
        ~Queen() = default;
        void findMoves();
        bool canAttackKing();
        void eliminateIllegalMoves();
        std::unique_ptr<Piece> clonePiece(Board&);
        int score();
};

#endif

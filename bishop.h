#ifndef Bishop_H
#define Bishop_H

#include "piece.h"
#include "board.h"

class Bishop: public Piece {
        static const int heatmap[boardsize][boardsize];
    public:
        Bishop(int, int, char, Board&, int = 0);
        ~Bishop() = default;
        void findMoves();
        bool canAttackKing();
        void eliminateIllegalMoves();
        std::unique_ptr<Piece> clonePiece(Board&);
        int score();
};

#endif

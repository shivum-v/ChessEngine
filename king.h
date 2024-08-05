#ifndef King_H
#define King_H

#include "piece.h"
#include "board.h"

class King: public Piece {
        static const int heatmap[boardsize][boardsize];
    public:
        King(int, int, char, Board& b, int = 0);
        ~King() = default;
        void findMoves();
        bool canAttackKing();
        void eliminateIllegalMoves();
        std::unique_ptr<Piece> clonePiece(Board&);
        int score();
};

#endif

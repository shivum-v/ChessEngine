#ifndef Blank_H
#define Blank_H

#include "piece.h"

class Blank: public Piece {
    public:
        Blank(int, int, Board&);
        ~Blank() = default;
        void findMoves();
        std::unique_ptr<Piece> clonePiece(Board&);
        int score();
};

#endif

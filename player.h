#ifndef Player_H
#define Player_H
#include "board.h"


class Player {
    protected:
        Board& board;
        bool resign = false;
    public:
        Player(Board&);
        virtual void move() = 0;
        bool getResign();
};

#endif

#ifndef _SCOREBOARD_H_
#define _SCOREBOARD_H_
#include "board.h" 
#include "observer.h"
#include <iostream>

class Scoreboard: public Observer {
    Board& board;
    int scorewhite = 0;
    int scoreblack = 0;
    public:
        Scoreboard(Board&);
        ~Scoreboard();
        void notify(bool);
};
#endif

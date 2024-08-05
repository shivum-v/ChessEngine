#include "player.h"
#include "board.h"
#include "piece.h"
#include <vector>
#include <algorithm> 
#include <random> 
#include <chrono> 
#include <cmath>

class Computer : public Player {
    const int level;
    struct moveData {
        int f1, r1, f2, r2;
    };
    public:
        Computer(Board&, int);
        void move();
        float scoreboard(Board&);
        float miniMax(Board&, int, bool, float, float, moveData&);
        void randomMove();
};




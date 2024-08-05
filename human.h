#ifndef Human_H
#define Human_H
#include "player.h"
#include <string>
#include <iostream>

class Human: public Player {
    public:
        Human(Board&);
        void move();
};

#endif


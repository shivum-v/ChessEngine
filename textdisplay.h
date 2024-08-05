#ifndef TDisplay_H
#define TDisplay_H
#include "observer.h"
#include "board.h"
#include <iostream>

class Textdisplay : public Observer {
      Board& board;
      public:
        virtual void notify(bool);
        Textdisplay(Board&);
        ~Textdisplay();
};

#endif


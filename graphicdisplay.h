#ifndef Graph_H
#define Graph_H
#include "observer.h"
#include "window.h"
#include "board.h"
#include <iostream>
#include <map>
#include <X11/xpm.h>

class GraphicDisplay: public Observer {
    int top, bottom, left, right;
    Xwindow w;
    Board& board;
    char curboard[bsize][bsize];
    std::map<char, Pixmap> GreenPixmaps;
    std::map<char, Pixmap> WhitePixmaps;
    struct PieceImage {
        char piece;
        const char* filepath;
    };
    public:
        GraphicDisplay(int, int, int, int, Board&);
        ~GraphicDisplay();
        virtual void notify(bool) override;
        void loadPieceImages();
};

#endif

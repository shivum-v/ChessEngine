#include "textdisplay.h"
using namespace std;

void Textdisplay::notify(bool b) {
    if (!b) {return;}
    cout << endl << endl << endl;
    for(int i = 0; i < bsize; ++i) {
        // Writing out each rank
        cout << bsize - i << ' ';
        for(int j = 0; j < bsize; ++j) {
            board.getPiece(j, i)->printPiece();
        }
        cout << endl;
    }

    // Writing file letters
    cout << endl << "  ";
    for (int i = 0; i < bsize; ++i) {
        cout << char(a + i);
    }
    cout << endl;
}

Textdisplay::Textdisplay(Board& b): board{b} {
    board.attach(this);
} 

Textdisplay::~Textdisplay() {
    board.detach(this);
}



#include "scoreboard.h"
using namespace std;


Scoreboard::Scoreboard(Board& b): board{b} {
    board.attach(this);
} 

Scoreboard::~Scoreboard(){
    board.detach(this);

    // Printing final scoreboard
    cout << "Final Score:" << endl;
    cout << "White: " << scorewhite << endl;
    cout << "Black: " << scoreblack << endl;
} 

void Scoreboard::notify(bool b) {
    if (b) return;
    if (!board.getGameOver()) return;
    if (!board.getbkingcheck() && !board.getwkingcheck() && !board.getResign()) return;
    (board.getTurn() ? ++scoreblack : ++scorewhite);
}


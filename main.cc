#include <iostream>
#include <string>
#include "board.h"
#include <memory>
#include "player.h"
#include "human.h"
#include "computer.h"
#include "textdisplay.h"
#include "scoreboard.h"
#include "graphicdisplay.h"

using namespace std;


void gameHandler(Board& game, Player& p1, Player& p2) {
    string cmd;
    while(true) {
        // If the game is over
        if (game.getGameOver()) {
            game.notifyObservers(false);
            while(cin >> cmd) {
                if (cmd == "continue") {break;}
            }
            game.defaultBoard();
            game.notifyObservers(true);
            return;
        }

        // Game is not over => It is either white or blacks turn
        cout << (game.getTurn() ? "white turn" : "black turn") << endl;
        (game.getTurn() ? p1.move() : p2.move());
    }
   
}

int main() {
    string command;
    Board gameboard;
    Textdisplay td{gameboard};
    GraphicDisplay gd{0, 70, 0, 70, gameboard};
    Scoreboard score{gameboard};
    
    gameboard.notifyObservers(true);

    while(cin >> command) {
        // Enters setup mode (see method in board and project specifications for commands)
        if (command == "setup") {
            gameboard.setupBoard();
        }

        // Enters game mode
        else if (command == "game") {
            string p1;
            string p2;
            cin >> p1 >> p2;
            unique_ptr<Player> player1;
            unique_ptr<Player> player2;

            // Checking string p1
            if (p1 == "human") {
                player1 = make_unique<Human>(gameboard);
            } else if (p1 == "computer1") {
                player1 = make_unique<Computer>(gameboard, 1);
            } else if (p1 == "computer2") {
                player1 = make_unique<Computer>(gameboard, 2);
            } else if (p1 == "computer3") {
                player1 = make_unique<Computer>(gameboard, 3);
            } else if (p1 == "computer4") {
                player1 = make_unique<Computer>(gameboard, 4);
            } else {
                cout << "Invalid game argument" << endl;
                continue;
            }

            // Checking string p2
            if (p2 == "human") {
                player2 = make_unique<Human>(gameboard);
            } else if (p2 == "computer1") {
                player2 = make_unique<Computer>(gameboard, 1);
            } else if (p2 == "computer2") {
                player2 = make_unique<Computer>(gameboard, 2);
            } else if (p2 == "computer3") {
                player2 = make_unique<Computer>(gameboard, 3);
            } else if (p2 == "computer4") {
                player2 = make_unique<Computer>(gameboard, 4);
            } else {
                cout << "Invalid game argument" << endl;
                continue;
            }

            gameHandler(gameboard, *player1, *player2);
        }

        // Quits program
        else if (command == "q") {break;}
    }
}

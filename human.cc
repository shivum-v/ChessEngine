#include "human.h" 
using namespace std;

Human::Human(Board& o): Player{o} {}

void Human::move() {
    string cmd;
    int f1, r1, f2, r2;
    char promotion;
    string p1;
    string p2;
    while(true) {
        cin >> cmd;
        // Resignation
        if (cmd == "resign") {
            board.setGameOver(true);
            board.setResign(true);
            resign = true;
            cout << (board.getTurn() ? "Black" : "White") << " wins!" << endl;
            return;
        }
        
        // Human undo's the last move
        if (cmd == "undo") {
            board.undo(true);
            return;
        }

        // Moving a piece
        if (cmd == "move") {
            cin >> p1 >> p2;
            f1 = p1[0] - 'a';
            r1 = bsize - (p1[1]- '0');
            f2 = p2[0] - 'a';
            r2 = bsize - (p2[1]- '0');
            
            // Piece moving is invalid
            if (!(0 <= f1 && bsize > f1 && 0 <= r1 && bsize > r1 && 0 <= f2 && bsize > f2 && 0 <= r2 && bsize > r2)) {
            cout << "Out of Bounds" << endl;
            continue;
            }

            // Trying to move a piece of the opponent
            else if ((board.getPiece(f1, r1)->getcolour() == 'w' && !board.getTurn()) || (board.getPiece(f1, r1)->getcolour() == 'b' && board.getTurn())) {
            cout << "Not your piece/turn" << endl;
            continue;
            }
            
            // Cannot move the piece (It is not in the list of legalMoves)
            if (!(board.getPiece(f1, r1)->movePiece(f2, r2))) {
                cout << "Invalid Move Command" << endl;
                continue;
            }

            // Checking for promotion
            else if ((board.getPiece(f1, r1)->getPiece() == 'p' || board.getPiece(f1, r1)->getPiece() == 'P') && (!r2 || r2 == bsize - 1)) {
                cin >> promotion;
                // Seeing if promotion piece is invalid or valid
                if (promotion != 'Q' && promotion != 'R' && promotion != 'N' && promotion != 'B' && 
                    promotion != 'q' && promotion != 'r' && promotion != 'n' && promotion != 'b') {
                        cout << "Invalid Promotion - Redo Move" << endl;
                        continue;
                    }
                else {
                    board.makeTurn(f1, r1, f2, r2, promotion);
                    return;
                }
            }

            // Able to make the move
            else {
                board.makeTurn(f1, r1, f2, r2);
                return;
            }
        }
    }
}

#include "bishop.h"
const int divisor = 2;
using namespace std;

// Bishop Heatmap - The higher the number, the more we want the bishop to be on that square
const int Bishop::heatmap[boardsize][boardsize] = {
    {2, 2, 2, 2, 2, 2, 2, 2},
    {3, 3, 3, 3, 3, 3, 3, 3},
    {4, 4, 4, 4, 4, 4, 4, 4},
    {5, 5, 5, 5, 5, 5, 5, 5},
    {2, 4, 6, 8, 8, 6, 4, 2},
    {3, 6, 8, 6, 6, 8, 6, 3},
    {6, 8, 6, 4, 4, 6, 8, 6},
    {6, 6, 3, 2, 2, 3, 6, 6}
};

Bishop::Bishop(int f, int r, char c, Board& b, int moved): Piece{f, r, c, (c == 'w' ? 'B' : 'b'), b, moved} {}


bool Bishop::canAttackKing() { 
    Piece* opposition;
    
    // Up and right movement
    if (boardsize > file + 1 && 0 < rank) {
        for (int i = 1; (file + i < boardsize && rank - i >= 0); ++i) {
            opposition = board.getPiece(file + i, rank - i);
            if (opposition->getPiece() == oppKing) return true;
            if (opposition->getcolour() != 'n') break;
        }
    }
    
    // Down and right movement
    if (boardsize > file + 1 && boardsize > rank + 1) {
        for (int i = 1; (file + i < boardsize && rank + i < boardsize); ++i) {
            opposition = board.getPiece(file + i, rank + i);
            if (opposition->getPiece() == oppKing) return true;
            if (opposition->getcolour() != 'n') break;
        }
    }
    
    // Up and left movement
    if (0 < file && 0 < rank) {
        for (int i = 1; (file - i >= 0 && rank - i >= 0); ++i) {
            opposition = board.getPiece(file - i, rank - i);
            if (opposition->getPiece() == oppKing) return true;
            if (opposition->getcolour() != 'n') break;
        }
    }
    
    // Down and left movement
    if (0 < file && boardsize > rank + 1) {
        for (int i = 1; (file - i >= 0 && rank + i < boardsize); ++i) {
            opposition = board.getPiece(file - i, rank + i);
            if (opposition->getPiece() == oppKing) return true;
            if (opposition->getcolour() != 'n') break;
        }
    }
    return false;
}

void Bishop::eliminateIllegalMoves() {
    if ((board.getTurn() && color == 'b') || (!board.getTurn() && color == 'w')) {
        return;
    }

    // Iterating through legalMoves, if a move is not legal (i.e. the king is still in check), move is removed
    for(auto move = legalMoves.begin(); move != legalMoves.end();) {
        Board tmp{board};
        bool invalid_move = false;
        tmp.movePiece(file, rank, move->file, move->rank);
        for(auto piece : tmp) {
            if (piece->getcolour() == color || piece->getcolour() == 'n') continue;
            if (piece->canAttackKing()) {
                invalid_move = true;
                break;
            }
        }
        if (invalid_move) {
            move = legalMoves.erase(move);
        }
        else ++move;
    }
}


void Bishop::findMoves() {
    legalMoves.clear();
    Piece* opposition;
    
    // Up and right movement
    if (boardsize > file + 1 && 0 < rank) {
        for (int i = 1; (file + i < boardsize && rank - i >= 0); ++i) {
            opposition = board.getPiece(file + i, rank - i);
            char c = opposition->getcolour();
            if (c == color) {
                break;
            } else if (c == 'n') {
                legalMoves.push_back(location{file + i, rank - i});
            } else {
                legalMoves.push_back(location{file + i, rank - i});
                if (opposition->getPiece() == oppKing) {
                    (oppKing == 'K' ? board.setwkingcheck(true) : board.setbkingcheck(true));
                }
                break;
            }
        }
    }
    
    // Down and right movement
    if (boardsize > file + 1 && boardsize > rank + 1) {
        for (int i = 1; (file + i < boardsize && rank + i < boardsize); ++i) {
            opposition = board.getPiece(file + i, rank + i);
            char c = opposition->getcolour();
            if (c == color) {
                break;
            } else if (c == 'n') {
                legalMoves.push_back(location{file + i, rank + i});
            } else {
                legalMoves.push_back(location{file + i, rank + i});
                if (opposition->getPiece() == oppKing) {
                    (oppKing == 'K' ? board.setwkingcheck(true) : board.setbkingcheck(true));
                }
                break;
            }
        }
    }
    
    // Up and left movement
    if (0 < file && 0 < rank) {
        for (int i = 1; (file - i >= 0 && rank - i >= 0); ++i) {
            opposition = board.getPiece(file - i, rank - i);
            char c = opposition->getcolour();
            if (c == color) {
                break;
            } else if (c == 'n') {
                legalMoves.push_back(location{file - i, rank - i});
            } else {
                legalMoves.push_back(location{file - i, rank - i});
                if (opposition->getPiece() == oppKing) {
                    (oppKing == 'K' ? board.setwkingcheck(true) : board.setbkingcheck(true));
                }
                break;
            }
        }
    }
    
    // Down and left movement
    if (0 < file && boardsize > rank + 1) {
        for (int i = 1; (file - i >= 0 && rank + i < boardsize); ++i) {
            opposition = board.getPiece(file - i, rank + i);
            char c = opposition->getcolour();
            if (c == color) {
                break;
            } else if (c == 'n') {
                legalMoves.push_back(location{file - i, rank + i});
            } else {
                legalMoves.push_back(location{file - i, rank + i});
                if (opposition->getPiece() == oppKing) {
                    (oppKing == 'K' ? board.setwkingcheck(true) : board.setbkingcheck(true));
                }
                break;
            }
        }
    }
    eliminateIllegalMoves();
}

std::unique_ptr<Piece> Bishop::clonePiece(Board& b) {
    return std::make_unique<Bishop>(file, rank, color, b, moves);
}

int Bishop::score() {
    // Additional score based off heatmap defined above (Must flip heatmap when accessing black piece scores)
    int pos = (color == 'w' ? heatmap[rank][file] : heatmap[(boardsize - 1) - rank][file]);
    return (color == 'w' ? 30 + pos : -30 - pos);
}


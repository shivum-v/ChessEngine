#include "knight.h"
const int divisor = 2;
using namespace std;

// Knight Heatmap - The higher the number, the more we want the knight to be on that square
const int Knight::heatmap[boardsize][boardsize] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {1, 2, 3, 4, 4, 3, 2, 1},
    {1, 3, 3, 5, 5, 3, 3, 1},
    {1, 5, 8, 10, 10, 8, 5, 1},
    {1, 5, 8, 10, 10, 8, 5, 1},
    {1, 3, 4, 5, 5, 4, 3, 1},
    {1, 2, 3, 4, 4, 3, 2, 1},
    {0, 0, 0, 0, 0, 0, 0, 0}
};

Knight::Knight(int f, int r, char c, Board& b, int moved): Piece{f, r, c, (c == 'w' ? 'N' : 'n'), b, moved} {}


bool Knight::canAttackKing() {
    Piece* opposition;

    // One up movements
    if (boardsize > rank + 1) {
        // Two right
        if (boardsize > file + 2) {
            opposition = board.getPiece(file + 2, rank + 1);
            if (opposition->getPiece() == oppKing) return true;
        }
        // Two left
        if (0 <= file - 2) {
            opposition = board.getPiece(file - 2, rank + 1);
            if (opposition->getPiece() == oppKing) return true;
        }
    }

    // Two up movements
    if (boardsize > rank + 2) {
        // One right movement
        if (boardsize > file + 1) {
            opposition = board.getPiece(file + 1, rank + 2);
            if (opposition->getPiece() == oppKing) return true;
        }
        // One left movement
        if (0 <= file - 1) {
            opposition = board.getPiece(file - 1, rank + 2);
            if (opposition->getPiece() == oppKing) return true;
        }
    }

    // One down movement
    if (0 <= rank - 1) {
        // Two right movement
        if (boardsize > file + 2) {
            opposition = board.getPiece(file + 2, rank - 1);
            if (opposition->getPiece() == oppKing) return true;
        }
        // Two left movement
        if (0 <= file - 2) {
            opposition = board.getPiece(file - 2, rank - 1);
            if (opposition->getPiece() == oppKing) return true;
        }
    }
    
    // Two down movement
    if (0 <= rank - 2) {
        // One right movement
        if (boardsize > file + 1) {
            opposition = board.getPiece(file + 1, rank - 2);
            if (opposition->getPiece() == oppKing) return true;
        }
        // One left movement
        if (0 <= file - 1) {
            opposition = board.getPiece(file - 1, rank - 2);
            if (opposition->getPiece() == oppKing) return true;
        }
    }
    return false;
}

void Knight::eliminateIllegalMoves() {
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

void Knight::findMoves() {
    legalMoves.clear();
    Piece* opposition;

    // One down movements
    if (boardsize > rank + 1) {
        // Two right
        if (boardsize > file + 2) {
            opposition = board.getPiece(file + 2, rank + 1);
            if (opposition->getcolour() != color) legalMoves.push_back(location{file + 2, rank + 1});
            if (opposition->getPiece() == oppKing) {
                (oppKing == 'K' ? board.setwkingcheck(true) : board.setbkingcheck(true));
            }
        }
        // Two left
        if (0 <= file - 2) {
            opposition = board.getPiece(file - 2, rank + 1);
            if (opposition->getcolour() != color) legalMoves.push_back(location{file - 2, rank + 1});
            if (opposition->getPiece() == oppKing) {
                (oppKing == 'K' ? board.setwkingcheck(true) : board.setbkingcheck(true));
            }
        }
    }

    // Two down movements
    if (boardsize > rank + 2) {
        // One right movement
        if (boardsize > file + 1) {
            opposition = board.getPiece(file + 1, rank + 2);
            if (opposition->getcolour() != color) legalMoves.push_back(location{file + 1, rank + 2});
            if (opposition->getPiece() == oppKing) {
                (oppKing == 'K' ? board.setwkingcheck(true) : board.setbkingcheck(true));
            }
        }
        // One left movement
        if (0 <= file - 1) {
            opposition = board.getPiece(file - 1, rank + 2);
            if (opposition->getcolour() != color) legalMoves.push_back(location{file - 1, rank + 2});
            if (opposition->getPiece() == oppKing) {
                (oppKing == 'K' ? board.setwkingcheck(true) : board.setbkingcheck(true));
            }
        }
    }

    // One up movement
    if (0 <= rank - 1) {
        // Two right movement
        if (boardsize > file + 2) {
            opposition = board.getPiece(file + 2, rank - 1);
            if (opposition->getcolour() != color) legalMoves.push_back(location{file + 2, rank - 1});
            if (opposition->getPiece() == oppKing) {
                (oppKing == 'K' ? board.setwkingcheck(true) : board.setbkingcheck(true));
            }
        }
        // Two left movement
        if (0 <= file - 2) {
            opposition = board.getPiece(file - 2, rank - 1);
            if (opposition->getcolour() != color) legalMoves.push_back(location{file - 2, rank - 1});
            if (opposition->getPiece() == oppKing) {
                (oppKing == 'K' ? board.setwkingcheck(true) : board.setbkingcheck(true));
            }
        }
    }
    
    // Two up movement
    if (0 <= rank - 2) {
        // One right movement
        if (boardsize > file + 1) {
            opposition = board.getPiece(file + 1, rank - 2);
            if (opposition->getcolour() != color) legalMoves.push_back(location{file + 1, rank - 2});
            if (opposition->getPiece() == oppKing) {
                (oppKing == 'K' ? board.setwkingcheck(true) : board.setbkingcheck(true));
            }
        }
        // One left movement
        if (0 <= file - 1) {
            opposition = board.getPiece(file - 1, rank - 2);
            if (opposition->getcolour() != color) legalMoves.push_back(location{file - 1, rank - 2});
            if (opposition->getPiece() == oppKing) {
                (oppKing == 'K' ? board.setwkingcheck(true) : board.setbkingcheck(true));
            }
        }
    }
    eliminateIllegalMoves();
}

std::unique_ptr<Piece> Knight::clonePiece(Board& b) {
    return std::make_unique<Knight>(file, rank, color, b, moves);
}

int Knight::score() {
    // Additional score based off heatmap defined above
    int pos = heatmap[rank][file];
    return (color == 'w' ? 30 + pos : -30 - pos);
}

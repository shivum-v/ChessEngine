#include "rook.h"
const int divisor = 2;
using namespace std;

// Rook Heatmap - The higher the number, the more we want the rook to be on that square
const int Rook::heatmap[boardsize][boardsize] = {
    {3, 3, 3, 3, 3, 3, 3, 3},
    {3, 3, 3, 3, 3, 3, 3, 3},
    {2, 2, 2, 2, 2, 2, 2, 2},
    {4, 4, 3, 2, 2, 3, 4, 4},
    {4, 4, 3, 2, 2, 3, 4, 4},
    {5, 5, 5, 5, 5, 5, 5, 5},
    {6, 6, 6, 6, 6, 6, 6, 6},
    {5, 6, 8, 8, 8, 8, 6, 5}
};

Rook::Rook(int f, int r, char c, Board& b, int moved): Piece{f, r, c, (c == 'w' ? 'R' : 'r'), b, moved} {}


bool Rook::canAttackKing() {
    Piece* opposition;
    // Up movement
    if (rank != 0) {
        for (int i = 1; i <= rank; ++i) {
            opposition = board.getPiece(file, rank - i);
            if (opposition->getPiece() == oppKing) return true;
            if (opposition->getcolour() != 'n') break;
        }
    }
    
    // Down movement
    if (rank != (boardsize - 1)) {
        for (int i = 1; i < (boardsize - rank); i++) {
            opposition = board.getPiece(file, rank + i);
            if (opposition->getPiece() == oppKing) return true;
            if (opposition->getcolour() != 'n') break;
        }
    }

    // Left movement
    if (file != 0) {
        for (int i = 1; i <= file; i++) {
            opposition = board.getPiece(file - i, rank);
            if (opposition->getPiece() == oppKing) return true;
            if (opposition->getcolour() != 'n') break;
        }
    }

    // Right movement
    if (file != (boardsize - 1)) {
        for (int i = 1; i < (boardsize - file); i++) {
            opposition = board.getPiece(file + i, rank);
            if (opposition->getPiece() == oppKing) return true;
            if (opposition->getcolour() != 'n') break;
        }
    }
    return false;
}

void Rook::eliminateIllegalMoves() {
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

void Rook::findMoves() {
    legalMoves.clear();
    Piece* opposition;

    // Up movement
    if (rank != 0) {
        for (int i = 1; i <= rank; ++i) {
            opposition = board.getPiece(file, rank - i);
            char c = opposition->getcolour();
            if (c == color) {
                break;
            } else if (c == 'n') {
                legalMoves.push_back(location{file, rank - i});
            } else {
                legalMoves.push_back(location{file, rank - i});
                if (opposition->getPiece() == oppKing) {
                    (oppKing == 'K' ? board.setwkingcheck(true) : board.setbkingcheck(true));
                }
                break;
            }
        }
    }
    
    // Down movement
    if (rank != (boardsize - 1)) {
        for (int i = 1; i < (boardsize - rank); i++) {
            opposition = board.getPiece(file, rank + i);
            char c = opposition->getcolour();
            if (c == color) {
                break;
            } else if (c == 'n') {
                legalMoves.push_back(location{file, rank + i});
            } else {
                legalMoves.push_back(location{file, rank + i});
                if (opposition->getPiece() == oppKing) {
                    (oppKing == 'K' ? board.setwkingcheck(true) : board.setbkingcheck(true));
                }
                break;
            }
        }
    }

    // Left movement
    if (file != 0) {
        for (int i = 1; i <= file; i++) {
            opposition = board.getPiece(file - i, rank);
            char c = opposition->getcolour();
            if (c == color) {
                break;
            } else if (c == 'n') {
                legalMoves.push_back(location{file - i, rank});
            } else {
                legalMoves.push_back(location{file - i, rank});
                if (opposition->getPiece() == oppKing) {
                    (oppKing == 'K' ? board.setwkingcheck(true) : board.setbkingcheck(true));
                }
                break;
            }
        }
    }

    // Right movement
    if (file != (boardsize - 1)) {
        for (int i = 1; i < (boardsize - file); i++) {
            opposition = board.getPiece(file + i, rank);
            char c = opposition->getcolour();
            if (c == color) {
                break;
            } else if (c == 'n') {
                legalMoves.push_back(location{file + i, rank});
            } else {
                legalMoves.push_back(location{file + i, rank});
                if (opposition->getPiece() == oppKing) {
                    (oppKing == 'K' ? board.setwkingcheck(true) : board.setbkingcheck(true));
                }
                break;
            }
        }
    }
    eliminateIllegalMoves();
}

std::unique_ptr<Piece> Rook::clonePiece(Board& b) {
    return std::make_unique<Rook>(file, rank, color, b, moves);
}

int Rook::score() {
    // Additional score based off heatmap defined above (Must flip heatmap when accessing black piece scores)
    int pos = (color == 'w' ? heatmap[rank][file] : heatmap[(boardsize - 1) - rank][file]);
    return (color == 'w' ? 50 + pos : -50 - pos);
}


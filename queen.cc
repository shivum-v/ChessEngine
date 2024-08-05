#include "queen.h"
const int divisor = 2;
using namespace std;

// Queen Heatmap - The higher the number, the more we want the queen to be on that square
const int Queen::heatmap[boardsize][boardsize] = {
    {7, 7, 7, 7, 7, 7, 7, 7},
    {6, 6, 6, 6, 6, 6, 6, 6},
    {5, 5, 5, 5, 5, 5, 5, 5},
    {4, 4, 4, 4, 4, 4, 4, 4},
    {3, 3, 3, 3, 3, 3, 3, 3},
    {2, 2, 2, 2, 2, 2, 2, 2},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {0, 0, 0, 0, 0, 0, 0, 0}
};

Queen::Queen(int f, int r, char c, Board& b, int moved): Piece{f, r, c, (c == 'w' ? 'Q' : 'q'), b, moved} {}

bool Queen::canAttackKing() {
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

void Queen::eliminateIllegalMoves() {
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

void Queen::findMoves() {
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


    // Up and right movement
    if (boardsize > file + 1 && 0 < rank) {
        for (int i = 1; (file + i < boardsize && rank - i >= 0); ++i) { {
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

std::unique_ptr<Piece> Queen::clonePiece(Board& b) {
    return std::make_unique<Queen>(file, rank, color, b, moves);
}

int Queen::score() {
    // Additional score based off heatmap defined above (Must flip heatmap when accessing black piece scores)
    int pos = (color == 'w' ? heatmap[rank][file] : heatmap[(boardsize - 1) - rank][file]);
    return (color == 'w' ? 90 + pos : -90 - pos);
}


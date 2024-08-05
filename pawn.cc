#include "pawn.h"
const int divisor = 2;
using namespace std;

// Pawn Heatmap - The higher the number, the more we want the pawn to be on that square
const int Pawn::heatmap[boardsize][boardsize] = {
    {7, 8, 9, 10, 10, 9, 8, 7},
    {6, 7, 8, 9, 9, 8, 7, 6},
    {5, 6, 7, 8, 8, 7, 6, 5},
    {4, 5, 6, 9, 9, 6, 5, 4},
    {3, 4, 8, 10, 10, 6, 4, 3},
    {2, 3, 4, 5, 5, 4, 3, 2},
    {1, 2, 3, 4, 4, 3, 2, 1},
    {0, 0, 0, 0, 0, 0, 0, 0}
};

Pawn::Pawn(int f, int r, char c, Board& b, bool moved): Piece{f, r, c, (c == 'w' ? 'P' : 'p'), b, moved} {}


bool Pawn::canAttackKing() {
    Piece* opposition;
    if (color == 'w') {
        if (0 <= rank - 1) {
        // One up and to the left movement (take piece)
            if (0 <= file - 1) {
                opposition = board.getPiece(file - 1, rank - 1);
                if (opposition->getPiece() == oppKing) return true;
            } 

            // One up and to the right movement (take piece)
            if (boardsize > file + 1) {
                opposition = board.getPiece(file + 1, rank - 1);
                if (opposition->getPiece() == oppKing) return true;
            }
        }

    } if (color == 'b') {
        if (boardsize > rank + 1) {
            // One down left movement (take piece)
            if (0 <= file - 1) {
                opposition = board.getPiece(file - 1, rank + 1);
                if (opposition->getPiece() == oppKing) return true;
            } 
            
            // One down right movement (take piece)
            if (boardsize > file + 1) {
                opposition = board.getPiece(file + 1, rank + 1);
                if (opposition->getPiece() == oppKing) return true;
            }
        }
    }
    return false;
}

void Pawn::eliminateIllegalMoves() {
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

void Pawn::findMoves() {
    legalMoves.clear();
    Piece* opposition;
    
    // If white
    if (color == 'w') {
        if (0 <= rank - 1) {
            // One up movement
            opposition = board.getPiece(file, rank - 1);
            if (opposition->getcolour() == 'n') {
                legalMoves.push_back(location{file, rank - 1});
                // Two up movement
                if ((0 <= rank - 2) && !moves) {
                    opposition = board.getPiece(file, rank - 2);
                    if (opposition->getcolour() == 'n') legalMoves.push_back(location{file, rank - 2});
                }
            }

            // One up and to the left movement (take piece)
            if (0 <= file - 1) {
                opposition = board.getPiece(file - 1, rank - 1);
                if (opposition->getcolour() == 'b') legalMoves.push_back(location{file - 1, rank - 1});

                if (board.hasmoved() && rank == 3 && board.getPiece(file - 1, rank)->getPiece() == 'p' && board.getPiece(file - 1, rank)->getmoves() == 1 && board.lastMove()[rank-2][file - 1]->getPiece() == 'p') {
                    legalMoves.push_back(location{file - 1, rank - 1});
                }

                if (opposition->getPiece() == oppKing) {
                    (oppKing == 'K' ? board.setwkingcheck(true) : board.setbkingcheck(true));
                }

            } 

            // One up and to the right movement (take piece)
            if (boardsize > file + 1) {
                opposition = board.getPiece(file + 1, rank - 1);
                if (opposition->getcolour() == 'b') legalMoves.push_back(location{file + 1, rank - 1});

                if (board.hasmoved() && rank == 3 && board.getPiece(file + 1, rank)->getPiece() == 'p' && board.getPiece(file + 1, rank)->getmoves() == 1 && (board.lastMove()[rank - 2][file + 1]->getPiece() == 'p')) {
                    legalMoves.push_back(location{file + 1, rank - 1});
                }

                if (opposition->getPiece() == oppKing) {
                    (oppKing == 'K' ? board.setwkingcheck(true) : board.setbkingcheck(true));
                }
            } 
        }
    } if (color == 'b') { // If black
        if (boardsize > rank + 1) {
            // One down movement
            opposition = board.getPiece(file, rank + 1);
            if (opposition->getcolour() == 'n') {
                legalMoves.push_back(location{file, rank + 1});
                // Two down movement
                if ((boardsize > rank + 2) && !moves) {
                    opposition = board.getPiece(file, rank + 2);
                    if (opposition->getcolour() == 'n') legalMoves.push_back(location{file, rank + 2});
                }
            }

            // One down left movement (take piece)
            if (0 <= file - 1) {
                opposition = board.getPiece(file - 1, rank + 1);
                if (opposition->getcolour() == 'w') legalMoves.push_back(location{file - 1, rank + 1});

                if (board.hasmoved() && rank == 4 && board.getPiece(file - 1, rank)->getPiece() == 'P' && board.getPiece(file - 1, rank)->getmoves() == 1 && (board.lastMove()[rank + 2][file - 1]->getPiece() == 'P')) {
                    legalMoves.push_back(location{file - 1, rank + 1});
                }

                if (opposition->getPiece() == oppKing) {
                    (oppKing == 'K' ? board.setwkingcheck(true) : board.setbkingcheck(true));
                }
            } 
            
            // One down right movement (take piece)
            if (boardsize > file + 1) {
                opposition = board.getPiece(file + 1, rank + 1);
                if (opposition->getcolour() == 'w') legalMoves.push_back(location{file + 1, rank + 1});

                if (board.hasmoved() && rank == 4 && board.getPiece(file + 1, rank)->getPiece() == 'P' && board.getPiece(file + 1, rank)->getmoves() == 1 && (board.lastMove()[rank + 2][file + 1]->getPiece() == 'P')) {
                    legalMoves.push_back(location{file + 1, rank + 1});
                }

                if (opposition->getPiece() == oppKing) {
                    (oppKing == 'K' ? board.setwkingcheck(true) : board.setbkingcheck(true));
                }
            } 
        }
    }
    eliminateIllegalMoves();
}

std::unique_ptr<Piece> Pawn::clonePiece(Board& b) {
    return std::make_unique<Pawn>(file, rank, color, b, moves);
}

int Pawn::score() {
    // Additional score based off heatmap defined above (Must flip heatmap when accessing black piece scores)
    int pos = (color == 'w' ? heatmap[rank][file] : heatmap[(boardsize - 1) - rank][file]);
    return (color == 'w' ? 10 + pos : -10 - pos);
}

#include "king.h"

const int kfile = 4;
const int spaces = 2;

// King Heatmap - The higher the number, the more we want the king to be on that
// square
const int King::heatmap[boardsize][boardsize] = {
    {1, 1, 1, 1, 1, 1, 1, 1}, {1, 1, 1, 1, 1, 1, 1, 1},
    {2, 2, 2, 2, 2, 2, 2, 2}, {2, 2, 2, 2, 2, 2, 2, 2},
    {3, 3, 2, 2, 2, 2, 3, 3}, {4, 4, 4, 4, 4, 4, 4, 4},
    {6, 6, 6, 6, 6, 6, 6, 6}, {8, 8, 8, 4, 4, 4, 8, 8}};

King::King(int f, int r, char c, Board &b, int moved)
    : Piece{f, r, c, (c == 'w' ? 'K' : 'k'), b, moved} {}

bool King::canAttackKing() {
    Piece *opposition;

    // Moving up
    if (rank != 0) {
        opposition = board.getPiece(file, rank - 1);
        if (opposition->getPiece() == oppKing)
            return true;

        // Moving up and left
        if (file != 0) {
            opposition = board.getPiece(file - 1, rank - 1);
            if (opposition->getPiece() == oppKing)
                return true;
        }

        // Moving up and right
        if (file + 1 < boardsize) {
            opposition = board.getPiece(file + 1, rank - 1);
            if (opposition->getPiece() == oppKing)
                return true;
        }
    }

    // Moving down
    if (rank + 1 < boardsize) {
        opposition = board.getPiece(file, rank + 1);
        if (opposition->getPiece() == oppKing)
            return true;

        // Moving down and left
        if (file != 0) {
            opposition = board.getPiece(file - 1, rank + 1);
            if (opposition->getPiece() == oppKing)
                return true;
        }

        // Moving down and right
        if (file + 1 < boardsize) {
            opposition = board.getPiece(file + 1, rank + 1);
            if (opposition->getPiece() == oppKing)
                return true;
        }
    }

    // Moving left
    if (file != 0) {
        opposition = board.getPiece(file - 1, rank);
        if (opposition->getPiece() == oppKing)
            return true;
    }

    // Moving right
    if (file + 1 < boardsize) {
        opposition = board.getPiece(file + 1, rank);
        if (opposition->getPiece() == oppKing)
            return true;
    }
    return false;
}

void King::eliminateIllegalMoves() {
    if ((board.getTurn() && color == 'b') ||
        (!board.getTurn() && color == 'w')) {
        return;
    }

    // Iterating through legalMoves, if a move is not legal (i.e. the king is
    // still in check), move is removed
    for (auto move = legalMoves.begin(); move != legalMoves.end();) {
        Board tmp{board};
        bool invalid_move = false;
        tmp.movePiece(file, rank, move->file, move->rank);
        for (auto piece : tmp) {
            if (piece->getcolour() == color || piece->getcolour() == 'n')
                continue;
            if (piece->canAttackKing()) {
                invalid_move = true;
                break;
            }
        }
        if (invalid_move) {
            move = legalMoves.erase(move);
        } else
            ++move;
    }
}

void King::findMoves() {
    legalMoves.clear();
    Piece *opposition;

    // Moving up
    if (rank != 0) {
        opposition = board.getPiece(file, rank - 1);
        if (opposition->getcolour() != color)
            legalMoves.push_back(location{file, rank - 1});

        // Moving up and left
        if (file != 0) {
            opposition = board.getPiece(file - 1, rank - 1);
            if (opposition->getcolour() != color)
                legalMoves.push_back(location{file - 1, rank - 1});
        }

        // Moving up and right
        if (file + 1 < boardsize) {
            opposition = board.getPiece(file + 1, rank - 1);
            if (opposition->getcolour() != color)
                legalMoves.push_back(location{file + 1, rank - 1});
        }
    }

    // Moving down
    if (rank + 1 < boardsize) {
        opposition = board.getPiece(file, rank + 1);
        if (opposition->getcolour() != color)
            legalMoves.push_back(location{file, rank + 1});

        // Moving down and left
        if (file != 0) {
            opposition = board.getPiece(file - 1, rank + 1);
            if (opposition->getcolour() != color)
                legalMoves.push_back(location{file - 1, rank + 1});
        }

        // Moving down and right
        if (file + 1 < boardsize) {
            opposition = board.getPiece(file + 1, rank + 1);
            if (opposition->getcolour() != color)
                legalMoves.push_back(location{file + 1, rank + 1});
        }
    }

    // Moving left
    if (file != 0) {
        opposition = board.getPiece(file - 1, rank);
        if (opposition->getcolour() != color)
            legalMoves.push_back(location{file - 1, rank});
    }

    // Moving right
    if (file + 1 < boardsize) {
        opposition = board.getPiece(file + 1, rank);
        if (opposition->getcolour() != color)
            legalMoves.push_back(location{file + 1, rank});
    }

    for (auto piece : board) {
        if (piece->getcolour() == color || piece->getcolour() == 'n')
            continue;
        if (piece->canAttackKing()) {
            eliminateIllegalMoves();
            return;
        }
    }

    // White castling possibilities
    if (moves == 0 && !board.getwkingcheck() && file == kfile &&
        board.getPiece(file, rank)->getcolour() == 'w') {
        // King side
        if (board.getPiece(boardsize - 1, boardsize - 1)->getPiece() ==
                'R' && // Rook is in bottom right corner
            board.getPiece(boardsize - 1, boardsize - 1)->getmoves() ==
                0 && // Rook has not moved
            board.getPiece(kfile + 1, boardsize - 1)->getcolour() ==
                'n' && // f1 is empty
            board.getPiece(kfile + 2, boardsize - 1)->getcolour() ==
                'n') { // g1 is empty

            bool castle = true;
            for (int i = 1; i <= spaces; ++i) {
                Board tmp{board};
                tmp.movePiece(file, rank, file + i, rank);
                for (auto b : tmp) {
                    if (b->getcolour() == 'b' && b->canAttackKing()) {
                        castle = false;
                        break;
                    }
                }
            }

            if (castle) {
                legalMoves.push_back(location{file + spaces, rank});
            }
        }
        // Queen side
        if (board.getPiece(0, boardsize - 1)->getPiece() ==
                'R' && // Rook is in bottom left corner
            board.getPiece(0, boardsize - 1)->getmoves() ==
                0 && // Rook has not moved
            board.getPiece(kfile - 3, boardsize - 1)->getcolour() ==
                'n' && // b1 is empty
            board.getPiece(kfile - 2, boardsize - 1)->getcolour() ==
                'n' && // c1 is empty
            board.getPiece(kfile - 1, boardsize - 1)->getcolour() ==
                'n') { // d1 is empty
            bool castle = true;
            for (int i = 1; i <= spaces; ++i) {
                Board tmp{board};
                tmp.movePiece(file, rank, file - i, rank);
                for (auto b : tmp) {
                    if (b->getcolour() == 'b' && b->canAttackKing()) {
                        castle = false;
                        break;
                    }
                }
            }

            if (castle) {
                legalMoves.push_back(location{file - spaces, rank});
            }
        }
    }

    // Black castling possibilities
    if (moves == 0 && !board.getbkingcheck() && file == kfile &&
        board.getPiece(file, rank)->getcolour() == 'b') {
        // King side
        if (board.getPiece(boardsize - 1, 0)->getPiece() ==
                'r' && // Rook is in top right corner
            board.getPiece(boardsize - 1, 0)->getmoves() ==
                0 && // Rook has not moved
            board.getPiece(kfile + 1, 0)->getcolour() == 'n' && // f1 is empty
            board.getPiece(kfile + 2, 0)->getcolour() == 'n') { // g1 is empty

            bool castle = true;
            for (int i = 1; i <= spaces; ++i) {
                Board tmp{board};
                tmp.movePiece(file, rank, file + i, rank);
                for (auto b : tmp) {
                    if (b->getcolour() == 'w' && b->canAttackKing()) {
                        castle = false;
                        break;
                    }
                }
            }

            if (castle) {
                legalMoves.push_back(location{file + 2, rank});
            }
        }
        // Queen side
        if (board.getPiece(0, 0)->getPiece() ==
                'r' && // Rook is in top left corner
            board.getPiece(0, 0)->getmoves() == 0 && // Rook has not moved
            board.getPiece(kfile - 3, 0)->getcolour() == 'n' && // b1 is empty
            board.getPiece(kfile - 2, 0)->getcolour() == 'n' && // c1 is empty
            board.getPiece(kfile - 1, 0)->getcolour() == 'n') { // d1 is empty
            bool castle = true;
            for (int i = 1; i <= spaces; ++i) {
                Board tmp{board};
                tmp.movePiece(file, rank, file - i, rank);
                for (auto b : tmp) {
                    if (b->getcolour() == 'w' && b->canAttackKing()) {
                        castle = false;
                        break;
                    }
                }
            }

            if (castle) {
                legalMoves.push_back(location{file - spaces, rank});
            }
        }
    }

    eliminateIllegalMoves();
}

std::unique_ptr<Piece> King::clonePiece(Board &b) {
    return std::make_unique<King>(file, rank, color, b, moves);
}

int King::score() {
    // Additional score based off heatmap defined above (Must flip heatmap when
    // accessing black piece scores)
    int pos = (color == 'w' ? heatmap[rank][file]
                            : heatmap[(boardsize - 1) - rank][file]);
    return (color == 'w' ? 10000 + pos : -10000 - pos);
}

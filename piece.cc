#include "piece.h"
using namespace std;

Piece::Piece(int f, int r, char c, char t, Board& b, int moves): file{f}, rank{r}, color{c}, type{t}, board{b}, moves{moves}, oppKing{(c == 'w' ? 'k' : 'K')} {}

Piece::~Piece() {}

void Piece::printPiece() {
    cout << type;
}


char Piece::getPiece() { return type; }

char Piece::getcolour() {return color; }

bool Piece::location::operator==(const location& o) const {return file == o.file && rank == o.rank;}


bool Piece::movePiece(int f, int r) {
    // Create a new struct location called tmp, store with file and rank parameters
    location tmp{f, r};

    // Iterate through all legal moves
    for (auto move : legalMoves) {
        // If tmp is within legalMoves => It is a legal move => Return true
        if (tmp == move) return true;
    }

    // Tmp is not within legalMoves => It is not a legal move => Return false
    return false;
}

void Piece::setFileRank(int f, int r) {
    file = f;
    rank = r;
    ++moves;
}

bool Piece::canAttackKing() {return false;}

int Piece::getmoves() {return moves;}

bool Piece::emptyMoves() {return legalMoves.empty();}

std::vector<Piece::location>& Piece::getLegalMoves() {return legalMoves;}


int Piece::getFile() {return file;}
int Piece::getRank() {return rank;}

int Piece::score() {return 0;}

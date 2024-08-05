#include "board.h"

using namespace std;

Board::Board() {
    defaultBoard();
}

Board::Board(const Board& o): board{o.cloneBoard(*this)}, wturn{o.wturn}, wkingcheck{o.wkingcheck}, bkingcheck{o.bkingcheck} {}

void Board::defaultBoard() {
    board.clear();
    for(int i = 0; i < bsize; ++i) {
        // Populating each rank with the appropriate Pieces (Pushing back onto tmp)
        vector<unique_ptr<Piece>> tmp;
        for(int j = 0; j < bsize; ++j) {
            if (i == 1 || i == 6) {
                tmp.push_back(make_unique<Pawn>(j, i, (i == 1 ? 'b' : 'w'), *this));
            }
            else if ((i == 0 || i == 7) && (j == 0 || j == 7)) {
                tmp.push_back(make_unique<Rook>(j, i, (i == 0 ? 'b' : 'w'), *this));
            } 
            else if ((i == 0 || i == 7) && (j == 1 || j == 6)) {
                tmp.push_back(make_unique<Knight>(j, i, (i == 0 ? 'b' : 'w'), *this));
            } 
            else if ((i == 0 || i == 7) && (j == 2 || j == 5)) {
                tmp.push_back(make_unique<Bishop>(j, i, (i == 0 ? 'b' : 'w'), *this));
            } 
            else if ((i == 0 || i == 7) && (j == 3)) {
                tmp.push_back(make_unique<Queen>(j, i, (i == 0 ? 'b' : 'w'), *this));
            } 
            else if ((i == 0 || i == 7) && (j == 4)) {
                tmp.push_back(make_unique<King>(j, i, (i == 0 ? 'b' : 'w'), *this));
            } 
            else {tmp.push_back(make_unique<Blank>(j, i, *this));}
        }
        // Push back tmp onto board
        board.push_back(std::move(tmp));
    }

    // Iterate through every piece to find all possible moves
    for (auto piece : *this) {piece->findMoves();}
   
    wturn = true;
    resign = false;
    wkingcheck = false;
    bkingcheck = false;
    gameOver = false;

    while(!moves.empty()) {
        moves.pop();
    }
}



void Board::setupBoard() {
    string command;
    int file, rank;
    string location;
    string colour;
    char piece;
    while(cin >> command) {
        bool check = false;
        int wking = 0;
        int bking = 0;

        // Adding a Piece
        if (command == "+") {
            cin >> piece >> location;
            file = location[0] - 'a';
            rank = bsize - (location[1] - '0');

            // Piece is being added outside of chess boundaries
            if (file < 0 || file >= bsize || rank < 0 || rank >= bsize) {
                cout << "Invalid Location" << endl;
                continue;
            }

            // Setting location on board with appropriate piece (Unique pointer to the correct constructor)
            if (piece == 'k' || piece == 'K') {
                board[rank][file] = make_unique<King>(file, rank, (piece == 'k' ? 'b' : 'w'), *this, 1);
            }
            if (piece == 'q' || piece == 'Q') {
                board[rank][file] = make_unique<Queen>(file, rank, (piece == 'q' ? 'b' : 'w'), *this, 1);
            }
            if (piece == 'b' || piece == 'B') {
                board[rank][file] = make_unique<Bishop>(file, rank, (piece == 'b' ? 'b' : 'w'), *this, 1);
            }
            if (piece == 'n' || piece == 'N') {
                board[rank][file] = make_unique<Knight>(file, rank, (piece == 'n' ? 'b' : 'w'), *this, 1);
            }
            if (piece == 'r' || piece == 'R') {
                board[rank][file] = make_unique<Rook>(file, rank, (piece == 'r' ? 'b' : 'w'), *this, 1);
            }
            if (((piece == 'p' || piece == 'P')) && (rank != 0 && boardsize - 1 != rank)) { // Ensuring you cannot place a pawn on the end ranks
                board[rank][file] = make_unique<Pawn>(file, rank, (piece == 'p' ? 'b' : 'w'), *this, ((piece == 'p' && rank == 1) || (piece == 'P' && rank == 6)) ? 0 : 1);
            }
            notifyObservers(true);
        }

        // Removing a piece
        if (command == "-") {
            cin >> location;
            file = location[0] - 'a';
            rank = bsize - (location[1] - '0');

            // Removal is out of chess boundaries
            if (file < 0 || file >= bsize || rank < 0 || rank >= bsize) {
                cout << "Invalid Location" << endl;
                continue;
            }

            // Setting location to Blank Piece
            board[rank][file] = make_unique<Blank>(file, rank, *this);
            notifyObservers(true);
        }

        // Setting who moves first
        if (command == "=") {
            cin >> colour;
            wturn = (colour == "white" ? true : false);
        }

        // Finished setup
        if (command == "done") {
            // Iterate through every piece to determine number of black and white kings
            for(auto i : *this) {
                if (i->getPiece() == 'k') ++bking;
                if (i->getPiece() == 'K') ++wking;
            }

            // Iterate through every piece to see if one colours king is in check
            for (auto i : *this) {
                if (i->canAttackKing()) {
                    cout << "Cannot start game in check" << endl;
                    check = true;
                    break;
                }
            }

            if (check) continue;
            wturn = !wturn;

            // Iterate through every piece to see if other colours king is in check
            for (auto i : *this) {
                if (i->canAttackKing()) {
                    cout << "Cannot start game in check" << endl;
                    wturn = !wturn;
                    check = true;
                    break;
                }
            }

            if (check) continue;
            wturn = !wturn;

            // Checking if there are exactly one of each king
            if (bking == wking && wking == 1) {
                // There is one of each king => Find all legal moves from the position
                for (auto piece : *this) {piece->findMoves();}
                break;
            }
            else {
                cout << "Please ensure the right number of Kings are present" << endl;
                continue;
            }
        }
    }
}

void Board::gameEnd() {
    gameOver = true;
    
    // Find all legal moves
    for (auto piece : *this) {piece->findMoves();}

    // If legalMoves is empty => There are no legal moves => Game is over (Checkmate or stalemate)
    for (auto piece : *this) {
        if ((piece->getcolour() == 'w' && !wturn) || (piece->getcolour() == 'b' && wturn) || piece->getcolour() == 'n') {
            continue;
        }
            
        if (!(piece->emptyMoves())) {
            gameOver = false;
            break;
        }
    }
}

void Board::movePiece(int f1, int r1, int f2, int r2, char prom) {
    moves.push(cloneBoard(*this));

    // Enpassent Logic Below
    if ((board[r1][f1]->getPiece() == 'p' || board[r1][f1]->getPiece() == 'P') && board[r2][f2]->getcolour() == 'n' && f1 != f2) {
        board[r1][f2] = make_unique<Blank>(r1, f2, *this);
    }

    // Castling Logic Below
    if ((board[r1][f1]->getPiece() == 'k' || board[r1][f1]->getPiece() == 'K') && ((f2 - f1) > 1 || (f2 - f1) < -1)) {
        // Short castling
        if (f2 == 6) {
            board[r2][f2 + 1] = make_unique<Blank>(f2 + 1, r2, *this);
            board[r2][f2 - 1] = make_unique<Rook>(f2 - 1, r2, board[r1][f1]->getcolour(), *this, 1);
        } 
        
        // Long castling
        if (f2 == 2) {
            board[r2][f2 - 2] = make_unique<Blank>(f2 - 2, r2, *this);
            board[r2][f2 + 1] = make_unique<Rook>(f2 + 1, r2, board[r1][f1]->getcolour(), *this, 1);
        }
    }

    // Promotion logic below
    if (prom == 'q' || prom == 'Q') board[r2][f2] = (make_unique<Queen>(f2, r2, (wturn ? 'w' : 'b'), *this, 0));
    else if (prom == 'r' || prom == 'R') board[r2][f2] = (make_unique<Rook>(f2, r2, (wturn ? 'w' : 'b'), *this, 0));
    else if (prom == 'n' || prom == 'N') board[r2][f2] = (make_unique<Knight>(f2, r2, (wturn ? 'w' : 'b'), *this, 0));
    else if (prom == 'b' || prom == 'B') board[r2][f2] = (make_unique<Bishop>(f2, r2, (wturn ? 'w' : 'b'), *this, 0));
    // For computer promotion => Promotes to queen
    else if ((getPiece(f1, r1)->getPiece() == 'p' || getPiece(f1, r1)->getPiece() == 'P') && (!r2 || r2 == bsize - 1)) board[r2][f2] = (make_unique<Queen>(f2, r2, (wturn ? 'w' : 'b'), *this, 0));
    // Making a normal move
    else board[r2][f2] = std::move(board[r1][f1]);
    board[r1][f1] = make_unique<Blank>(f1, r1, *this);
    board[r2][f2]->setFileRank(f2, r2);
    wturn = !wturn;
}



void Board::setwkingcheck(bool b) {wkingcheck = b;}
bool Board::getwkingcheck() {return wkingcheck;}
void Board::setbkingcheck(bool b) {bkingcheck = b;}
bool Board::getbkingcheck() {return bkingcheck;}

bool Board::makeTurn(int f1, int r1, int f2, int r2, char prom) {
    wkingcheck = false;
    bkingcheck = false;
    movePiece(f1, r1, f2, r2, prom);
    gameEnd();
    
    // If game is over => Checkmate or stalemate
    if (gameOver && !observers.empty()) {
        // If either king is in check => Reached checkmate
        if (wkingcheck || bkingcheck) {
            cout << "Checkmate! " << (wkingcheck ? "Black" : "White") << " wins!" << endl;
        } else {
            cout << "Stalemate" << endl;
        }
        notifyObservers(true);
        return true;
    }

    // If a king is in check
    if ((wkingcheck || bkingcheck) && !observers.empty()) {
        cout << (wkingcheck ? "White" : "Black") << " is in Check" << endl;
    }
    notifyObservers(true);
    return true;
    
}

Piece* Board::getPiece(int f, int r) { return board[r][f].get(); }

// Iterator methods
Board::Iterator::Iterator(int f, int r, Board& b): b{b}, file{f}, rank{r} {}  

Board::Iterator& Board::Iterator::operator++() {
    ++file;
    if (file == bsize) {
        file = 0;
        ++rank;
    }
    return *this;
}

Piece* Board::Iterator::operator*() {
    return (b.board[rank][file]).get();
}

Piece* Board::Iterator::operator->() {
    return (b.board[rank][file]).get();
}

bool Board::Iterator::operator!=(const Board::Iterator& o) {
    return !(file == o.file && rank == o.rank);
}

Board::Iterator Board::begin() {
    Board::Iterator answer(0, 0, *this);
    return answer;
}

Board::Iterator Board::end() {
    Board::Iterator answer(0, bsize, *this);
    return answer;
}

std::vector<std::vector<std::unique_ptr<Piece>>> Board::cloneBoard(Board& b) const {
    std::vector<std::vector<std::unique_ptr<Piece>>> clone;

    for (int i = 0; i < boardsize; ++i) {
        std::vector<std::unique_ptr<Piece>> tmp;
        for (int j = 0; j < boardsize; ++j) {
            tmp.push_back(std::move((board[i][j])->clonePiece(b)));
        }
        clone.push_back(std::move(tmp));
    }

    return clone;
}

void Board::undo(bool user) {
    // User is true if called by player
    if (moves.empty()) return;
    
    // Moves are stored in a stack, to undo the last move we pop from the stack
    board = std::move(moves.top());
    moves.pop();
    wturn = !wturn;

    wkingcheck = false;
    bkingcheck = false;
    gameOver = false;
    resign = false;

    // Move has been undone => Find all legal moves from this position
    for (auto piece : *this) {piece->findMoves();}

    if (user) notifyObservers(true);
}


std::vector<std::vector<std::unique_ptr<Piece>>>& Board::lastMove() {
    return moves.top();
}

bool Board::hasmoved() {return !moves.empty();}

bool Board::getGameOver() {return gameOver;}

void Board::setGameOver(bool b) {gameOver = b;}

bool Board::getTurn() {return wturn;}

bool Board::getResign() {return resign;}
void Board::setResign(bool b) {resign = b;}

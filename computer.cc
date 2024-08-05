#include "computer.h"
using namespace std;

Computer::Computer(Board& b, int level): Player{b}, level{level} {}

void Computer::move() {
    string cmd;
    while (cin >> cmd) {
        // User makes computer resign
        if (cmd == "resign") {
            board.setGameOver(true);
            board.setResign(true);
            resign = true;
            cout << (board.getTurn() ? "Black" : "White") << " wins!" << endl;
            return;
        }

        // User undos last computer move
        if (cmd == "undo") {
            // Calling with parameter true since player is typing in move
            board.undo(true);
            return;
        }

        // User tells computer to move piece
        if (cmd == "move") {
            break;
        }
    }

    // Level 1 compueter => Random move generated
    if (level == 1) {
        randomMove();
        return;
    }

    Computer::moveData bestMove;
    Board tmp{board};

    // Dependent on computer level, call miniMax to determine best move, store in bestMove
    if (level == 2) miniMax(tmp, 1, board.getTurn(), -500000000, 500000000, bestMove);
    if (level == 3) miniMax(tmp, 2, board.getTurn(), -500000000, 500000000, bestMove);
    if (level == 4) miniMax(tmp, 3, board.getTurn(), -500000000, 500000000, bestMove);

    // Make the best move according to the miniMax algorithm and its depth (bestMove)
    board.makeTurn(bestMove.f1, bestMove.r1, bestMove.f2, bestMove.r2);
}

void Computer::randomMove() {
    // Algorithm to determine a move at random
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine rng(seed);
    vector<Piece*> pieces; 
    char col = (board.getTurn() ? 'w' : 'b');

    // collect all pieces with legal moves
    for (auto piece : board) {
        if (piece->getcolour() != col) continue;
        if (piece->emptyMoves()) continue;
        pieces.push_back(piece);
    }

    // shuffle the list of legal moves
    for (int i = 0; i < 3; i++) {
        std::shuffle(pieces.begin(), pieces.end(), rng);
    }
    Piece* piece = pieces[0];

    // shuffle the moves that the first piece can do
    vector<Piece::location> moves = piece->getLegalMoves();
    for (int i = 0; i < 3; i++) {
        std::shuffle(moves.begin(), moves.end(), rng);
    }

    // Make the random move
    board.makeTurn(piece->getFile(), piece->getRank(), moves[0].file, moves[0].rank);
}

float Computer::scoreboard(Board& b) {
    // Determining the score of the board (Add on score for if a king is in check or checkmate)
    float score = 0;
    for(auto piece : b) {score += piece->score();}
    if (b.getwkingcheck()) score -= 1000000;
    if (b.getbkingcheck()) score += 1000000;
    b.gameEnd();
    if (b.getGameOver() && b.getwkingcheck()) score -= 50000000;
    if (b.getGameOver() && b.getbkingcheck()) score += 50000000;
    return score;
}

float Computer::miniMax(Board& tmp, int depth, bool isWhite, float alpha, float beta, moveData& bestmove) {
    // Method to determine computer move
    // => Looks at possible moves from each position, and returns the best move available
    // => Will look depth number of moves ahead
    // => The more skilled the computer, the more in depth the tree will go (More accurate calculations for best move)
    // => As depth increases, runtime exponentially increases

    // initialize the best score to be the worst possible (this will force the computer to make any move) i.e cannot make no move
    float bestScore = isWhite ? -2147483647.0 : 2147483647.0;

    // set current score to 0 for now...
    float score = 0;

    // loop over all pieces and add the ones that match the colour of the player's turn 
    vector<Piece*> pieces; 

    // we score the board if we do not want to look further or if the game has ended
    if (depth == 0 || tmp.getGameOver()) {
        return scoreboard(tmp);
    }

    // for everypiece that matches the colour the computer is playing for, find its moves and add it to the piece vector if it has
    // a move to play
    for(auto piece : tmp) {
        if ((isWhite && piece->getcolour() != 'w') || (!isWhite && piece->getcolour() != 'b')) continue;
        piece->findMoves();
        if (piece->emptyMoves()) continue;
        pieces.push_back(piece);
    }

    // if we have no move to play, we return with a valuation that does not favour the current player
    if (pieces.empty()) {
        return (isWhite ? -50000.00 : 50000.00);
    }
    
    // we sort the pieces vector in order of score so that we look at the pieces with the highest score first for the best move (makes alpha beta pruning more efficient)
    std::sort(pieces.begin(), pieces.end(), [&tmp](Piece* a, Piece* b) { return tmp.getTurn() ? (a->score() > b->score()) : (a->score() < b->score()); });

    // initialize bestmove to the first move that the highest scoring piece can make
    if (depth == level - 1) bestmove = {pieces[0]->getFile(), pieces[0]->getRank(), pieces[0]->getLegalMoves()[0].file, pieces[0]->getLegalMoves()[0].rank};

    // for every piece, we will play out their move on a temporary board and recusrively call minimax
    for (auto piece : pieces) {
        vector<Piece::location> moves = piece->getLegalMoves();

        // we sort moves in order of captures first
        std::sort(moves.begin(), moves.end(), [&tmp](const Piece::location& a, const Piece::location& b) {
        bool aIsCapture = tmp.getPiece(a.file, a.rank)->getcolour() != 'n';
        bool bIsCapture = tmp.getPiece(b.file, b.rank)->getcolour() != 'n';
        return aIsCapture > bIsCapture;
    });
        // create a temporary board
        Board newb{tmp};
        for (auto move : moves) {
            // do all moves that piece can do on this temporary board and then let score be the miniMax of the recursive call
            newb.movePiece(piece->getFile(), piece->getRank(), move.file, move.rank);
            score = miniMax(newb, depth - 1, !isWhite, alpha, beta, bestmove);

            // if the player is white, we want to maximize score and copy the move that generated the best score to bestmove, the opposite is true for black
            if (isWhite) {
                if (score >= bestScore) {
                    bestScore = score;
                    if (depth == level - 1) bestmove = {piece->getFile(), piece->getRank(), move.file, move.rank};
                    
                }
                // update alpha for alpha beta pruning
                alpha = max(alpha, bestScore);
            } else {
                if (score <= bestScore) {
                    bestScore = score;
                    if (depth == level - 1) bestmove = {piece->getFile(), piece->getRank(), move.file, move.rank};
                }
                // update beta for alpha beta pruning
                beta = min(beta, bestScore);
            }
            // undo the move that was played
            newb.undo(false);
            // if the move we are currently evaluating is worse than a move we have already seen, we do not need to search further
            if (beta <= alpha) {
                break;
            }
        }
    }

    // return score up the search tree
    return bestScore;

}




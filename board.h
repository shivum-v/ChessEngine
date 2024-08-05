#ifndef Board_H
#define Board_H
#include <vector>
#include <string>
#include <stack>
#include "subject.h"
#include "piece.h"
#include "blank.h"
#include "pawn.h"
#include "rook.h"
#include "knight.h"
#include "bishop.h"
#include "queen.h"
#include "king.h"
#include <memory>
#include <utility>

const int bsize = 8;
const char a = 'a';


class Board : public Subject {
    std::vector<std::vector<std::unique_ptr<Piece>>> board;
    std::stack<std::vector<std::vector<std::unique_ptr<Piece>>>> moves;
    bool wturn = true;
    bool wkingcheck = false;
    bool bkingcheck = false;
    bool gameOver = false;
    bool resign = false;
    public:
        Board();
        Board(const Board&);
        void defaultBoard();
        void setupBoard();
        void movePiece(int, int, int, int, char = '*');
        bool makeTurn(int, int, int, int, char = '*');
        void setwkingcheck(bool);
        bool getwkingcheck();
        void setbkingcheck(bool);
        bool getbkingcheck();
        bool getGameOver();
        void setGameOver(bool);
        bool getTurn();
        bool getResign();
        void setResign(bool b);
        class Iterator {
            friend class Board;
            Board& b;
            int file, rank;
            Iterator(int, int, Board&);
            public:
                Iterator& operator++();
                Piece* operator*();
                Piece* operator->();
                bool operator!=(const Iterator&);
        };
        Iterator begin();
        Iterator end();
        Piece* getPiece(int, int);
        std::vector<std::vector<std::unique_ptr<Piece>>> cloneBoard(Board&) const;
        std::vector<std::vector<std::unique_ptr<Piece>>>& lastMove();
        void undo(bool);
        bool hasmoved();
        void gameEnd();
};


#endif

#ifndef Piece_H
#define Piece_H
#include <iostream>
#include <vector>
#include <memory>


const int boardsize = 8;
class Board;
class Piece {
    public:
        struct location {
            int file;
            int rank;
            bool operator==(const location&) const;
        };
    protected:
        int file, rank;
        char color; 
        char type;
        Board& board;
        int moves = 0;
        std::vector<location> legalMoves;
        char oppKing;
    public:
        Piece(int, int, char, char, Board&, int = 0);
        virtual ~Piece();
        virtual void printPiece();
        bool movePiece(int, int);
        char getPiece();
        char getcolour();
        void setFileRank(int, int);
        virtual void findMoves() = 0;
        virtual std::unique_ptr<Piece> clonePiece(Board&) = 0;
        virtual bool canAttackKing();
        int getFile();
        int getRank();
        bool emptyMoves();
        std::vector<location>& getLegalMoves();
        int getmoves();
        virtual int score();
};

#endif

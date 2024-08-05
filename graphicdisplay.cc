#include "graphicdisplay.h"
using namespace std;

GraphicDisplay::GraphicDisplay(int t, int b, int l, int r, Board& board): w{(r-l)*10, (b-t)*10}, board{board} {
    board.attach(this);
    loadPieceImages();

    // Drawing the green and white tiles
    for(int i = 0; i < bsize; ++i) {
        for(int j = 0; j < bsize; ++j) {
            curboard[j][i] = board.getPiece(j, i)->getPiece();
            int color = ((i + j) % 2 == 0) ? w.White : w.Green;
            w.fillRectangle(j * 80, i * 80, 80, 80, color);
            if (board.getPiece(j, i)->getcolour() == 'n') continue;
            // Location of the piece depends on which Pixmaps map is called
            Pixmap pixmap = color == w.Green ? GreenPixmaps[board.getPiece(j, i)->getPiece()] : WhitePixmaps[board.getPiece(j, i)->getPiece()];
            if (pixmap) {
                w.drawPixmap(j * 80, i * 80, pixmap, 80, 80);
            }
        }
    } 

    // Drawing files and ranks
    for (int i = 0; i < bsize; ++i) {
        // Files
        string file;
        file += ('A' + i);
        w.drawString((i + 0.5) * 80, bsize * 80 + 20, file);

        // Ranks
        string rank = to_string(bsize - i);
        w.drawString(bsize * 80 + 20, (i + 0.55) * 80, rank);
    }
}

GraphicDisplay::~GraphicDisplay() {
    board.detach(this);

    // Iterating through both Pixmaps
    // Calling XFreePixmap, a method in X11 library, to free each Pixmap
    for (auto& pm : GreenPixmaps) {
        XFreePixmap(w.getDisplay(), pm.second);
    }
    for (auto& pm : WhitePixmaps) {
        XFreePixmap(w.getDisplay(), pm.second);
    }
}

void GraphicDisplay::loadPieceImages() {
    // Array of PieceImages with green tile background
    PieceImage greenimages[] = {
        {'P', "GWhitePawn.xpm"},
        {'R', "GWhiteRook.xpm"},
        {'B', "GWhiteBishop.xpm"},
        {'N', "GWhiteKnight.xpm"},
        {'Q', "GWhiteQueen.xpm"},
        {'K', "GWhiteKing.xpm"},
        {'p', "GBlackPawn.xpm"},
        {'r', "GBlackRook.xpm"},
        {'b', "GBlackBishop.xpm"},
        {'n', "GBlackKnight.xpm"},
        {'q', "GBlackQueen.xpm"},
        {'k', "GBlackKing.xpm"}
    };

    // Array of PieceImages with white tile background
    PieceImage whiteimages[] {
        {'P', "WWhitePawn.xpm"},
        {'R', "WWhiteRook.xpm"},
        {'B', "WWhiteBishop.xpm"},
        {'N', "WWhiteKnight.xpm"},
        {'Q', "WWhiteQueen.xpm"},
        {'K', "WWhiteKing.xpm"},
        {'p', "WBlackPawn.xpm"},
        {'r', "WBlackRook.xpm"},
        {'b', "WBlackBishop.xpm"},
        {'n', "WBlackKnight.xpm"},
        {'q', "WBlackQueen.xpm"},
        {'k', "WBlackKing.xpm"}
    };

    // Iterates through every element in array, and converts to Pixmap
    for (const auto& img : greenimages) {
        Pixmap pixmap;
        if (XpmReadFileToPixmap(w.getDisplay(), w.getWindow(), img.filepath, &pixmap, NULL, NULL) == XpmSuccess) {
            GreenPixmaps[img.piece] = pixmap;
            cout << "Uploaded Green " << img.piece << endl;
        }
    }

    for (const auto& img : whiteimages) {
        Pixmap pixmap;
        if (XpmReadFileToPixmap(w.getDisplay(), w.getWindow(), img.filepath, &pixmap, NULL, NULL) == XpmSuccess) {
            WhitePixmaps[img.piece] = pixmap;
            cout << "Uploaded White " << img.piece << endl;
        }
    }
}

void GraphicDisplay::notify(bool b) {
    if (!b) return;

    // Drawing the green and white tiles
    for (int i = 0; i < bsize; ++i) {
        for (int j = 0; j < bsize; ++j) {
            if (curboard[j][i] != board.getPiece(j, i)->getPiece()) {
                curboard[j][i] = board.getPiece(j, i)->getPiece();

                int color = ((i + j) % 2 == 0) ? w.White : w.Green;
                w.fillRectangle(j * 80, i * 80, 80, 80, color);

                if (board.getPiece(j, i)->getcolour() != 'n') {
                    // Location of the piece depends on which Pixmaps map is called
                    Pixmap pixmap = color == w.Green ? GreenPixmaps[board.getPiece(j, i)->getPiece()] : WhitePixmaps[board.getPiece(j, i)->getPiece()];
                    if (pixmap) {
                        w.drawPixmap(j * 80, i * 80, pixmap, 80, 80);
                    }
                }
            }
        }
    }
}


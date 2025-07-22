#ifndef CHESS_H
#define CHESS_H
// Just ensure the class is defined only once

#include <cstddef>
#include "color.h"
#include "chess.h"
#include "chesstype.h"
#include "coordinate.h"
#include "board.h"
#include "square.h"
// The includes

class Chess {
    Color color;
    ChessType type;
    Square *theSquare;
    public:
        Color getColor() const; // Get the color

        ChessType getType() const; // Get the type

        Square *getSquare() const; // Get its Square

        virtual bool isValidMove(const Board &theBoard, Coordinate begin, 
                                 Coordinate end) const = 0;
            // Check if this is a valid move

        virtual vector<Coordinate> validMoves (const Board &theBoard) const = 0;
            // Give all the valid move options
};

#endif


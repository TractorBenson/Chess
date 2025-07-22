#ifndef CHESS_H
#define CHESS_H
// Just ensure the class is defined only once

#include <cstddef>
#include "enum/color.h"
#include "chess/chess.h"
#include "enum/chesstype.h"
#include "struct/coordinate.h"
#include "board/board.h"
#include "square/square.h"
#include "struct/coordinate.h"
// The includes

using namespace std;
class Chess {
    Color color;
    ChessType type;
    Square *theSquare;
    protected:
        Coordinate getCoordinate() const;
    public:

        Chess() = default;
        Chess(Color color, ChessType type, Square *theSquare);
        
        Color getColor() const; // Get the color

        ChessType getType() const; // Get the type

        Square *getSquare() const; // Get its Square

        virtual bool isValidMove(Board &theBoard, Coordinate begin, 
                                 Coordinate end) const = 0;
            // Check if this is a valid move

        virtual vector<Coordinate> validMoves (const Board &theBoard) const = 0;
            // Give all the valid move options
        
        virtual void updateMoved() = 0; // Make the isMoved true

        virtual void update() = 0; // Make canBeEnPassant false
};

#endif


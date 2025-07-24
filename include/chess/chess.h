#ifndef CHESS_H
#define CHESS_H
// Just ensure the class is defined only once

#include <cstddef>
#include "enum/color.h"
#include "enum/chesstype.h"
#include "struct/coordinate.h"
#include "square/square.h"
#include "board/board.h"
// The includes

using namespace std;

class Board;

class Chess {
    Color color;
    ChessType type;
    Square *theSquare;
    protected:
        bool isMoved;
    public:
        Coordinate getCoordinate() const;

        Chess() = default;
        Chess(Color color, ChessType type, Square *theSquare, bool isMoved);
        
        Color getColor() const; // Get the color

        ChessType getType() const; // Get the type

        Square *getSquare() const; // Get its Square

        void setSquare(Square *sqr);

        virtual bool isValidMove(Board &theBoard, Coordinate begin, 
                                 Coordinate end) = 0;
            // Check if this is a valid move

        virtual vector<Coordinate> validMoves (Board &theBoard) = 0;
            // Give all the valid move options
        
        virtual void updateMoved() = 0; // Make the isMoved true

        virtual void update() = 0; // Make canBeEnPassant false

        virtual bool checkCanBeEnPassant() const;
        
        bool getIsMovedStatus() const;
};

#endif


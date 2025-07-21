#ifndef CHESS_H
#define CHESS_H
// Just ensure the class is defined only once

#include <cstddef>;
#include "color.h"
#include "chess.h"
#include "chesstype.h"
#include "coordinate.h"
#include "board.h"
// The includes

class Chess {
    Color color;
    ChessType type;

    public:
        virtual bool isValidMove(const Board &theBoard, Coordinate begin, 
                                 Coordinate end) const = 0;
            // Check if this is a valid move

        virtual bool isCheck (const Board &theBoard, Coordinate posi) 
                        const = 0;
            // Check if the chess can check opponent's king
        
        virtual bool isCheckmate (const Board &theBoard, Coordinate posi) 
                        const = 0;
};

#endif


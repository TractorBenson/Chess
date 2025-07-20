#ifndef CHESS_H
#define CHESS_H
// Just ensure the class is defined only once

import <cstddef>;
import <color.h>;
import <chess.h>;
import <coordinate.h>;

class Chess {
    Color color;
    ChessType type;

    public:
        virtual bool isValidMove(const &theBoard, Coordinate begin, 
                                 Coordinate end) const = 0;
            // Check if this is a valid move

        virtual bool isCheck (const Board &theBoard, Coordinate posi) 
                        const = 0;
            // Check if the chess can check opponent's king
        
        virtual bool isCheckmate (const Board &theBoard, Coordinate posi) 
                        const  = 0;
};

#endif


#ifndef QUEEN_H
#define QUEEN_H

#include "chess/chess.h"

class Queen : public Chess {
    public:
        bool isValidMove(const Board &theBoard, Coordinate begin, 
                                    Coordinate end) const override;
            // The override declaration
        
        vector<Coordinate> validMoves (const Board &theBoard) const override;
            // Give all the valid move options
};

#endif
#ifndef ROOK_H
#define ROOK_H

#include "chess.h"

class Rook : public Chess {
    bool isMoved;
    public:
        bool isValidMove(const Board &theBoard, Coordinate begin, 
                                    Coordinate end) const override;
            // The override declaration
        
        vector<Coordinate> validMoves (const Board &theBoard) const override;
            // Give all the valid move options
};

#endif
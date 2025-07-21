#ifndef KING_H
#define KING_H

#include "chess.h"

class King : public Chess {
    bool isMoved;
    public:
        bool isValidMove(const Board &theBoard, Coordinate begin, 
                                    Coordinate end) const override;
            // The override declaration
        
        vector<Coordinate> validMoves (const Board &theBoard) const override;
            // Give all the valid move options

        bool isChecked (const Board &theBoard) const;
            // The override declaration
    
        bool isCheckmated (const Board &theBoard) const;
            // The override declaration
};

#endif
#ifndef KNIGHT_H
#define KNIGHT_H

#include "chess/chess.h"

class Knight : public Chess {
    public:
        bool isValidMove(const Board &theBoard, Coordinate begin, 
                                    Coordinate end) const override;
            // The override declaration
        
        vector<Coordinate> validMoves (const Board &theBoard) const override;
            // Give all the valid move options
        
        void update() override;

        void updateMoved() override;
};

#endif
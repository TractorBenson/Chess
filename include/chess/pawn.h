#ifndef PAWN_H
#define PAWN_H

#include "chess/chess.h"

class Pawn : public Chess {
    bool isMoved;
    bool canBeEnPassant;
    public:
        bool isValidMove(const Board &theBoard, Coordinate begin, 
                                    Coordinate end) const override;
            // The override declaration
        
        vector<Coordinate> validMoves (const Board &theBoard) const override;
            // Give all the valid move options

        void setEnPassant(); // Set canBeEnPassant to true
        
        void update() override; // Update the canBeEnPassant to false

        void updateMoved() override; // Set isMoved to true
};

#endif


#ifndef PAWN_H
#define PAWN_H

#include <algorithm>
#include "chess/chess.h"

using namespace std;

class Pawn : public Chess {
    bool isMoved;
    bool canBeEnPassant;
    public:
        Pawn(Color color, Square *theSquare, bool isMoved, 
             bool canBeEnPassant);
        bool isValidMove(Board &theBoard, Coordinate begin, 
                         Coordinate end) override;
            // The override declaration
        
        vector<Coordinate> validMoves (Board &theBoard) override;
            // Give all the valid move options

        void setEnPassant(); // Set canBeEnPassant to true
        
        void update() override; // Update the canBeEnPassant to false

        void updateMoved() override; // Set isMoved to true

        bool checkCanBeEnPassant() const override;
};

#endif


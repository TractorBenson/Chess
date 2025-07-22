#ifndef PAWN_H
#define PAWN_H

#include <algorithm>
#include "chess/chess.h"
#include "chess/king.h"
#include "board/board.h"

using namespace std;

class Pawn : public Chess {
    bool isMoved;
    bool canBeEnPassant;
    public:
        Pawn(Color color, Square *theSquare, bool isMoved, 
             bool canBeEnPassant);
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


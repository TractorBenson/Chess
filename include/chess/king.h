#ifndef KING_H
#define KING_H

#include <algorithm>
#include "chess/chess.h"

using namespace std;


class King : public Chess {
    public:
        King(Color color, Square *theSquare, bool isMoved);
        bool isValidMove(Board &theBoard, Coordinate begin, 
                         Coordinate end) override;
            // The override declaration
        
        vector<Coordinate> validMoves (Board &theBoard) override;
            // Give all the valid move options

        int isChecked (const Board &theBoard) const;
            // Determine whether the king is checked, and return the number 
            //   of enemy chesses that are checking it.
    
        bool isCheckmated (Board &theBoard);
            // Determine if the king is checkmated
        
        void update() override;

        void updateMoved() override;
};

#endif
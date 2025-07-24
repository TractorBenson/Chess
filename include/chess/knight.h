#ifndef KNIGHT_H
#define KNIGHT_H

#include "chess/chess.h"

using namespace std;


class Knight : public Chess {
    public:
        Knight(Color color, Square *theSquare, bool isMoved);
        bool isValidMove(Board &theBoard, Coordinate begin, 
                         Coordinate end) override;
            // The override declaration
        
        vector<Coordinate> validMoves (Board &theBoard) override;
            // Give all the valid move options
        
        void update() override;

        void updateMoved() override;
};

#endif
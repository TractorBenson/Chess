#ifndef BISHOP_H
#define BISHOP_H

#include "chess/chess.h"

using namespace std;


class Bishop : public Chess {
    public:
        Bishop(Color color, Square *theSquare, bool isMoved);
        bool isValidMove(Board &theBoard, Coordinate begin, 
                         Coordinate end) override;
            // The override declaration

        vector<Coordinate> validMoves (Board &theBoard) override;
            // Give all the valid move options
        
        void update() override;

        void updateMoved() override;
};

#endif



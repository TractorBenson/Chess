#ifndef QUEEN_H
#define QUEEN_H

#include "chess/chess.h"

using namespace std;
class Queen : public Chess {
    public:
        Queen(Color color, Square *theSqare, bool isMoved);
        bool isValidMove(Board &theBoard, Coordinate begin, 
                         Coordinate end) override;
            // The override declaration
        
        vector<Coordinate> validMoves (Board &theBoard) override;
            // Give all the valid move options
        
        void update() override;

        void updateMoved() override;
};

#endif

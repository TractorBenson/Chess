#ifndef ROOK_H
#define ROOK_H

#include "chess/chess.h"

using namespace std;

class Rook : public Chess {
    bool isMoved;
    public:
        Rook(Color color, Square *theSquare, bool isMoved);
        bool isValidMove(const Board &theBoard, Coordinate begin, 
                                    Coordinate end) const override;
            // The override declaration
        
        vector<Coordinate> validMoves (const Board &theBoard) const override;
            // Give all the valid move options
        
        void update() override;

        void updateMoved() override;
};

#endif
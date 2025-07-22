#ifndef KING_H
#define KING_H

#include "chess/chess.h"

class King : public Chess {
    bool isMoved;
    public:
        bool isValidMove(const Board &theBoard, Coordinate begin, 
                                    Coordinate end) const override;
            // The override declaration
        
        vector<Coordinate> validMoves (const Board &theBoard) const override;
            // Give all the valid move options

        int isChecked (const vector<vector<Square>> &grid, 
                       const vector<unique_ptr<Chess>> &whiteChesses, 
                       const vector<unique_ptr<Chess>> &blackChesses) const;
            // Determine whether the king is checked, and return the number 
            //   of enemy chesses that are checking it.
    
        bool isCheckmated (const vector<vector<Square>> &grid, 
                           const vector<unique_ptr<Chess>> &whiteChesses, 
                           const vector<unique_ptr<Chess>> &blackChesses) const;
            // Determine if the king is checkmated
        
        void update() override;

        void updateMoved() override;
};

#endif
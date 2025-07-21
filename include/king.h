#ifndef KING_H
#define KING_H

#include "chess.h"

class King : public Chess {
    bool isMoved;
    public:
        bool isValidMove(const Board &theBoard, Coordinate begin, 
                                    Coordinate end) const override;
            // The override declaration

        bool isCheck (const Board &theBoard, Coordinate posi) 
                            const override;
            // The override declaration
        
        bool isCheckmate (const Board &theBoard, Coordinate posi) 
                            const override;
            // The override declaration
};

#endif
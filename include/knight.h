#ifndef KNIGHT_H
#define KNIGHT_H

#include "chess.h"

class Knight : public Chess {
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
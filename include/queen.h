#ifndef QUEEN_H
#define QUEEN_H

#include "chess.h"

class Queen : public Chess {
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
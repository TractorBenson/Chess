#include <cstddef>
#include "color.h"
#include "chess.h"
#include "chesstype.h"
#include "coordinate.h"
#include "board.h"
#include "square.h"


Color Chess::getColor() const {
    return color;
}

ChessType Chess::getType() const {
    return type;
}

Square *Chess::getSquare() const {
    return theSquare;
}

Coordinate Chess::getCoordinate() const {
    return this->getSquare()->getCoordinate();
}
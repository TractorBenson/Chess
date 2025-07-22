#include <cstddef>
#include "color.h"
#include "chess.h"
#include "chesstype.h"
#include "coordinate.h"
#include "board.h"
#include "square.h"

using namespace std;

Chess::Chess(Color color, ChessType type, Square *theSquare): color{color}, 
                type{type}, theSquare{theSquare} {}

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
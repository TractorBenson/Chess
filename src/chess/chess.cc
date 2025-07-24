#include <cstddef>
#include "enum/color.h"
#include "chess/chess.h"
#include "enum/chesstype.h"
#include "struct/coordinate.h"
#include "square/square.h"
#include "board/board.h"

using namespace std;

Chess::Chess(Color color, ChessType type, Square *theSquare, bool isMoved): 
            color{color}, type{type}, theSquare{theSquare}, isMoved{isMoved} {}

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

void Chess::setSquare(Square *sqr) {
    theSquare = sqr;
    return;
}

bool Chess::getIsMovedStatus() const {
    return isMoved;
}


bool Chess::checkCanBeEnPassant() const {}


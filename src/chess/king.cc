#include "chess/king.h"

using namespace std;

King::King(Color color, Square *theSquare, bool isMoved): 
    Chess{color, ChessType::King, theSquare}, isMoved{isMoved} {}

void King::update() {}

void King::updateMoved() { isMoved = true; }





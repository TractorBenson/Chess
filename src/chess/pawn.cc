#include "chess/pawn.h"


void Pawn::setEnPassant() { canBeEnPassant = true; }

void Pawn::update() { canBeEnPassant = false };

void Pawn::updateMoved() { isMoved = true; }




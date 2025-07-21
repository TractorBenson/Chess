#include "../include/square.h"
#include <iostream>

using namespace std;

// Constructor of a Square, used when initiailizing a grid
Square::Square(Coordinate coordinate, Color color = Color::NOTHING, 
    TextDisplay *td, GraphDisplay *gd) : 
    coordinate{coordinate}, color{color}, td{td}, gd{gd} {}

// notifies both text and graph displayer
// set the td and gd (they are unique) when initializing a grid
void Square::notifyDisplayer() {
    for (auto displayer : obs) {
        displayer->notify();
    }
}

// return the pointer to the chess on current board. 
//   If current Square has no chess on it, return nullptr
Chess* Square::getChess() {
    if (color != Color::NOTHING) {
        return chess;
    }
    return nullptr;
}

Color Square::getColor() {
    return color;
}

// Place a chess newChess on the current Square. If Current
//   Square already has an existing chess, output message and do nothing
void Square::setChess(Chess* newChess) {
    if (color == Color::NOTHING) {
        chess = newChess;
    } else {
        cout << "Invalid chess placement! 
        A chess already exists in the square!" << endl;
    }
}

void Square::removeChess() {
    chess = nullptr;
}

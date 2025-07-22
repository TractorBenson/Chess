#include "square/square.h"
#include <iostream>

using namespace std;

// Constructor of a Square, used when initiailizing a grid
Square::Square(Coordinate coordinate, Color color = Color::NOTHING) : 
    coordinate{coordinate}, color{color} {}

Square::Square(const Square& newSquare){
    coordinate = newSquare.coordinate;
    color = newSquare.color;
    chess = nullptr;
    obs = nullptr;
}

// Remember to attach observer after initilizing a new Square
void Square::attachObserver(unique_ptr<Observer> newObs) {
    obs.emplace_back(newObs.get());
}

// notifies both text and graph displayer. This method
//   should be called when initializing a square and updating chess
void Square::notifyDisplayer() {
    for (auto displayer : obs) {
        displayer->notify();
    }
}

// Returns the coordinate of current square
Coordinate Square::getCoordinate() const {
    return coordinate;
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
void Square::setChess(unique_ptr<Chess> newChess) {
    if (color == Color::NOTHING) {
        chess = newChess.get();
    } else {
        cout << "Invalid chess placement! " << 
        "A chess already exists in the square!" << endl;
    }
}

void Square::removeChess() {
    chess = nullptr;
}

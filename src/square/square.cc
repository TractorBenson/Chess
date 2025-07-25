#include "square/square.h"
#include <iostream>

using namespace std;

// Constructor of a Square, used when initiailizing a grid
Square::Square(Coordinate coordinate, Color color = Color::NOTHING) : 
    coordinate{coordinate}, color{color} {}

// Square::Square(const Square& newSquare){
//     coordinate = newSquare.coordinate;
//     color = newSquare.color;
//     chess = newSquare.getChess();
// }

// Remember to attach observer after initilizing a new Square
void Square::attachObserver(vector<unique_ptr<Observer>>& newObs) {
    for (auto& ob : newObs) {
        obs.emplace_back(ob.get());
    }
}

// notifies both text and graph displayer. This method
//   should be called when initializing a square and updating chess
void Square::notifyDisplayer() {
    for (auto displayer : obs) {
        displayer->notify(*this);
    }
}

// Returns the coordinate of current square
Coordinate Square::getCoordinate() const {
    return coordinate;
}

// return the pointer to the chess on current board. 
//   If current Square has no chess on it, return nullptr
Chess* Square::getChess() const{
    return chess;
}

Color Square::getColor() const{
    return color;
}

// Place a chess newChess on the current Square. If Current
//   Square already has an existing chess, output message and do nothing
void Square::setChess(Chess* newChess) {
    if (!chess) {
        chess = newChess;
    } else if (!newChess){
        chess = newChess;
    } else {
        cout << "Invalid chess placement! " << 
        "A chess already exists in the square!" << endl;
    }
}

void Square::setCoordinate(int row, int col) {
    coordinate.row = row;
    coordinate.col = col;
}

void Square::setColor(Color newColor) {
    color = newColor;
}
void Square::removeChess() {
    chess = nullptr;
}


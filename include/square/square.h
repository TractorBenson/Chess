#ifndef SQUARE_H
#define SQUARE_H
// #include "chess/chess.h"
#include "enum/color.h"
#include "struct/coordinate.h"
#include "observer/observer.h"
#include <memory>
#include <vector>

class Chess;

using namespace std;

class Square {
    Coordinate coordinate;
    Color color;
    Chess* chess = nullptr;
    vector<Observer*> obs;
    
    public:
    Square() = default;
    // Constructor. User has to call setChess to place a chess on Square
    Square(Coordinate coordinate, Color color);

    Square(const Square& newSquare);
    void attachObserver(vector<unique_ptr<Observer>>& newObs);
    // Notify text and graph displayers of the square's current state
    void notifyDisplayer();

    // return the pointer to the chess on current board. 
    Chess* getChess() const;
    // Returns the coordinate of current square
    Coordinate getCoordinate() const;
    Color getColor() const;
    // Place a paticular chess on the current square
    void setChess(Chess* newChess);
    void setCoordinate(int row, int col);
    void setColor(Color newColor);
    // Remove the chess, if exists, from the current square
    void removeChess();
};


#endif
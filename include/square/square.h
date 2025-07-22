#ifndef SQUARE_H
#define SQUARE_H
#include "chess/chess.h"
#include "chess/color.h"
#include "struct/coordinate.h"
#include "observer/observer.h"
#include <memory>
#include <vector>

using namespace std;

class Square {
    Coordinate coordinate;
    Color color;
    Chess* chess = nullptr;
    vector<Observer*> obs;
    
    public:
    // Constructor. User has to call setChess to place a chess on Square
    Square(Coordinate coordinate, Color color);

    Square(const Square& newSquare);
    void attachObserver(unique_ptr<Observer> newObs);
    // Notify text and graph displayers of the square's current state
    void notifyDisplayer();
    // return the pointer to the chess on current board. 
    Chess* getChess();
    // Returns the coordinate of current square
    Coordinate getCoordinate() const;
    Color getColor();
    // Place a paticular chess on the current square
    void setChess(unique_ptr<Chess> newChess);
    // Remove the chess, if exists, from the current square
    void removeChess();
};


#endif
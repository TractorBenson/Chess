#ifndef SQUARE_H
#define SQUARE_H
#include "chess.h"
#include "color.h"
#include <memory>
#include "coordinate.h"
#include "observer.h"

using namespace std;

class Square {
    Coordinate coordinate;
    Color color;
    Chess* chess = nullptr;
    vector<Observer*> obs;

    public:
    // Constructor. User has to call setChess to place a chess on Square
    Square(Coordinate coordinate, Color color, 
    TextDisplay *td, GraphDisplay *gd);
    // Notify text and graph displayers of the square's current state
    void notifyDisplayer();
    // return the pointer to the chess on current board. 
    Chess* getChess();
    Color getColor();
    // Place a paticular chess on the current square
    void setChess(unique_ptr<Chess>);
    // Remove the chess, if exists, from the current square
    void removeChess();
};


#endif
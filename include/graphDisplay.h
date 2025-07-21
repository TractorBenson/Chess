#ifndef GRAPHDISPLAY_H
#define GRAPHDISPLAY_H
// ensure only define once

#include "observer/observer.h"

class GraphDisplay {
    // store chars to display the board
    const size_t WINDOW_SIZE;

public:
    // Square call this to update state
    void notify(Square&) override;

    // use to output the board to given ostream
    void drawSquare(Square&);
};

#endif

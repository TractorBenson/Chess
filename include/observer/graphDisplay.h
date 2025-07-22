#ifndef GRAPHDISPLAY_H
#define GRAPHDISPLAY_H
// ensure only define once

#include <string>
#include <memory>
#include "observer/observer.h"
#include "enum/chesstype.h"
#include "enum/color.h"
#include "struct/coordinate.h"
#include "observer/window.h"

class GraphDisplay : public Observer{
    // store chars to display the board
    const size_t WINDOW_SIZE = 500; // set screen size to be 500. can be adjusted to be other values.
    const int BACKGROUND_COLOR = 0; // set default background color to be black

    std::unique_ptr<Xwindow> xw;
    size_t gridLength; // size of grid, in number of cell
    size_t size = 0; // size of one cell, in px
    size_t edgeSize = 0; // size of edge, in px

    // helper function to draw square by index
    void drawSquareByIndex(size_t row, size_t col, Color color);

public:
    // ctor, n is number of cell in the grid
    GraphDisplay(size_t n = 8);

    // Square call this to update state
    void notify(Square&) override;
};

#endif

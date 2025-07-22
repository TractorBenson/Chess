#ifndef GRAPHDISPLAY_H
#define GRAPHDISPLAY_H
// ensure only define once

#include <string>
#include <memory>
#include "observer/observer.h"
#include "observer/window.h"

class GraphDisplay : public Observer{
    // store chars to display the board
    const size_t WINDOW_SIZE;
    const int BACKGROUND_COLOR;

    std::unique_ptr<Xwindow> xw;
    size_t gridLength; // size of grid, in number of cell
    size_t size = 0; // size of one cell, in px
    size_t edgeSize = 0; // size of edge, in px

    // helper function to draw square by index
    void drawSquareByIndex(size_t row, size_t col, Color color);
    // helper function to draw string by index
    void drawStringByIndex(size_t row, size_t col, std::string str);

public:
    // ctor
    GraphDisplay(size_t n = 8);
    // dtor
    ~GraphDisplay();

    // Square call this to update state
    void notify(Square&) override;
};

#endif

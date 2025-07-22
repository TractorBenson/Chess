#include "observer/graphDisplay.h"

GraphDisplay::GraphDisplay(size_t n): size{n} {
    int resultingGridLength = WINDOW_SIZE; // use to subract until fit
    int mod = resultingGridLength % n; // module to store redundant edge
    resultingGridLength -= mod; // minus edge, so is real size of grid

    edgeSize = mod / 2; // mod is total size of two side of edge, so /2

    gridLength = resultingGridLength / size; // length in px of single square

    // init background
    xw->fillRectangle(0, 0, WINDOW_SIZE, WINDOW_SIZE, BACKGROUND_COLOR);
}

void GraphDisplay::drawSquareByIndex(size_t row, size_t col, Color color) {
    // calc real position of the Square
    int resultX = col * size + edgeSize;
    int resultY = row * size + edgeSize;

    // convert color into given format
    int colorNum;
    if (color == Color::WHITE) colorNum = 2;
    else if (color == Color::BLACK) colorNum = 3;
    else colorNum = 0;

    xw->fillRectangle(resultX, resultY, size, size, colorNum);
}

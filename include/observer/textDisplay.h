#ifndef TEXTDISPLAY_H
#define TEXTDISPLAY_H
// ensure only define once

#include <iostream>
#include <vector>
#include "observer/observer.h"
#include "enum/chesstype.h"
#include "enum/color.h"
#include "struct/coordinate.h"
#include "chess/chess.h"

class TextDisplay : public Observer{
    // store chars to display the board
    size_t size;
    std::vector<std::vector<char>> charGrid;

    // helper function convert a Square to specific char
    char convertSquare(const Square&);

public:
    // use a size_t to init vector
    TextDisplay(size_t size);

    // Square call this to update state
    void notify(const Square&) override;

    // use to output the board to given ostream
    std::ostream &printBoard(std::ostream &os);
};

// use to output the board
std::ostream &operator<<(std::ostream &os, TextDisplay &td);

#endif

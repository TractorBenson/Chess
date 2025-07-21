#ifndef TEXTDISPLAY_H
#define TEXTDISPLAY_H
// ensure only define once

#include "observer/observer.h"

class TextDisplay : public Observer{
    // store chars to display the board
    std::vector<std::vector<char>>;

public:
    // Square call this to update state
    void notify(Square&) override;

    // use to output the board to given ostream
    std::ostream printBoard(std::ostream &os);
};

// use to output the board
std::ostream operator<<(std::ostream &os, TextDisplay &td);

#endif

#ifndef GRAPHDISPLAY_H
#define GRAPHDISPLAY_H
// ensure only define once

class Square; // forward declaration

class GraphDisplay {
    // store chars to display the board
    std::vector<std::vector<char>>;

public:
    // Square call this to update state
    void notified(Square&);

    // use to output the board to given ostream
    std::ostream printBoard(std::ostream &os);
}

// use to output the board
std::ostream operator<<(std::ostream &os, TextDisplay &td);

#endif
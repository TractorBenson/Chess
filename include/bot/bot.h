#ifndef BOT_H
#define BOT_H
// ensure only define once

#include <string>
#include "enum/color.h"

class Board; // forward declaration

class Bot {
    // pointer to current board
    Board *theBoard;

    // color of chess of this bot
    Color color;

    // level of the Bot
    int level;

public:
    // ctor take a Board* and Color to init
    Bot(Board *theBoard, Color color, int level);

    // main.cc will call this function to tell this Bot to move one step,
    // bot return string, format is the same as human player
    std::string move();
}


#endif

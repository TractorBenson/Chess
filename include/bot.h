#ifndef BOT_H
#define BOT_H
// ensure only define once

#include color.h

class Board; // forward declaration

class Bot {
    // pointer to current board
    Board *theBoard;

    // color of chess of this bot
    Color color;

public:
    // ctor take a Board* and Color to init
    Bot(Board *theBoard, Color color);

    // main.cc will call this function to tell this Bot to move one step,
    // all operation will be done by Bot, so just one calling
    virtual void move() = 0;
}



#endif
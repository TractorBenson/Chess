#ifndef BOT_H
#define BOT_H
// ensure only define once

#include <string>
#include <map>
#include <vector>
#include "enum/color.h"
#include "struct/coordinate.h"
#include "third_party/PRNG.h"
#include "chess/king.h"
#include "board/board.h"

class Bot {
    // pointer to current board
    Board *theBoard;
    King *opponentKing;

    // color of chess of this bot
    Color color;

    // level of the Bot
    int level;

    PRNG prng;

    std::map<Coordinate, std::vector<Coordinate>> moveMap;

    void constructMoveMap();

    std::string convertCoordinate(Coordinate begin, Coordinate end);

    int random(int n);
    std::string randomMove();
    std::string captureFirstMove();
    std::string surviveFirstMove();

public:
    // ctor take a Board* ,Color and level to init
    Bot(Board *theBoard, Color color, int level);

    // main.cc will call this function to tell this Bot to move one step,
    // bot return string, format is the same as human player
    std::string move();
};


#endif

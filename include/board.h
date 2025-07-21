#ifndef BOARD_H
#define BOARD_H
#include "chess.h"
#include "square.h"
#include "color.h"
#include "observer.h"
#include <iostream>
#include <vector>
#include <memory>

using namespace std;

class Board {
    // The grid consists of mutiple squares
    vector<vector<Square>> grid;

    // Pointing to the textdisplayer and graphic displayer
    vector<unique_ptr<Observer>> obs;

    // Two vectors storing all white and black chesses
    vector<unique_ptr<Chess>> whiteChesses;
    vector<unique_ptr<Chess>> blackChesses;
    // Color indicating whose turn to play
    Color currentPlayer = Color::WHITE;
    // Keep track to both player's last move

    public:
        vector<vector<Square>>& getGrid() const;
        void switchPlayer(); // switch player's turn to play
        bool checkDraw() const;
        void updateChess();
        void initSquares(); // initialize the board before game starts

        // Place a chess of certain type on loc with certain color
        void placeChess(Coordinate loc, char type, Color color);

        // Remove existing chess on loc, do nothing if there's no chess on loc
        void removeChess(Coordinate loc);
        // place all chesses with default layout before gamestart
        void initChessesWithDefaultArrange();
        // Checks if there are two kings, each in different colors
        bool isValidSetup();
        // Move the chess on location begin to end
        bool moveChess(Coordinate begin, Coordinate end);
        // Determine if a check exist in current step
        bool isCheck() const;
        // Determine if a checkmate exist in current step
        bool isCheckmate() const;
        friend ostream &operator<<(ostream &out, const Grid &g);
};

ostream &operator<<(ostream &out, const Grid &g);
#endif

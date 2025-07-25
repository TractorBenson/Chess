#ifndef BOARD_H
#define BOARD_H
#include "chess/chess.h"
#include "square/square.h"
#include "enum/chesstype.h"
#include "enum/color.h"
#include "observer/observer.h"
#include "struct/moveBackup.h"
#include <iostream>
#include <cstddef>
#include <vector>
#include <memory>
#include <optional>

class King;
using namespace std;
class Board {
    const size_t sideLength = 8;
    // The grid consists of mutiple squares
    vector<vector<Square>> grid;
    // Pointing to the textdisplayer and graphic displayer
    vector<unique_ptr<Observer>> obs;

    // Two vectors storing all white and black chesses
    vector<unique_ptr<Chess>> whiteChesses;
    vector<unique_ptr<Chess>> blackChesses;

    // Used in checkDraw, isValidSetup.
    King* whiteKing = nullptr;
    King* blackKing = nullptr;

    // Allows for undoing the last step of the move
    optional<MoveBackup> lastTry;
    
    public:
        Board();
        const vector<vector<Square>>& getGrid() const;
        size_t getSideLength() const;
        Color getChessColor(Coordinate coord) const;
        ChessType getChessType(Coordinate coord) const;
        King* getWhiteKing() const;
        King* getBlackKing() const;
        //const Square& getSquare(Coordinate coord) const;
        bool squareIsEmpty(Coordinate coord) const;
        const vector<unique_ptr<Chess>>& getWhiteChesses() const;
        const vector<unique_ptr<Chess>>& getBlackChesses() const;
        int numOfChesses(ChessType type, Color checkColor) const;

        bool noValidMoves(Color currentPlayer); // check for draw and checkmate
        void updateChess(Color CurrentPlayer);
        void initSquares(); // initialize the board before game starts

        // Place a chess of certain type on loc with certain color
        void placeChess(Coordinate loc, char type);

        // Remove existing chess on loc, do nothing if there's no chess on loc
        void removeChess(Coordinate loc);

        void initBoard();
        // place all chesses with default layout before gamestart
        void initChessesWithDefaultArrange();
        // Checks if there are two kings, each in different colors
        bool isValidSetup();
        // checks if the chess is a pawn and can be promoted in current round
        bool canPromote(Coordinate begin, Coordinate end);

        // Move the chess on location begin to end, 
        //   remember to take these two coordinate from a vector from bot 
        bool moveChess(Coordinate begin, Coordinate end);
        void resetLT();
        // Try to move a chess, used to check if the move causes a "self-check"
        void testMove(Coordinate begin, Coordinate end, Color currentPlayer);

        void simpleMove(Coordinate begin, Coordinate end);
        // Redo the last movement of current player
        void redoLastStep();
        // Determine if a check exist in current step
        bool isCheck(Color currentPlayer) const;
        // Determine if a checkmate exist in current step
        bool isCheckmate(Color currentPlayer);
        friend ostream &operator<<(ostream &out, const Board &board);
};

ostream &operator<<(ostream &out, const Board &board);

#endif

#include <algorithm>
#include "chess/pawn.h"
#include "chess/king.h"
#include "board/board.h"
#include "square/square.h"

using namespace std;

Pawn::Pawn(Color color, Square *theSquare, bool isMoved, 
           bool canBeEnPassant): Chess{color, ChessType::Pawn, theSquare}, 
                                 isMoved{isMoved}, 
                                 canBeEnPassant{canBeEnPassant} {}


bool Pawn::isValidMove(const Board &theBoard, Coordinate begin, 
                    Coordinate end) const {
    if (begin.col < 0 || begin.col >= theBoard.getSideLength() || 
        begin.row < 0 || begin.row >= theBoard.getSideLength() || 
        end.col < 0 || end.col >= theBoard.getSideLength() ||
        end.row < 0 || end.row >= theBoard.getSideLength()) {
        // If it is out of bound, return false
        return false;
    }
    if (begin.col == end.col && begin.row == end.row) return false;
    // If the destination position is the start position, give back false

    int diff_x_coordinate = end.col - begin.col;
    // The distance of x-coordinate from begin's position to end's position

    int diff_y_coordinate = end.row - begin.row;
    // The distance of y-coordinate from begin's position to end's position

    vector<vector<Square>> &tmp_grid = theBoard.getGrid();
    // Get the grid reference

    Chess *tmp_king = nullptr; // The pointer points to the king
    Color color = this->getColor(); // The color of the current player
    if (color == Color::White) tmp_king = theBoard->getWhiteKing();
    else tmp_king = theBoard->getBlackKing();
    // Get the king pointer of the friend color

    if ((color == Color::BLACK && diff_y_coordinate >= 0) ||
        (color == Color::WHITE && diff_x_coordinate <= 0)) {
            // If the pawn is not moving forward, return false
        return false;
    }

    if (isMoved == true && diff_y_coordinate > 1) {
        // If the pawn has moved, but it moves vertically over 1 step, 
        //   return false.
        return false;
    }


}

vector<Coordinate> Pawn::validMoves (const Board &theBoard) const {
    vector<Coordinate> result_moves;
    return
}

void Pawn::setEnPassant() { canBeEnPassant = true; }

void Pawn::update() { canBeEnPassant = false };

void Pawn::updateMoved() { isMoved = true; }




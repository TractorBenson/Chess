#include <algorithm>
#include "chess/pawn.h"
#include "chess/king.h"
#include "chess/chess.h"
#include "square/square.h"

using namespace std;

Pawn::Pawn(Color color, Square *theSquare, bool isMoved, 
           bool canBeEnPassant): Chess{color, ChessType::Pawn, 
                                       theSquare, isMoved}, 
                                 canBeEnPassant{canBeEnPassant} {}

bool Pawn::checkCanBeEnPassant() const {
    return canBeEnPassant;
}

bool Pawn::isValidMove(Board &theBoard, Coordinate begin, 
                       Coordinate end) {
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

    const vector<vector<Square>> &tmp_grid = theBoard.getGrid();
    // Get the grid reference

    // Check if the ending point is the friend chess, if is, then give false 
    //   back.
    Chess *tmp_chess = tmp_grid[end.row][end.col].getChess();
    if (tmp_chess != nullptr && tmp_chess->getColor() == this->getColor()) 
        return false;


    King *tmp_king = nullptr; // The pointer points to the king
    Color color = this->getColor(); // The color of the current player
    if (color == Color::WHITE) tmp_king = theBoard.getWhiteKing();
    else tmp_king = theBoard.getBlackKing();
    // Get the king pointer of the friend color

    if ((color == Color::BLACK && diff_y_coordinate >= 0) ||
        (color == Color::WHITE && diff_y_coordinate <= 0)) {
            // If the pawn is not moving forward, return false
        return false;
    }

    if (abs(diff_y_coordinate) > 2) {
        // If the pawn's vertical movement is greater than 2, give back false
        return false;
    }

    // Check if the x coordinate is over 1 steps
    if (abs(diff_x_coordinate) > 1) return false;


    // Check if the pawn goes vertically, there is no obstacles on the way and 
    //   at the destination.
    if (diff_x_coordinate == 0) {

        tmp_chess = tmp_grid[end.row][end.col].getChess();
        // If it has a chess at the destination, give back false
        if (tmp_chess != nullptr) {
            return false;
        }

        // If it goes two steps
        if (abs(diff_y_coordinate) == 2)
        {
            // If it has already been moved
            tmp_chess = tmp_grid[begin.row + (diff_y_coordinate / 
                                             abs(diff_y_coordinate))]
                                [begin.col].getChess();
            if (isMoved == true) return false;
            else if (tmp_chess != nullptr) return false;
            // Else if there is an obstacle on the way, give back false
        }
    } else {
        // Else, the x-coordinate movement must be 1
        if (abs(diff_y_coordinate) != 1) return false;

        tmp_chess = tmp_grid[end.row][end.col].getChess();
        if (tmp_chess == nullptr) {
            // If there is no chess there that can be eaten, check the 
            //   En Passant situation.
            if (color == Color::BLACK) {
                // If the chess color is black, add row number by 1 to find 
                //   the pawn beside it.
                tmp_chess = tmp_grid[end.row + 1][end.col].getChess();
            } else {
                tmp_chess = tmp_grid[end.row - 1][end.col].getChess();
            }
            if (tmp_chess == nullptr) return false;
            if (!(tmp_chess->getType() == ChessType::Pawn && 
                  tmp_chess->getColor() != this->getColor() && 
                  tmp_chess->checkCanBeEnPassant())) {
                // If the chess is not the situation: A Pawn that can be 
                //   En Passant, give false back.
                return false;
            }
        }
    }


    // Mock the board first
    theBoard.testMove(begin, end, color);
    if (tmp_king->isChecked(theBoard) != 0) {
        theBoard.redoLastStep();
        // If the king is checked after this move, is invalid, 
        //   return false and remember to undo this move
        return false;
    }
    // If this move will be valid, return true then, and don't
    //   forget to make the board one step back.
    theBoard.redoLastStep();
    return true;
}

vector<Coordinate> Pawn::validMoves (Board &theBoard) {
    vector<Coordinate> result_moves; // The results
    vector<vector<int>> directions = {
        {1, 0}, 
        {2, 0}, 
        {1, 1}, 
        {1, -1}
    }; // All the valid directions

    // Get the original position
    Coordinate original_posi = this->getCoordinate();


    for (int i = 0; i < directions.size(); i++) {
        // Loop all the valid directions to get all the valid 
        //   moves.

        // Get the mock position
        Coordinate mock_posi = original_posi;
        
        if (this->getColor() == Color::BLACK) {
            // If the color is black, the position will go down
            mock_posi.row -= directions[i][0];
            mock_posi.col -= directions[i][1];
        } else {
            // If the color is white, the position will go up
            mock_posi.row += directions[i][0];
            mock_posi.col += directions[i][1];
        }
        if (this->isValidMove(theBoard, original_posi, 
                              mock_posi)) {
            // If the mock position is valid, record it
            result_moves.emplace_back(mock_posi);
        }
    }
    // Give back the result
    return result_moves;
}

void Pawn::setEnPassant() { canBeEnPassant = true; }

void Pawn::update() { canBeEnPassant = false; }

void Pawn::updateMoved() { isMoved = true; }




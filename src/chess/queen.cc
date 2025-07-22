#include "chess/queen.h"
#include <algorithm>
#include "board/board.h"
#include "chess/king.h"

bool Queen::isValidMove(const Board &theBoard, Coordinate begin, 
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

    if (!(abs(diff_x_coordinate) == abs(diff_y_coordinate) || 
          diff_x_coordinate == 0 || 
          diff_y_coordinate == 0)) {
        // If the distance is not diagonal, horizontal or vertical, 
        //   return false (not a valid move)
        return false;
    }


    int distance = abs(max(diff_x_coordinate, diff_y_coordinate));
    // Find the number of step between begin position and end position


    Coordinate one_step_direction{diff_y_coordinate / distance, 
                                  diff_x_coordinate / distance};
    // Get the step direction to reach the end position


    Coordinate moack_posi = this->getCoordinate();
    // Get the mock position to see if there are any obstacles on the way


    for (int i = 1; i < distance; i++) {
        // Check if there are any obstacles on the way

        // Get one step further first, then check
        mock_posi.col += one_step_direction.col;
        mock_posi.row += one_step_direction.row;
        Chess *tmp_chess = tmp_grid[mock_posi.row]
                                   [mock_posi.col].getChess();
        // Get the chess on the grid with that mock position coordinate
        if (tmp_chess != nullptr) {
            // If there exist a chess, then it's impossible to get through, 
            //   give back false.
            return false;
        }
    }

    Chess *tmp_king = nullptr; // The pointer points to the king
    Color color = this->getColor(); // The color of the current player
    if (color == Color::White) tmp_king = theBoard->getWhiteKing();
    else tmp_king = theBoard->getBlackKing();
    // Get the king pointer of the friend color

    // Mock the board first
    theBoard.moveAnyway(begin, end);
    if (tmp_king->isChecked()) {
        theBoard.backOneStep();
        // If the king is checked after this move, is invalid, 
        //   return false and remember to undo this move
        return false;
    }
    // If this move will be valid, return true then, and don't
    //   forget to make the board one step back.
    theBoard.backOneStep();
    return true;
}

vector<Coordinate> Queen::validMoves (const Board &theBoard) const {

}

void Queen::update() {}

void Queen::updateMoved() {}


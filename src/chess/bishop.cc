#include <cstddef>
#include <algorithm>
#include "chess/chess.h"
#include "chess/bishop.h"
#include "board/board.h"
#include "chess/king.h"

using namespace std;

bool Bishop::isValidMove(const Board &theBoard, Coordinate begin, 
                         Coordinate end) const {
    if (begin.col < 0 || begin.col >= theBoard.getSideLength() || 
        begin.row < 0 || begin.row >= theBoard.getSideLength() || 
        end.col < 0 || end.col >= theBoard.getSideLength() ||
        end.row < 0 || end.row >= theBoard.getSideLength()) {
        // If it is out of bound, return false
        return false;
    }
    int diff_x_coordinate = end.col - begin.col;
    // The distance of x-coordinate from begin's position to end's position

    int diff_y_coordinate = end.row - begin.row;
    // The distance of y-coordinate from begin's position to end's position

    vector<vector<Square>> &tmp_grid = theBoard.getGrid(); // Get the grid reference
    if (diff_x_coordinate != diff_y_coordinate & 
        diff_x_coordinate != diff_y_coordinate * -1) {
        // If the distance is not diagonal, return false (not a valid move)
        return false;
    }
    else {
        int steps_to_check_obstacle = abs(diff_x_coordinate);
        // The steps to move from the begin coordinate to end coordinate

        Coordinate one_step{diff_x_coordinate / steps_to_check_obstacle, 
                            diff_y_coordinate / steps_to_check_obstacle};
        // The direction to move from the current coordinate one step closer 
        //   to the end coordinate.

        Coordinate current_check_step{begin};
        // Initialize the start position for checking
        for (int i = 1; i < steps_to_check_obstacle - 1; i++) {
            current_check_step.col += one_step.col;
            current_check_step.row += one_step.row;
            Chess *getTheChess = tmp_grid[current_check_step.row]
                                         [current_check_step.col].getChess();
            if (getTheChess) {
                // If it has obstacle, return false
                return false;
            }
        }
        Chess *tmp_king = nullptr; // The pointer points to the king
        Color color = this->getColor(); // The color of the current player
        vector<unique_ptr<Chess>> &tmp_white_chesses = getWhiteChesses();
        vector<unique_ptr<Chess>> &tmp_black_chesses = getBlackChesses();

        Board mock_board; // This is the mock board

        vector<unique_ptr<Chess>> mock_white_chesses;
        // This is mock vector of white chesses

        vector<unique_ptr<Chess>> mock_black_chesses;
        // This is mock vector of black chesses

        if (color == Color::White) tmp_king = theboard.getWhiteKing();
        else tmp_king = theboard.getBlackKing();
        // Get the pointer to the king


        if (!(tmp_king_king_type->isChecked())) {
            // If it doesn't make the original king be under attacked

            

            return true;
        } else {

        }
    }

void Bishop::update() {}

void Bishop::updateMoved {}

}



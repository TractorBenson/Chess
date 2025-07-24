#include <cstddef>
#include <algorithm>
#include "chess/chess.h"
#include "chess/bishop.h"
#include "chess/king.h"
#include "board/board.h"

using namespace std;


Bishop::Bishop(Color color, Square *theSquare, bool isMoved):
    Chess{color, ChessType::Bishop, theSquare, isMoved} {}


bool Bishop::isValidMove(Board &theBoard, Coordinate begin, 
                         Coordinate end) {
    if (begin.col < 0 || begin.col >= theBoard.getSideLength() || 
        begin.row < 0 || begin.row >= theBoard.getSideLength() || 
        end.col < 0 || end.col >= theBoard.getSideLength() ||
        end.row < 0 || end.row >= theBoard.getSideLength()) {
        // If it is out of bound, return false
        return false;
    }
    if (begin.col == end.col && begin.row == end.row) return false;
    int diff_x_coordinate = end.col - begin.col;
    // The distance of x-coordinate from begin's position to end's position

    int diff_y_coordinate = end.row - begin.row;
    // The distance of y-coordinate from begin's position to end's position

    const vector<vector<Square>> &tmp_grid = theBoard.getGrid();
    // Get the grid reference


    if (abs(diff_x_coordinate) != abs(diff_y_coordinate)) {
        // If the distance is not diagonal, return false (not a valid move)
        return false;
    }

    // Check if the ending point is the friend chess, if is, then give false 
    //   back.
    Chess *tmp_chess = tmp_grid[end.row][end.col].getChess();
    if (tmp_chess != nullptr && tmp_chess->getColor() == this->getColor()) 
        return false;
    
    else {
        int steps_to_check_obstacle = abs(diff_x_coordinate);
        // The steps to move from the begin coordinate to end coordinate

        Coordinate one_step{diff_y_coordinate / steps_to_check_obstacle, 
                            diff_x_coordinate / steps_to_check_obstacle};
        // The direction to move from the current coordinate one step closer 
        //   to the end coordinate.

        Coordinate current_check_step{begin};
        // Initialize the start position for checking
        for (int i = 1; i < steps_to_check_obstacle; i++) {
            current_check_step.col += one_step.col;
            current_check_step.row += one_step.row;
            Chess *getTheChess = tmp_grid[current_check_step.row]
                                         [current_check_step.col].getChess();
            if (getTheChess != nullptr) {
                // If it has obstacle, return false
                return false;
            }
        }
        King *tmp_king = nullptr; // The pointer points to the king
        Color color = this->getColor(); // The color of the current player
        if (color == Color::WHITE) tmp_king = theBoard.getWhiteKing();
        else tmp_king = theBoard.getBlackKing();
        // Get the king pointer of the friend color

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
}

vector<Coordinate> Bishop::validMoves (Board &theBoard) {
    vector<Coordinate> result_moves; // The results
    vector<vector<int>> directions = {
        {-1,  1},
        {-1, -1},
        { 1, -1},
        { 1,  1}
    }; // The valid move directions

    Coordinate original_posi = this->getCoordinate();
    // The coordinate of this position


    for (int i = 0; i < directions.size(); i++) {
        // Go to those four direction valid to find out all the valid moves

        Coordinate mock_posi = original_posi;
        // The mock position



        for (int j = 0; j < theBoard.getSideLength(); j++) {

            // Move the mock position one step further
            mock_posi.row += directions[i][0];
            mock_posi.col += directions[i][1];

            
            if (this->isValidMove(theBoard, original_posi, 
                                  mock_posi)) {
                // While this mock position is a valid move, 
                result_moves.emplace_back(mock_posi);
            }
        }
    }

    // Return the answer
    return result_moves;
}

void Bishop::update() {}

void Bishop::updateMoved() {}





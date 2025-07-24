#include "chess/queen.h"
#include <algorithm>
#include "board/board.h"
#include "chess/king.h"

using namespace std;

Queen::Queen(Color color, Square *theSqare, bool isMoved): 
    Chess{color, ChessType::Queen, theSqare, isMoved} {}

bool Queen::isValidMove(Board &theBoard, Coordinate begin, 
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

    if (!(abs(diff_x_coordinate) == abs(diff_y_coordinate) || 
          diff_x_coordinate == 0 || 
          diff_y_coordinate == 0)) {
        // If the distance is not diagonal, horizontal or vertical, 
        //   return false (not a valid move)
        return false;
    }


    int distance = max(abs(diff_x_coordinate), abs(diff_y_coordinate));
    // Find the number of step between begin position and end position


    Coordinate one_step_direction{diff_y_coordinate / distance, 
                                  diff_x_coordinate / distance};
    // Get the step direction to reach the end position


    Coordinate mock_posi = this->getCoordinate();
    // Get the mock position to see if there are any obstacles on the way


    for (int i = 1; i < distance; i++) {
        // Check if there are any obstacles on the way

        // Get one step further first, then check
        mock_posi.col += one_step_direction.col;
        mock_posi.row += one_step_direction.row;
        tmp_chess = tmp_grid[mock_posi.row]
                            [mock_posi.col].getChess();
        // Get the chess on the grid with that mock position coordinate
        if (tmp_chess != nullptr) {
            // If there exist a chess, then it's impossible to get through, 
            //   give back false.
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

vector<Coordinate> Queen::validMoves (Board &theBoard) {
    vector<Coordinate> result_moves; // The results
    vector<vector<int>> directions = {
        {-1,  1},
        {-1, -1},
        { 1, -1},
        { 1,  1},
        {-1,  0}, 
        { 1,  0}, 
        { 0, -1},
        { 0,  1}
    }; // All the valid directions
    Coordinate original_posi = this->getCoordinate();
    // The original position

    for (int i = 0; i < directions.size(); i++) {
        // Loop all the valid direction to find all the valid moves

        Coordinate mock_posi = original_posi;
        // The mock position

        mock_posi.row += directions[i][0];
        mock_posi.col += directions[i][1];
        // Make the mock position move one step

        for (int j = 0; j < theBoard.getSideLength(); j++) {
            if (this->isValidMove(theBoard, original_posi, 
                                  mock_posi)) {
                // While this mock position is a valid move, 
                result_moves.emplace_back(mock_posi);
            }

            // Move the mock position one step further
            mock_posi.row += directions[i][0];
            mock_posi.col += directions[i][1];
        }
    }
    // Give back the result
    return result_moves;
}

void Queen::update() {}

void Queen::updateMoved() {}


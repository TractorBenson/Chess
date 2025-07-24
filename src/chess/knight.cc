#include "chess/knight.h"
#include <algorithm>
#include "board/board.h"
#include "chess/king.h"

using namespace std;

Knight::Knight(Color color, Square *theSquare): 
    Chess{color, ChessType::Knight, theSquare} {}


bool Knight::isValidMove(Board &theBoard, Coordinate begin, 
                         Coordinate end) {
    if (begin.col < 0 || begin.col >= theBoard.getSideLength() || 
        begin.row < 0 || begin.row >= theBoard.getSideLength() || 
        end.col < 0 || end.col >= theBoard.getSideLength() ||
        end.row < 0 || end.row >= theBoard.getSideLength()) {
        // If it is out of bound, return false
        return false;
    }
    if (begin.col == end.col && begin.row == end.row) return false;
    int diff_x_coordinate = abs(end.col - begin.col);
    // The distance of x-coordinate from begin's position to end's position

    int diff_y_coordinate = abs(end.row - begin.row);
    // The distance of y-coordinate from begin's position to end's position

    int sum_x_and_y_diff = diff_x_coordinate + diff_y_coordinate;
    if (sum_x_and_y_diff != 3) return false;
    if (!((diff_x_coordinate == 2 && diff_y_coordinate == 1) || 
          (diff_x_coordinate == 1 && diff_y_coordinate == 2))) {
        return false;
    }

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

vector<Coordinate> Knight::validMoves (Board &theBoard) {
    vector<Coordinate> result_moves; // The results
    vector<vector<int>> directions = {
        {  2,  1 },
        {  1,  2 },
        { -1,  2 },
        { -2,  1 },
        { -2, -1 },
        { -1, -2 },
        {  1, -2 },
        {  2, -1 }
    }; // All the valid directions

    // This chess's position
    Coordinate original_posi = this->getCoordinate();

    for (int i = 0; i < directions.size(); i++) {
        // Loop all the directions to find out all the valid moves

        // The mock position
        Coordinate mock_posi = original_posi;

        // Make the position go one step first
        mock_posi.row += directions[i][0];
        mock_posi.col += directions[i][1];

        if (this->isValidMove(theBoard, original_posi, 
                                mock_posi)) {
            // While this mock position is a valid move, 
            result_moves.emplace_back(mock_posi);
        }
    }
    // Give back the result
    return result_moves;
}

void Knight::update() {}

void Knight::updateMoved() {}



#include "chess/rook.h"
#include "chess/rook.h"
#include "board/board.h"
#include "chess/king.h"

using namespace std;

Rook::Rook(Color color, Square *theSquare, bool isMoved): 
    Chess{color, ChessType::Rook, theSquare}, isMoved{isMoved} {}


bool Rook::isValidMove(const Board &theBoard, Coordinate begin, 
                                    Coordinate end) const {
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

    vector<vector<Square>> &tmp_grid = theBoard.getGrid();
    // Get the grid reference

    if (!(diff_x_coordinate == 0 || diff_y_coordinate == 0)) {
        // If the path from begin position to end position is not a horizontal 
        //   or vertical line, return false.
        return false;
    }
    int distance = max(abs(diff_x_coordinate), abs(diff_y_coordinate));
    // Get the distance between those two positions, since one is 0, 
    //   other one is positive, use this formula.

    Coordinate one_step{diff_y_coordinate / distance, 
                        diff_x_coordinate / distance};
    // Get one step direction to reach the end position

    Coordinate current_posi = begin;
    for (int i = 1; i < distance; i++) {
        current_posi.row += one_step.row;
        current_posi.col += one_step.col;
        Chess *tmp_chess = tmp_grid[current_posi.row]
                                   [current_posi.col].getChess();
        if (tmp_chess != nullptr) {
            // If it has obstacle, return false
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
    if (tmp_king->isChecked() != 0) {
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

vector<Coordinate> Rook::validMoves (const Board &theBoard) const {
    vector<Coordinate> result_moves; // The result vector
    vector<vector<int>> directions = {
        {-1,  0}, 
        { 1,  0}, 
        { 0, -1},
        { 0,  1}
    }; // The direction vector
    Coordinate original_posi = this->getCoordinate();
    // The mock position

    for (int i = 0; i < directions.size(); i++) {
        // Go four ways, and find out all the valid moves
        Coordinate mock_posi = original_posi;
        mock_posi.row += directions[i][0];
        mock_posi.col += directions[i][1];
        // Go one step first
        
        for (int j = 0; j < theBoard.getSideLength; j++) {
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
    // Give back the answer
    return result_moves;
}

void Rook::update() {}

void Rook::updateMoved() {}


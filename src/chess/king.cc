#include "chess/king.h"

using namespace std;

King::King(Color color, Square *theSquare, bool isMoved): 
    Chess{color, ChessType::King, theSquare}, isMoved{isMoved} {}

bool King::isValidMove(Board &theBoard, Coordinate begin, 
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
    
    if (!(abs(diff_x_coordinate) <= 1 && abs(diff_y_coordinate) <= 1)) {
        // If the king goes 2 steps or more in either x or y coordinate, 
        //   it's invalid, give false back.
        return false;
    } else if (abs(diff_x_coordinate) == 2 && diff_y_coordinate == 0) {
        // The castling situation haha!

        // If the king is already underattacked, it's invalid, return false
        if (this->isChecked(theBoard) != 0) return false;

        Coordinate mock_rook_posi = begin;
        Chess *tmp_chess; // The Rook doing the castling
        // The position of the rook
        if (this->getColor() == Color::BLACK) {
            // If the castling color is black
            if (diff_x_coordinate > 0) {
                // If the king is going right
                tmp_chess = tmp_grid[end.row][end.col + 1].getChess();
                // Then get the black rook on the right
                // If the chess does not exist or the chess is not rook, 
                //   give back false.
                if (tmp_chess == nullptr || 
                    tmp_chess->getType() != ChessType::Rook) return false;

                const Rook *tmp_rook = tmp_chess->getRook();

                // If the king is moved or the rook is moved, then 
                //   the castling is also invalid, give back false.
                if (isMoved == true || tmp_rook->getIsMovedStatus() == true) {
                    return false;
                }

                for (int i = 1; i < 3; i++) {
                    Coordinate mock_king_way{begin.row, begin.col + i};
                    theBoard.moveAnyway(begin, mock_king_way);
                    if (this->isChecked(theBoard) != 0) {
                        theBoard.backOneStep();
                        return false;
                    }
                    theBoard.backOneStep();
                }
            } else {
                
            }
        }
    }

    const vector<vector<Square>> &tmp_grid = theBoard.getGrid();
    // Get the grid reference


    // Check if the ending point is the friend chess, if is, then give false 
    //   back.
    Chess *tmp_chess = tmp_grid[end.row][end.col].getChess();
    if (tmp_chess != nullptr && tmp_chess->getColor() == this->getColor()) 
        return false;

    
    // If it goes to here, then the king's move would be valid if it is
}


void King::update() {}

void King::updateMoved() { isMoved = true; }





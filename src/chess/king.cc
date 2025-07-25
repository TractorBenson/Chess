#include <algorithm>
#include "chess/king.h"
#include "chess/chess.h"
#include "board/board.h"

using namespace std;

King::King(Color color, Square *theSquare, bool isMoved): 
    Chess{color, ChessType::King, theSquare, isMoved} {}

bool King::isValidMove(Board &theBoard, Coordinate begin, 
                       Coordinate end) {
    if (begin.col < 0 || begin.col >= theBoard.getSideLength() || 
        begin.row < 0 || begin.row >= theBoard.getSideLength() || 
        end.col < 0 || end.col >= theBoard.getSideLength() ||
        end.row < 0 || end.row >= theBoard.getSideLength()) {
        // If it is out of bound, return false
        cout << "false 0000000000000000000000000" << endl;
        return false;
    }

    if (begin.col == end.col && begin.row == end.row) return false;

    int diff_x_coordinate = end.col - begin.col;
    // The distance of x-coordinate from begin's position to end's position

    int diff_y_coordinate = end.row - begin.row;
    // The distance of y-coordinate from begin's position to end's position

    const vector<vector<Square>> &tmp_grid = theBoard.getGrid();
    // Get the grid reference

    // Check if the ending point is the friend chess, if is, then give false 
    //   back.
    Chess *tmp_chess = tmp_grid[end.row][end.col].getChess();
    if (tmp_chess != nullptr && tmp_chess->getColor() == this->getColor()) {
        cout << "false 1" << endl;
        return false;
    }
    
    if (abs(diff_x_coordinate) <= 1 && abs(diff_y_coordinate) <= 1) {
        // If the king goes 2 steps or more in either x or y coordinate, 
        //   it's invalid, give false back.
        
        // Check if the ending point will make the king be under attacked
        theBoard.testMove(begin, end, this->getColor());
        // Mock that position after moving
        if (this->isChecked(theBoard) != 0) {
            // If the king will be underattacked, go back first, and 
            //   give back false since it is invalid.
            theBoard.redoLastStep();
            cout << "false 3" << endl;
            return false;
        }
        // If it's fine, remember to go back
        theBoard.redoLastStep();
        cout << "true 1" << endl;
        return true;
    } else if (abs(diff_x_coordinate) == 2 && diff_y_coordinate == 0) {
        // The castling situation haha!

        // If the king is already underattacked, it's invalid, return false
        if (this->isChecked(theBoard) != 0) {
            cout << "false 4" << endl;
            return false;
        }

        // If the king is already moved, it's also invalid, return false!
        if (isMoved == true) {
            cout << "false 5" << endl;
            return false;
        }

        // Get the direction king is going to move
        const int x_one_step = diff_x_coordinate / abs(diff_x_coordinate);

        Coordinate mock_rook_posi = begin; // The mock rook position

        Chess *tmp_chess; // The temporary chess

        if (diff_x_coordinate > 0) {
            // If the king is going to the right

            // Then get the rook on the right (3 steps away)
            tmp_chess = tmp_grid[end.row][end.col + 1].getChess();
            
        } else {
            // If the king is going to the left

            // Then the the rook on the left (4 steps away)
            tmp_chess = tmp_grid[end.row][end.col - 2].getChess();
        }

        // If the chess does not exist or the chess is not the friend rook, 
        //   give back false.
        if (tmp_chess == nullptr || 
            tmp_chess->getType() != ChessType::Rook || 
            tmp_chess->getColor() != this->getColor()) {
                cout << "false 6" << endl;
                return false;
        }

        // If the rook is moved, then the castling is also invalid, 
        //   give back false.
        if (tmp_chess->getIsMovedStatus() == true) {
            cout << "false 7" << endl;
            return false;
        }

        // The absolute distance between king and rook
        const int distance = abs(tmp_chess->getCoordinate().col - begin.col);

        for (int i = 1; i < distance; i++)
        {
            // To loop the position between the king and the rook, check if
            //   there are any obstacles.

            Coordinate mock_king_way{begin.row, begin.col + i * x_one_step};
            // Get the mock position

            // Get the chess information on that square
            tmp_chess = tmp_grid[mock_king_way.row]
                                [mock_king_way.col].getChess();

            if (tmp_chess != nullptr) {
                // If there is some obstacles on the way, it's invalid, false
                cout << "false 8" << endl;
                return false;
            }
        }

        // Check whether the the way king is going to go through will be 
        //   under attacked.

        Coordinate mock_king_way{begin.row, begin.col + x_one_step};
        // Get the mock position of the expect rook position after castling

        theBoard.testMove(begin, mock_king_way, this->getColor());
        // Mock that position after castling
        if (this->isChecked(theBoard) != 0) {
            // If the king will be underattacked, go back first, and 
            //   give back false since it is invalid.
            theBoard.redoLastStep();
            cout << "false 9" << endl;
            return false;
        }
        // If it's fine, remember to go back
        theBoard.redoLastStep();


        // Check if the ending point will make the king be under attacked
        theBoard.testMove(begin, end, this->getColor());
        // Mock that position after moving
        if (this->isChecked(theBoard) != 0) {
            // If the king will be underattacked, go back first, and 
            //   give back false since it is invalid.
            theBoard.redoLastStep();
            cout << "false 10" << endl;
            return false;
        }
        // If it's fine, remember to go back
        theBoard.redoLastStep();

        // If all the conditions are satisfied, give back true
        cout << "true 2" << endl;
        return true;

        // The following will be the normal moves!
    } else {
        // If it goes to here, the move will be invalid
        cout << "false 11" << endl;
        return false;
    }
    
    // If it goes to here, then the king's move would be invalid
    cout << "false 12" << endl;
    return false;
}

vector<Coordinate> King::validMoves (Board &theBoard) {
    vector<Coordinate> result_moves;
    vector<vector<int>> directions = {
        { 0,   2}, 
        { 0,  -2}, 
        { 0,   1}, 
        { 0,  -1}, 
        {-1,   1}, 
        {-1,   0}, 
        {-1,  -1}, 
        { 1,   1}, 
        { 1,   0}, 
        { 1,  -1}
    };
    Coordinate original_posi = this->getCoordinate();
    for (int i = 0; i < directions.size(); i++) {
        Coordinate mock_posi = original_posi;
        mock_posi.row += directions[i][0];
        mock_posi.col += directions[i][1];
        if (this->isValidMove(theBoard, original_posi, 
                              mock_posi)) {
            result_moves.emplace_back(mock_posi);
        }
    }
    return result_moves;
}

int King::isChecked (const Board &theBoard) const {
    
    // Get the grid reference
    const vector<vector<Square>> &tmp_grid = theBoard.getGrid();

    Chess *tmp_chess;
    int attacking_num = 0;
    // First, check if the knight is attacking the king
    vector<Coordinate> knight_dir = {
        Coordinate{  2,  1 },
        Coordinate{  1,  2 },
        Coordinate{ -1,  2 },
        Coordinate{ -2,  1 },
        Coordinate{ -2, -1 },
        Coordinate{ -1, -2 },
        Coordinate{  1, -2 },
        Coordinate{  2, -1 }
    };

    for (int i = 0; i < knight_dir.size(); i++) {
        Coordinate mock_knight_posi = this->getCoordinate();
        mock_knight_posi.row += knight_dir[i].row;
        mock_knight_posi.col += knight_dir[i].col;
        if (mock_knight_posi.col < 0 || 
            mock_knight_posi.col >= theBoard.getSideLength() ||
            mock_knight_posi.row < 0 || 
            mock_knight_posi.row >= theBoard.getSideLength()) {
                continue;
        }
        tmp_chess = tmp_grid[mock_knight_posi.row]
                            [mock_knight_posi.col].getChess();
        if (tmp_chess != nullptr && 
            tmp_chess->getColor() != this->getColor() &&
            tmp_chess->getType() == ChessType::Knight) {
                attacking_num++;
        }
    }

    // Secondly, check the pawns
    vector<Coordinate> pawn_dir;
    if (this->getColor() == Color::BLACK) {
        // If the king is black
        pawn_dir.emplace_back(Coordinate{-1, -1});
        pawn_dir.emplace_back(Coordinate{-1,  1});
    } else {
        // Else, the king is white
        pawn_dir.emplace_back(Coordinate{1, -1});
        pawn_dir.emplace_back(Coordinate{1,  1});
    }
    for (int i = 0; i < pawn_dir.size(); i++) {
        Coordinate mock_pawn_posi = this->getCoordinate();
        mock_pawn_posi.row += pawn_dir[i].row;
        mock_pawn_posi.col += pawn_dir[i].col;
        if (mock_pawn_posi.col < 0 || 
            mock_pawn_posi.col >= theBoard.getSideLength() ||
            mock_pawn_posi.row < 0 || 
            mock_pawn_posi.row >= theBoard.getSideLength()) {
                continue;
        }
        tmp_chess = tmp_grid[mock_pawn_posi.row]
                            [mock_pawn_posi.col].getChess();
        if (tmp_chess != nullptr && 
            tmp_chess->getColor() != this->getColor() &&
            tmp_chess->getType() == ChessType::Pawn) {
                attacking_num++;
        }
    }

    // Thirdly, check the diagnals, which are the queens and bishops
    vector <Coordinate> diag_dir = {
        Coordinate{ 1,  1},
        Coordinate{ 1, -1},
        Coordinate{-1,  1},
        Coordinate{-1, -1}
    };
    for (int i = 0; i < diag_dir.size(); i++) {
        Coordinate mock_diag_posi = this->getCoordinate();
        while (true) {
            mock_diag_posi.row += diag_dir[i].row;
            mock_diag_posi.col += diag_dir[i].col;
            if (mock_diag_posi.col < 0 || 
                mock_diag_posi.col >= theBoard.getSideLength() ||
                mock_diag_posi.row < 0 || 
                mock_diag_posi.row >= theBoard.getSideLength()) {
                    break;
            }
            tmp_chess = tmp_grid[mock_diag_posi.row]
                                [mock_diag_posi.col].getChess();
            if (tmp_chess != nullptr) { 
                if (tmp_chess->getColor() == this->getColor()) break;
                else if (tmp_chess->getType() == ChessType::Queen || 
                         tmp_chess->getType() == ChessType::Bishop) {
                    attacking_num++;
                    break;
                }
                break;
            }
        }
    }

    // Fourthly, check the horizontal/vertical lines, which are
    //   queens and rooks.
    vector <Coordinate> sline_dir = {
        Coordinate{ 1,  0},
        Coordinate{-1,  0},
        Coordinate{ 0,  1},
        Coordinate{ 0, -1}
    };
    for (int i = 0; i < sline_dir.size(); i++) {
        Coordinate mock_sline_posi = this->getCoordinate();
        while (true) {
            mock_sline_posi.row += sline_dir[i].row;
            mock_sline_posi.col += sline_dir[i].col;
            if (mock_sline_posi.col < 0 || 
                mock_sline_posi.col >= theBoard.getSideLength() ||
                mock_sline_posi.row < 0 || 
                mock_sline_posi.row >= theBoard.getSideLength()) {
                    break;
            }
            tmp_chess = tmp_grid[mock_sline_posi.row]
                                [mock_sline_posi.col].getChess();
            if (tmp_chess != nullptr) {
                if (tmp_chess->getColor() == this->getColor()) break;
                else if (tmp_chess->getType() == ChessType::Queen || 
                         tmp_chess->getType() == ChessType::Rook) {
                    attacking_num++;
                    break;
                }
                break;
            }
        }
    }

    // Lastly, check the king cannot see king (Setup mode)
    for (int lrow=-1; lrow<=1; ++lrow)
        for (int lcol=-1; lcol<=1; ++lcol)
            if (!(lrow == 0 && lcol == 0)){
                Coordinate mock_king_posi;
                mock_king_posi.row = this->getCoordinate().row + lrow;
                mock_king_posi.col = this->getCoordinate().col + lcol;
                if (mock_king_posi.col < 0 || 
                    mock_king_posi.col >= theBoard.getSideLength() ||
                    mock_king_posi.row < 0 || 
                    mock_king_posi.col >= theBoard.getSideLength()) {
                        continue;
                }
                Chess *mock_king = tmp_grid[mock_king_posi.row]
                                           [mock_king_posi.col].getChess();
                if (mock_king == nullptr) {
                    continue;
                }
                if (mock_king->getType() == ChessType::King &&
                    mock_king->getColor() != this->getColor()) {
                    attacking_num++;
                    return attacking_num;
                }
            }

    return attacking_num;
}

bool King::isCheckmated (Board &theBoard) {
    return (this->isChecked(theBoard) && 
            theBoard.noValidMoves(this->getColor()));
}

void King::update() {}

void King::updateMoved() { isMoved = true; }





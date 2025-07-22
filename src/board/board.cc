#include "../../include/board/board.h"
#include "../../include/chess/pawn.h"

using namespace std;

    const vector<vector<Square>>& Board::getGrid() const {
        return grid;
    }
    size_t Board::getSideLength() const {
        return sideLength;
    }
    const vector<unique_ptr<Chess>>& Board::getWhiteChesses() const {
        return whiteChesses;
    }
    const vector<unique_ptr<Chess>>& Board::getBlackChesses() const {
        return blackChesses;
    }

    // checkDraw() returns true if any player ever has no legal 
    //   moves available, but is not in check. Otherwise, return false.
    // This function is used at the beginning of a new turn.
    bool Board::checkDraw(Color currentPlayer) const {
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                // current square has to have a chess on it
                
                    // Check player in turn only
                    if (grid[i][j].getChess()->getColor() == currentPlayer) {
                        // Check through all squares again
                        for (int begin = 0; begin < 8; ++begin) {
                            for (int end = 0; end < 8; ++end) {
                                // current square has to have a valid chess
                                if (grid[i][j].getChess()) {
                                    if (grid[i][j].chess->
                                        isValidMove(*this, begin, end)) {
                                            return true;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            return false;
        }
    // Set all pawn's canBeImpasant field to be flase.
    //  This is done at the beginning of every turn of currentPlayer
    void Board::updateChess(Color currentPlayer) {
        if (currentPlayer == Color::WHITE) {
            for (auto& chess : whiteChesses) {
               if() 
            }
        } else {
            for (auto& chess : blackChesses) {

            }
        }
    }

    Board*

    void initSquares(); // initialize the board before game starts

    // Place a chess of certain type on loc with certain color
    void placeChess(Coordinate loc, char type, Color color);

    // Remove existing chess on loc, do nothing if there's no chess on loc
    void removeChess(Coordinate loc);
    // place all chesses with default layout before gamestart
    void initChessesWithDefaultArrange();
    // Checks if there are two kings, each in different colors
    bool isValidSetup();
    // Move the chess on location begin to end
    bool moveChess(Coordinate begin, Coordinate end);
    // Determine if a check exist in current step
    bool isCheck() const;
    // Determine if a checkmate exist in current step
    bool isCheckmate() const;

ostream &operator<<(ostream &out, const Grid &g);
#endif

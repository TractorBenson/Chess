#include "../include/board/board.h"
#include "../include/chess/pawn.h"
#include "../include/chess/king.h"
#include "../include/chess/bishop.h"
#include "../include/chess/knight.h"
#include "../include/chess/queen.h"
#include "../include/chess/rook.h"

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

// Set all pawn's canBeImpasant field to false.
//   This is done at the beginning of every game turn of currentPlayer
void Board::updateChess(Color currentPlayer) {
    if (currentPlayer == Color::WHITE) {
        // loop through all white chesses and call update(),
        // Note that update() is only implemented on Pawns
        for (auto& chess : whiteChesses) {
        //  if(chess->getType() == ChessType::Pawn) {
            chess->update();
            //}
        }
    } else {
        // loop through all black chesses
        for (auto& chess : blackChesses) {
            if(chess->getType() == ChessType::Pawn) {
                chess->update();
            }
        }
    }
}

// Return the number of chesses of type in color. Used to check 
//   if a type of chess has its maximum number allowed on one side
int Board::numOfChesses(ChessType type, Color color) const{
    int count = 0;
    if (currentPlayer == Color::WHITE) {
        // loop through all white chesses
        for (auto& chess : whiteChesses) {
            if(chess->getType() == type) {
                ++count;
            }
        }
    } else {
        // loop through all black chesses
        for (auto& chess : blackChesses) {
            if(chess->getType() == type) {
                ++count;
            }
        }
    }
    return count;
}

// Place a chess of certain type on loc with certain color, used in setup mode
//   or when initilizing a default board. Main function is responsible for 
//   ensuring the location is valid to put a chess of certain color and type.
void Board::placeChess(Coordinate loc, char type, Color color) {

    std::unique_ptr<Chess> newChess;
    Square& sq = grid[loc.row][loc.col];
    

    switch (type) {
    case 'K':
        newChess = std::make_unique<King>
            (color, ChessType::King, &sq, false);
        break;
    case 'Q':
        newChess = std::make_unique
            <Queen>(color, ChessType::Queen, &sq);
        break;
    case 'B': 
        newChess = std::make_unique
            <Bishop>(color, ChessType::Bishop, &sq);
        break;
    case 'R':
        newChess = std::make_unique
            <Rook>(color, ChessType::Rook, &sq);
        break;
    case 'N':
        newChess = std::make_unique
            <Knight>(color, ChessType::Knight, &sq);
        break;
    case 'P': 
        newChess = std::make_unique
            <Pawn>(color, ChessType::Pawn, &sq, false, false);
        break;
    default:
        throw std::invalid_argument{"unknown chess type"};
    }

    // Add the newly added chess to the corresponding chess vector,
    //   if the chess is a King, update the King pointers, too.
    if (color == Color::BLACK) {
        if (newChess->getType() == ChessType::King) {
            blackKing = newChess.get();
        }
        blackChesses.emplace_back(std::move(newChess));
    } else {
        if (newChess->getType() == ChessType::King) {
            whiteKing = newChess.get();
        }
        whiteChesses.emplace_back(std::move(newChess))
    }
    sq.setChess(newChess.get());
}

// Remove existing chess on loc, do nothing if there's no chess on loc
void Board::removeChess(Coordinate loc) {
    Square& sq = grid[loc.row][loc.col];
    Chess* piece = sq.getChess();
    // Do nothing if the location has no chess on it
    if(!piece) return;
    if(piece == whiteKing) {
        whiteKing = nullptr;
    }
    if (piece == blackKing) {
        blackKing = nullptr
    }
    sq.removeChess();
    
    // Figure out which game side is the chess on
    auto& ownerVec = (piece->getColor() == Color::WHITE) ? 
        whiteChesses : blackChesses;
    
    // Find the actual unique_ptr of the chess to be removed
    auto uniqueChess = ownerVec.end();
    for (auto iter = ownerVec.begin(); iter != ownerVec.end(); ++iter) {
        if (iter->get() == piece) {
            uniqueChess = iter;
        }
    }
    // If the chess is actually found in chess vector of its color
    if (uniqueChess != ownerVec.end()) ownerVec.erase(uniqueChess);

    sq.notifyDisplayer();
}

// place all chesses with default layout before gamestart
void Board::initChessesWithDefaultArrange();


bool Board::isValidSetup() {
    // Checks if there are two kings, each in different colors
    if (numOfChesses(ChessType::King, Color::BLACK) != 1 || 
        numOfChesses(ChessType::King, Color::WHITE) != 1) {
        return false;
    }
    // Two kings should not be in check
    if ((*whiteKing).isChecked(grid, whiteChesses, blackChesses) || 
        (*blackKing).isChecked(grid, whiteChesses, blackChesses)) {
        return false;
    }
    // No pawns should be on the first or last row of the board
    for (size_t row = 0; row < sideLength; row += (sideLength - 1)) {
        for (size_t col = 0; col < sideLength; ++col) {
            if (grid[row][col].getChess() && 
                grid[row][col].getChess()->getType() == ChessType::Pawn) {
                    return false;
            }
        }
    }
    return true;
}
// Move the chess from begin coordinate to end coordinate
bool Board::moveChess(Coordinate begin, Coordinate end) {
    if (grid[begin.row][begin.col].getChess()->getColor() == Color::NOTHING) [
        cout << "There's no "
    ]
}
// Determine if a check exist in current step
bool Bpard::isCheck() const;
// Determine if a checkmate exist in current step
bool Board::isCheckmate() const {
    
}

ostream &operator<<(ostream &out, const Grid &g) {

}

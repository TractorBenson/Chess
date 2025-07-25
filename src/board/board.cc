#include "board/board.h"
#include "chess/pawn.h"
#include "chess/king.h"
#include "chess/bishop.h"
#include "chess/knight.h"
#include "chess/queen.h"
#include "chess/rook.h"
#include "observer/textDisplay.h"
#include "observer/graphDisplay.h"

using namespace std;

Board::Board() : grid(sideLength, vector<Square>(sideLength)) {

    initBoard();
    whiteChesses.reserve(16);
    blackChesses.reserve(16);

    obs.reserve(2);
    obs.emplace_back(make_unique<TextDisplay>(sideLength));
    //obs.emplace_back(make_unique<GraphDisplay>(sideLength));

    // Attach observer to all squares.
    for (auto& row : grid) {
        for (auto& sq : row) {
                sq.attachObserver(obs);     // raw Observer*
                sq.notifyDisplayer();
        }
    }
    for (auto& row : grid) {
        for (auto& sq : row) {
                sq.notifyDisplayer();     // raw Observer*
        }
    }
}

// const Square& Board::getSquare(Coordinate coord) const {
//     return grid[coord.row][coord.col];
// }
bool Board::squareIsEmpty(Coordinate coord) const {
    return grid[coord.row][coord.col].getChess() == nullptr;
}

const vector<vector<Square>>& Board::getGrid() const {
    return grid;
}
size_t Board::getSideLength() const {
    return sideLength;
}
Color Board::getChessColor(Coordinate coord) const {
    return grid[coord.row][coord.col].getChess()->getColor();
}
const vector<unique_ptr<Chess>>& Board::getWhiteChesses() const {
    return whiteChesses;
}
const vector<unique_ptr<Chess>>& Board::getBlackChesses() const {
    return blackChesses;
}
ChessType Board::getChessType(Coordinate coord) const{
    return grid[coord.row][coord.col].getChess()->getType();
}

King* Board::getBlackKing() const {
    return blackKing;
}

King* Board::getWhiteKing() const{
    return whiteKing;
}
// checkDraw() returns true if any player ever has no legal 
//   moves available, but is not in check. Otherwise, return false.
// This function is used at the beginning of a new turn.
bool Board::noValidMoves(Color currentPlayer) {
    
    const vector<unique_ptr<Chess>> &chesses = (currentPlayer == 
        Color::BLACK) ? blackChesses : whiteChesses;

    for (const auto& piece : chesses){
        // Check through all chesses on currentPlayer's side, 
        //  if there are at least one valid move, the game is not draw
        if (!piece->validMoves(*this).empty()) {
            return false;
        }
    }
    return true;
}

// Set all pawn's canBeImpasant field to false.
//   This is done at the beginning of every game turn of currentPlayer
void Board::updateChess(Color currentPlayer) {
    if (currentPlayer == Color::WHITE) {
        // loop through all white chesses and call update(),
        // Note that update() is only implemented on Pawns
        for (auto& chess : whiteChesses) {
            chess->update();
        }
    } else {
        // loop through all black chesses
        for (auto& chess : blackChesses) {
                chess->update();
        }
    }
}

// Return the number of chesses of type in color. Used to check 
//   if a type of chess has its maximum number allowed on one side
int Board::numOfChesses(ChessType type, Color color) const{
    int count = 0;
    if ( color == Color::WHITE) {
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
void Board::placeChess(Coordinate loc, char type) {

    std::unique_ptr<Chess> newChess;
    Square& sq = grid[loc.row][loc.col];

    switch (type) {
    case 'K':
        newChess = std::make_unique<King>
            (Color::WHITE, &sq, false);
        break;
     case 'k':
        newChess = std::make_unique<King>
            (Color::BLACK, &sq, false);
        break;
    case 'Q':
        newChess = std::make_unique
            <Queen>(Color::WHITE, &sq, false);
        break;
    case 'q':
        newChess = std::make_unique
            <Queen>(Color::BLACK, &sq, false);
        break;
    case 'B': 
        newChess = std::make_unique
            <Bishop>(Color::WHITE, &sq, false);
        break;
    case 'b': 
        newChess = std::make_unique
            <Bishop>(Color::BLACK, &sq, false);
        break;
    case 'R':
        newChess = std::make_unique
            <Rook>(Color::WHITE, &sq, false);
        break;
    case 'r':
        newChess = std::make_unique
            <Rook>(Color::BLACK, &sq, false);
        break;
    case 'N':
        newChess = std::make_unique
            <Knight>(Color::WHITE, &sq, false);
        break;
    case 'n':
        newChess = std::make_unique
            <Knight>(Color::BLACK, &sq, false);
        break;
    case 'P': 
        newChess = std::make_unique
            <Pawn>(Color::WHITE, &sq, false, false);
        break;
    case 'p': 
        newChess = std::make_unique
            <Pawn>(Color::BLACK, &sq, false, false);
        break;
    default:
        throw std::invalid_argument{"unknown chess type"};
    }

    // Add the newly added chess to the corresponding chess vector,
    //   if the chess is a King, update the King pointers, too.
    if (type == 'k') {
        blackKing = static_cast<King*>(newChess.get());
        blackChesses.emplace_back(std::move(newChess));
    }
    if (type == 'K') {
        whiteKing = static_cast<King*>(newChess.get());
        whiteChesses.emplace_back(std::move(newChess));
    }
    sq.setChess(newChess.get());
    sq.notifyDisplayer();
}

// Remove existing chess on loc, do nothing if there's no chess on loc
void Board::removeChess(Coordinate loc) {
    Square& sq = grid[loc.row][loc.col];
    Chess* piece = sq.getChess();
    // Do nothing if the location has no chess on it
    if(!piece) return;
    // Detach king pointers (although this might never happen)
    if(piece == whiteKing) {
        whiteKing = nullptr;
    }
    if (piece == blackKing) {
        blackKing = nullptr;
    }
    // Detach the chess with the square
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
    //  Remove chess from the vector of unique pointers
    if (uniqueChess != ownerVec.end()) ownerVec.erase(uniqueChess);

    sq.notifyDisplayer();
}

void Board::initBoard() {
    Color currentColor = Color::BLACK;

    for (size_t row = 0; row < sideLength; ++row) {
        for (size_t col = 0; col < sideLength; ++col) {
            // set fields of all squares
            Coordinate coord = Coordinate{row, col};
            grid[row][col].setCoordinate(row, col);
            grid[row][col].setColor(currentColor);
            if (currentColor == Color::BLACK) {
                currentColor = Color::WHITE;
            } else {
                currentColor = Color::BLACK;
            }
        }
        if (currentColor == Color::BLACK) {
                currentColor = Color::WHITE;
            } else {
                currentColor = Color::BLACK;
            }
    }       
}

// place all chesses with default layout before gamestart.
//   The chess fields are left as nullptr, while observers are
//   attached to the squares.
void Board::initChessesWithDefaultArrange() {
    Color currentColor = Color::BLACK;

    cout << "Grid has bound: " << grid.size() << endl;

    for (size_t row = 0; row < sideLength; ++row) {
        for (size_t col = 0; col < sideLength; ++col) {
            // set fields of all squares
            Coordinate coord = Coordinate{row, col};
            
            // place white chesses to their default position
            if (row == 0 && (col == 0 || col == sideLength - 1)) {
                placeChess(coord, 'R');
            } else if (row == 0 && (col == 1 || col == sideLength - 2)) {
                placeChess(coord, 'N');
            } else if (row == 0 && (col == 2 || col == sideLength - 3)) {
                placeChess(coord, 'B');
            } else if (row == 0 && col == 3) {
                placeChess(coord, 'Q');
            } else if (row == 0 && col == 4) {
                placeChess(coord, 'K');
            }

            cout << "2222222222222" << endl;

            // Place all black chesses to their default posistion
            if (row == 7 && (col == 0 || col == sideLength - 1)) {
                placeChess(coord, 'r');
            }else if (row == 7 && (col == 1 || col == sideLength - 2)) {
                placeChess(coord, 'n');
            } else if (row == 7 && (col == 2 || col == sideLength - 3)) {
                placeChess(coord, 'b');
            } else if (row == 7 && col == 3) {
                placeChess(coord, 'q');
            } else if (row == 7 && col == 4) {
                placeChess(coord, 'k');
            } else if (row == 1) {
                placeChess(coord, 'P');
            } else if (row == 6) {
                placeChess(coord, 'p');
            }
            grid[row][col].notifyDisplayer();
            if (currentColor == Color::BLACK) {
                currentColor = Color::WHITE;
            } else {
                currentColor = Color::BLACK;
            }

            cout << "3333333333333" << endl;
        }
        if (currentColor == Color::BLACK) {
            currentColor = Color::WHITE;
        } else {
            currentColor = Color::BLACK;
        }
    }
}


bool Board::isValidSetup() {
    // Checks if there are two kings, each in different colors
    if (numOfChesses(ChessType::King, Color::BLACK) != 1 || 
        numOfChesses(ChessType::King, Color::WHITE) != 1) {
        cout << "Invalid Setup! Does not have one white " <<
            "king and one black king." << endl;
        return false;
    }
    // Two kings should not be in check
    if ((*whiteKing).isChecked(*this) || 
        (*blackKing).isChecked(*this)) {
        cout << "Invalid Setup! At least one King is in check." << endl;
        return false;
    }
    // No pawns should be on the first or last row of the board
    for (size_t row = 0; row < sideLength; row += (sideLength - 1)) {
        for (size_t col = 0; col < sideLength; ++col) {
            if (grid[row][col].getChess() && 
                grid[row][col].getChess()->getType() == ChessType::Pawn) {
                    cout << "Invalid Setup! A pawn is at the " <<
                        "first or last row." << endl;
                    return false;
            }
        }
    }
    return true;
}

// checks if the chess is a pawn and can be promoted in current round
//   This is called after using moveChess(begin, end), which means the move is
//   valid, the method only checks if the chess is a pawn and can be promoted
bool Board::canPromote(Coordinate begin, Coordinate end) {
    // coordinate begin is ensured valid during input stage
    Square &sq = grid[begin.row][begin.col];
    Chess* piece = sq.getChess();
    if (piece->getType() != ChessType::Pawn) {
        return false;
    }
    // After the move being valid, check if the pawn is at bottom lines
    if (piece->getColor() == Color::BLACK) {
        // Case where black Pawn reached the white bottom
        if (end.row == 0) return true;
    } else {
        // Case where white Pawn reached the black bottom
        if (end.row == (sideLength - 1))
            return true;
    }
    return false;
}

// Checks if it is valid to move the chess on begin to end,
//   return true if the move is valid, else return false.
bool Board::moveChess(Coordinate begin, Coordinate end) {
    Square &sq = grid[begin.row][begin.col];
    if (!sq.getChess()) {
        cout << "Invalid move! There's no chess on the " << 
            "beginning location you slected!" << endl;
        return false;
    }

    if (sq.getChess()->isValidMove(*this, begin, end)) {
        return true;
    }
    cout << "Move is invalid! Please input your commamd again." << endl;
    return false;
}


// Used to check if the move will make currentPlayer's king in check.
//    (It does not check the king itself). It attempts to move chess 
//   from begin to end, but stores the original location of the chess
//    and any chess potentialy being taken. It allows for
//   redoing the attempt move later. Assume begin and end are valid
//   coordinate for a move.
void Board::testMove(Coordinate begin, Coordinate end, Color CurrentPlayer) {
    Square* from = &grid[begin.row][begin.col];
    Square* to = &grid[end.row][end.col];
    Chess* movedC = from->getChess();
    Chess* capturedC = to->getChess();
    King* movedKing = nullptr;

    // When we are moving a King, connect the point as well
    if (movedC->getType() == ChessType::King) {
        movedKing = (CurrentPlayer == Color::BLACK) ? blackKing : whiteKing;
    }

    lastTry = MoveBackup{from, to, movedC, capturedC, movedKing};

    // Actaully move the chess from begin to end
    to->setChess(movedC);
    from->setChess(nullptr);

    movedC->setSquare(to);

    // if the destination has a chess, it is captured,
    //   remove the chess
    if (capturedC) capturedC->setSquare(nullptr);

    if (movedC->getType() == ChessType::King) {
        if(CurrentPlayer == Color::BLACK) {
            blackKing = static_cast<King*>(movedC);
        } else {
            whiteKing = static_cast<King*>(movedC);
        }
    }
}
// move chess at begin to end, no matter what, assume the move is already valid
void Board::simpleMove(Coordinate begin, Coordinate end){
    Square* from = &(grid[begin.row][begin.col]);
    Square* to = &(grid[end.row][end.col]);
    Chess* movedC = from->getChess();
        // Actaully move the chess from begin to end

        
    int diff_x = end.col - begin.col;
    int diff_y = end.row - begin.row;

    if (movedC->getType() == ChessType::King) {
        // Castling case ahha!
        if (diff_x == 2 && diff_y == 0) {
            Square *future_rook = &(grid[end.row][end.col - 1]);
            Square *original_rook = &(grid[end.row][end.col + 1]);
            future_rook->setChess(original_rook->getChess());
            original_rook->setChess(nullptr);
            grid[end.row][end.col - 1].notifyDisplayer();
            grid[end.row][end.col + 1].notifyDisplayer();
        } else if (diff_x == -2 && diff_y == 0) {
            Square *future_rook = &(grid[end.row][end.col + 1]);
            Square *original_rook = &(grid[end.row][end.col - 2]);
            future_rook->setChess(original_rook->getChess());
            original_rook->setChess(nullptr);
            grid[end.row][end.col - 2].notifyDisplayer();
            grid[end.row][end.col + 1].notifyDisplayer();
        }
    } else if (movedC->getType() == ChessType::Pawn) {
        if (abs(diff_x) == 1 && abs(diff_y) == 1 && to->getChess() == nullptr)
        {
            // The En Passant situation haha!
            Square *pawnBeEaten = &(grid[begin.row][end.col]);
            pawnBeEaten->setChess(nullptr);
        }
    }

    to->setChess(movedC);
    from->setChess(nullptr);

    movedC->setSquare(to);
    grid[begin.row][begin.col].notifyDisplayer();
    grid[end.row][end.col].notifyDisplayer();
}

void Board::resetLT() {
    lastTry.reset();
}

// 
void Board::redoLastStep() {
    if (!lastTry) return; // nothing to undo

    const MoveBackup& m = *lastTry;

    // Put moved and captured chess back;
    m.from->setChess(m.movedC);
    m.to->setChess(m.capturedC);
    m.movedC->setSquare(m.from);
    if (m.capturedC) m.capturedC->setSquare(m.to);

    // update king pointers if king were moved
    if (m.movedC->getType() == ChessType::King) {
        if (m.movedC->getColor() == Color::BLACK) {
            blackKing = m.movedKing;
        } else {
            whiteKing = m.movedKing;
        }
    }
    
    lastTry.reset();    // discard the backup
}


// Find if the opposite side is checked. This method is called
//   after a valid move of currentPlayer
bool Board::isCheck(Color currentPlayer) const {
    if (currentPlayer == Color::WHITE) {
        if (blackKing->isChecked(*this)) {
            cout << "Black is in check." << endl;
            return true;
        }
    } else {
        if (whiteKing->isChecked(*this)) {
            cout << "White is in check." << endl;
            return true;
        }
    }
    return false;
}

// Find if the opposite side is checkmated. This method is called
//   after a valid move of currentPlayer and after isCheck() method.
//   If the method returns true, terminate current game immediately.
bool Board::isCheckmate(Color currentPlayer) {

    if (currentPlayer == Color::WHITE) {
        if (blackKing->isCheckmated(*this)) {
            cout << "Checkmate! White wins!" << endl;
            return true;
        }
    } else {
        if (whiteKing->isCheckmated(*this)) {
            cout << "Checkmate! Black wins!" << endl;
            return true;
        }
    }
    return false;
}

// Outputs current chess board in text form.
ostream &operator<<(ostream &out, const Board &b) {
    if (b.obs[0]) out << *(static_cast<TextDisplay*>(b.obs[0].get()));
    return out;
}

#include "bot/bot.h"
#include "chess/chess.h"
#include "enum/chesstype.h"
#include <set>
#include <memory>

using namespace std;

Bot::Bot(Board *theBoard, Color color, int level):
theBoard{theBoard}, color{color}, level{level} {
    if (color == Color::WHITE) {
        opponentKing = theBoard->getBlackKing();
    }
    else {
        opponentKing = theBoard->getWhiteKing();
    }
}

int Bot::random(int n) {
    int result = prng(n);
    while (result >= n) {
        result -= 1;
    }
    if (result < 0) result = 0;
    return result;
}

void Bot::constructMoveMap() {
    // init a Chess* vector
    vector<Chess*> myChesses;

    if (color == Color::WHITE) {
        for (const auto &up : theBoard->getWhiteChesses()) {
            myChesses.emplace_back(up.get());
        }
    }
    else {
        for (const auto &up : theBoard->getBlackChesses()) {
            myChesses.emplace_back(up.get());
        }
    }

    // add pairs in the map
    for (auto *cp : myChesses) {
        moveMap.emplace((cp->getSquare())->getCoordinate(), cp->validMoves(*theBoard));
    }
}

string Bot::convertCoordinate(Coordinate begin, Coordinate end) {
    char beginR, beginC, endR, endC;

    beginR = static_cast<char>(begin.row + '1');
    beginC = static_cast<char>(begin.col + 'a');
    endR = static_cast<char>(end.row + '1');
    endC = static_cast<char>(end.col + 'a');

    string result = "";
    result += beginC + beginR + ' ' + endC + endR;

    if ((end.row == 0 || end.row == 7) && theBoard->getGrid().at(begin.row).at(begin.col).getChess()->getType() == ChessType::Pawn) {
        result += ' ' + 'Q';
    }

    return result;
}

string Bot::move() {
    // clear moveMap for updated moves
    moveMap.clear();
    // create updated moveMap
    constructMoveMap();

    string result;

    if (level <= 1) {
        result = randomMove();
    }
    else if (level == 2) {
        result = captureFirstMove();
    }
    else if (level >= 3) {
        result = surviveFirstMove();
    }

    return result;
}

string Bot::randomMove() {
    vector<Coordinate> keys;
    for (auto & [cc, moves] : moveMap) {
        if (!moves.empty()) {
            keys.emplace_back(cc);
        }
    }

    Coordinate selectedBegin = keys.at(random(keys.size()));
    vector<Coordinate> selectedMoves = moveMap.find(selectedBegin)->second;
    Coordinate selectedEnd = selectedMoves.at(random(selectedMoves.size()));

    return convertCoordinate(selectedBegin, selectedEnd);
}

string Bot::captureFirstMove() {
    // create container for moves that capture
    map<Coordinate, vector<Coordinate>> captureMoves;
    vector<Coordinate> captureMovesKeys;

    // iterate all my chesses
    for (auto & [cc, moves] : moveMap) {
        // bool to detect if any move added, if so, add the begin coordinate to vector so can be picked randomly
        bool moveAdded = false;
        // iterate all moves for a single chess, if can capture, add to map
        for (auto move : moves) {
            // if targeting square contain a chess
            // since the move is valid, that chess must be opponent's
            // so if exist chess, this move is capture
            Chess *cp = theBoard->getGrid().at(move.row).at(move.col).getChess();
            if (cp) {
                captureMoves[cc].emplace_back(move);
                moveAdded = true;
            }
        }
        if (moveAdded) {
            captureMovesKeys.emplace_back(cc);
        }
    }

    // if there are capture move(s), go with it
    if (!captureMovesKeys.empty()) {
        Coordinate selectedBegin = captureMovesKeys.at(random(captureMovesKeys.size()));
        vector<Coordinate> selectedMoves = captureMoves.find(selectedBegin)->second;
        Coordinate selectedEnd = selectedMoves.at(random(selectedMoves.size()));
        
        return convertCoordinate(selectedBegin, selectedEnd);
    }

    // if not, try find move(s) can check
    map<Coordinate, vector<Coordinate>> checkMoves;
    vector<Coordinate> checkMovesKeys;
    // iterate all my chesses
    for (auto & [cc, moves] : moveMap) {
        // bool to detect if any move added, if so, add the begin coordinate to vector so can be picked randomly
        bool moveAdded = false;
        // iterate all moves for a single chess, if can check, add to map
        for (auto move : moves) {
            // attemp to move
            theBoard->testMove(cc, move, color);
            // after this move, if opponent's king is checked
            // i.e. this move check opponent's king
            if (opponentKing->isChecked(*theBoard)) {
                // then add this move to the map
                // and also update bool for later adding of key into keys
                checkMoves[cc].emplace_back(move);
                moveAdded = true;
            }
            // don't forget to recover the board
            theBoard->redoLastStep();
        }
        if (moveAdded) {
            checkMovesKeys.emplace_back(cc);
        }
    }
    
    // if there are check move(s), go with it
    if (!checkMovesKeys.empty()) {
        Coordinate selectedBegin = checkMovesKeys.at(random(checkMovesKeys.size()));
        vector<Coordinate> selectedMoves = checkMoves.find(selectedBegin)->second;
        Coordinate selectedEnd = selectedMoves.at(random(selectedMoves.size()));
        
        return convertCoordinate(selectedBegin, selectedEnd);
    }

    // if not, then no prefering, go with random move
    return randomMove();
}

string Bot::surviveFirstMove() {
    // get opponent's chesses raw pointers
    vector<Chess*> opponentChesses;
    if (color == Color::BLACK) {
        const auto &whiteChesses = theBoard->getWhiteChesses();
        for (const auto &up : whiteChesses) {
            opponentChesses.emplace_back(up.get());
        }
    }
    else {
        const auto &blackChesses = theBoard->getBlackChesses();
        for (const auto &up : blackChesses) {
            opponentChesses.emplace_back(up.get());
        }
    }

    // subtract all my chesses'position
    vector<Coordinate> myChessesPosition;
    for (auto & [cc, moves] : moveMap) {
        myChessesPosition.emplace_back(cc);
    }

    // subtract all reachable position of opponent's chesses
    vector<Coordinate> opponentReachablePosition;
    for (auto cp : opponentChesses) {
        for (auto move : cp->validMoves(*theBoard)) {
            opponentReachablePosition.emplace_back(move);
        }
    }

    // use set to track my chesses that is been capturing
    // and a vector to track keys to random pick
    set<Coordinate> capturedChesses;
    vector<Coordinate> capturedChessesKeys;
    // for every opponent reachable position 
    for (auto reachablePosi : opponentReachablePosition) {
        // for every my chesses's position
        for (auto myPosi : myChessesPosition) {
            // if any two are the same
            // i.e. arbitrary opponent chess can capture arbitrary my chess
            if (myPosi == reachablePosi) {
                // add my being captured chess's position to the set
                // if success (i.e. did not add before), return true to inserted
                auto [it, inserted] = capturedChesses.insert(myPosi);
                if (inserted) {
                    // so add this new key into vector
                    capturedChessesKeys.emplace_back(myPosi);
                }
            }
        }
    }

    // if exist any captured chess, go with it
    if (!capturedChessesKeys.empty()) {
        // iterate chess that is captured
        for (Coordinate selectedBegin : capturedChessesKeys) {
            // collect all moves that chess can go
            vector<Coordinate> feassibleMoves = moveMap.find(selectedBegin)->second;

            // remove all moves that end position is still been captured
            vector<Coordinate> selectedMoves;
            for (auto move : feassibleMoves) {
                bool addMove = true;
                for (auto reachablePosi : opponentReachablePosition) {
                    if (move == reachablePosi) {
                        addMove = false;
                        break;
                    }
                }
                if (addMove) {
                    selectedMoves.emplace_back(move);
                }
            }

            // if such move(s) exist, go with it
            if (!selectedMoves.empty()) {
                Coordinate selectedEnd = selectedMoves.at(random(selectedMoves.size()));
                return convertCoordinate(selectedBegin, selectedEnd); 
            }
            // if not, check next captured chess
            // since if it cannot escape, it worth nothing to rescue it
            else {
                continue;
            }
        }
        // if for loop end, that is, there exist captured chess, but
        // none of them can be rescued
        // then the situation is the same as no chess is captured
        // so end of if
    }

    // if there is not captured chesses, try to capture opponent's and check opponent
    return captureFirstMove();
}

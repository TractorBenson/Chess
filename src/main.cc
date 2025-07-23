#include "chess/chess.h"
#include "chess/bishop.h"
#include "chess/king.h"
#include "chess/knight.h"
#include "chess/pawn.h"
#include "chess/queen.h"
#include "chess/rook.h"
#include "enum/color.h"
#include "enum/chesstype.h"
#include "observer/graphDisplay.h"
#include "observer/textDisplay.h"
#include "observer/observer.h"
#include "observer/window.h"
#include "square/square.h"
#include "struct/coordinate.h"
#include "struct/moveBackup.h"
#include "third_party/stb_image.h"
#include "RandomNumberGenerator/PRNG.h"
#include <iostream>
#include <cstddef>
#include <vector>
#include <memory>
#include <optional>
#include <string>

using namespace std;

// Helper function to ensure input in range

bool contains(const vector<char>& range, char value) const{
    return std::find(range.begin(), range.end(), value) != range.end();
}

Coordinate convertCoord(string str) {
    char row = str[1];
    char col = str[0];
    int irow = row - '1';
    int icol = col - 'a';
    return Coordinate{irow, icol};
}

bool validCoord (string str) {
    char row = str[1];
    char col = str[0];
    return  (str.length() == 2)&& (row >= '1') && 
        (row <= '8') && (col >= 'a') && (col <= 'h');
}

ChessType 



int main () {
    char typeChar;
    string command;
    Color currentPlayer = Color::WHITE;
    vector<char> validParams = {'R', 'K', 'Q', 'P', 'B', 'N', 'r'
        , 'k', 'q', 'p', 'b', 'n'};

    cout << "Welcome to the Chess Game!" << endl;

    while(true) {
        // Step one: initilizing a empty board
        Board board;
        cout << "Choose game mode:\n"
            << "  setup - manual setup mode\n"
            << "  default - standard starting position\n"
            << "  quit - exit program " << endl;
        

        cin >> command;
        if (cin.fail()) break;
        if (command == "setup") {
            cout << "place a chess:  + [Chesstype] [coordinate]\n"
                    << "remove a chess:  - [coordinate]\n"
                    << "make it color's turn to go next:  = [color]" << endl;
            while (true) {
                string line;
                string para;

                getline(cin, line);
                istringstream iss(line);

                if (iss >> command) {
                    if (command == "+") {
                        if (iss >> typeChar && iss >> para) {
                            if (contains(validParams, typeChar) && 
                                validCoord(para)) {
                                    board.placeChess(convertCoord(para), typeChar);
                            }
                        }
                    } else if (command == "-"){

                    } else if (command == "=") {

                    }
                    cout << "Invalid command. Enter again." << endl;
                    continue;
                } else {
                    cout << "Fail to read command! Enter command again." << endl;
                }
            }
        } else if (command == "default") {
            // initialize the 8*8 grid and attach all observers to squares
            board.initChessesWithDefaultArrange();


        } else if (command == "quit") {
            cout << "You quit the game. Game terminates." << endl;
            break;
        }else{
            cout << "Invalid command, enter command again." << endl;
            continue;
        }
        cout << "Enter "
    
    }
}

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

void switchPlayer(Color& color) {
    if (color == Color::BLACK) {
        color = Color::WHITE;
    } else {
        color = Color::BLACK;
    }
} 



int main () {
    int intCoord;
    int blackScore = 0;
    int whiteScore = 0;
    char typeChar;
    string para; 
    string command;
    Color currentPlayer = Color::WHITE;
    vector<char> validParams = {'R', 'K', 'Q', 'P', 'B', 'N', 'r'
        , 'k', 'q', 'p', 'b', 'n'};

    cout << "Welcome to the Chess Game!" << endl;
    Board board;

    // outmost loop,allows for multiple gameplays
    while(true) {
        // Mode selection stage
        while(true) {
            // Step one: initilizing a empty board
            
            cout << "Choose game mode:\n"
                << "  setup - manual setup mode\n"
                << "  game [player_white] [player_black] - standard starting position\n"
                << "  quit - exit program " << endl;
            

            cin >> command;
            if (cin.fail()) break;
            if (command == "setup") {
                cout << "place a chess:  + [Chesstype] [coordinate]\n"
                        << "remove a chess:  - [coordinate]\n"
                        << "make it color's turn to go next:  = [color]\n"
                        << "quit:  Exits setup mode" << endl;
                while (true) {
                    string line;

                    getline(cin, line);
                    istringstream iss(line);

                    if (iss >> command) {
                        if (command == "+") {
                            // if read of two parameters are successful
                            if (iss >> typeChar && iss >> para) {
                                // If the two parameters are legal
                                if (contains(validParams, typeChar) && 
                                    validCoord(para)) {
                                    // get Coordinate to place chess
                                    intCoord = convertCoord(para);
                                    // cannot place on an existing piece
                                    if (board.squareIsEmpty(intCoord)) {
                                        board.placeChess(intCoord, typeChar);
                                        cout << board;
                                        // check if the setup is valid?
                                        // if (!board.isValidSetup()) {
                                        //     cout << "Current setup is invalid!"
                                        //     << " Please remove the last chess placed." << end;
                                        // }
                                    } else {
                                        cout << "The square is occupied!" << endl;
                                    }
                                    continue;
                                }
                            }
                        } else if (command == "-"){
                            // if the read of the single param suceeds
                            if (iss >> para) {
                                // If the single parameter is legal
                                if (validCoord(para)) {
                                    intCoord = convertCoord(para);
                                    // remove directly, method deals with
                                    //   empty move itself
                                    board.removeChess(intCoord);
                                    cout << board;
                                    // check if the setup is valid?
                                    // if (!board.isValidSetup()) {
                                    //     cout << "Current setup is invalid!"
                                    //     << " Please add back the chess removed." << end;
                                    // }
                                    continue;
                                }
                            }
                        } else if (command == "=") {
                            // if the read of the single param suceeds
                            if (iss >> para) {
                                // If the single parameter is legal
                                if (para == "white" || para == "black") {
                                    switchPlayer(currentPlayer);
                                    if (para == "white") {
                                        cout << "Black will go in next turn" << endl;
                                    } else {
                                        cout << "White will go in next turn" << endl;
                                    }
                                    continue;
                                }
                            }
                        } else if (command == "done") {
                            // exit setup mode
                            // check if the setup is valid?
                            if (!board.isValidSetup()) {
                                cout << "Current setup is invalid!"
                                << " Cannot exit until the setup become valid." << end;
                            } else {
                                cout << "Setup mode exited." << endl;
                                break;
                            }
                        }
                        cout << "Invalid command. Enter again." << endl;
                    } else {
                        cout << "Fail to read mode! Enter command again." << endl;
                    }
                }
                break;
            } else if (command == "default") {
                // initialize the 8*8 grid and attach all observers to squares
                // under default chess layout
                board.initChessesWithDefaultArrange();
                break;

            } else if (command == "quit") {
                cout << "You quit the game. Game terminates." << endl;
                return;
            }else{
                cout << "Invalid command, enter command again." << endl;
                continue;
            }
        }

        while (true) {
            if (command == "resign") {
                if (currentPlayer == Color::BLACK) {
                    cout << "Black resigned! White wins!" << endl;
                    whiteScore++;
                } else {
                    cout << "White resigned! Black wins!" << endl;
                    blackScore++;
                }
                break;
            } else if (command == move)
        }
    }

}

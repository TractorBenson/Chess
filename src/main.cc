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
#include <sstream>
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

// This is changed by alick ----------------------------------------------------------------------------------
bool cin_move(string &move_string, string &move_from_coor, 
              string &move_to_coor) {

    string player_input; // The player input
    
    getline(cin, player_input); // Get the whole line
    if (cin.fail()) {
        if (cin.eof()) {
            return false;
        }
        else {
            cout << "invalid input lol... please type in again!" 
                 << endl;
            return false;
        }
    }

    istringstream iss{player_input}; // Make a istringstream
    if (iss >> move_string) {
        if (iss.fail()) {
            if (iss.eof()) {
                return false;
            }
            cin.clear();
            cin.ignore();
            cout << "Please input \"move\" first!" << endl;
            return false;
        } else {
            if (iss >> move_from_coor) {
                if (!validCoord(move_from_coor)) {
                    cout << "Please input a right coordinate format plz!" 
                         << endl;
                    return false;
                } else if (iss >> move_to_coor) {
                    if (!validCoord(move_to_coor)) {
                        cout << "Please input a right coordinate format plz!" 
                            << endl;
                        return false;
                    }
                    return true;
                } else {
                    if (iss.eof()) {
                        return false;
                    }
                    cout << "Please input a right coordinate format plz!" 
                            << endl;
                    return false;
                }
            } else {
                if (iss.eof()) {
                    return false;
                }
                cout << "Please input a right coordinate format plz!" 
                        << endl;
                return false;
            }
        }
    } else {
        if (iss.eof()) {
            endGameMessage(black_score, white_score);
            return false;
        }
        cin.clear();
        cin.ignore();
        cout << "Missing all the commands lol" << endl;
        return false;
    }
    return false;
} // This is changed by alick----------------------------------------------------------------------------------


void endGameMessage(int black_score, int white_score) {
    cout << "The score so far is: " << endl;
    cout << "Black " << black_score << " : White " << 
            << white_score << endl;
    cout << "Thanks for playing this!" << endl;
    return;
}

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
        if (cin.fail()) {
            // Alick changed here----------------------------------------------------------
            if (cin.eof()) {
                endGameMessage(black_score, white_score);
            }
            cin.clear();
            cin.ignore();
            cout << "Not a right command! Please enter again!" << endl;
            continue;
            // Alick changed here----------------------------------------------------------
        }
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
        } else if (command == "Default") {
            
            // initialize the 8*8 grid and attach all observers to squares
            board.initChessesWithDefaultArrange();
            Color currentPlayer = Color::BLACK;
            cout << "The board has been created! Please select the play mode! :)" 
                 << endl;
            string gameMode;
            getline(cin, gameMode);
            if (gameMode == "game white-player black-player") { // Alick changed here -------------------------------------------------
                while (true) {
                    if (board.isCheckmate(Color::BLACK)) {
                        // If the black side checkmates the white, black wins!
                        cout << "Black wins the game!    Game over!" << endl;
                        break;
                    } else if (board.isCheckmate(Color::WHITE)){
                        // If the white side checkmates the black, white wins!
                        cout << "Black wins the game!    Game over!" << endl;
                        break;
                    } else if ((!(board.isCheck(Color::BLACK)) && 
                                board.noValidMoves(Color::WHITE)) ||
                            (!(board.isCheck(Color::WHITE)) && 
                                board.noValidMoves(Color::BLACK))) {
                        // If one side makes the other side stalemate, stalemate!
                        cout << "Stalemate!" << endl;
                        break;
                    }

                    // Switch the player!
                    if (currentPlayer == Color::BLACK) {
                        currentPlayer = Color::WHITE;
                        cout << "It's White's move!" << endl;
                    } else {
                        currentPlayer = Color::BLACK;
                        cout << "It's Black's move!" << endl;
                    }

                    // Remind the player to move the chess
                    cout << "Please Enter your brilliant move!" << endl;
                    cout << "format: move [a-h][1-8] [a-h][1-8]" << endl << 
                        << "e.x. move e2 e4 :)" << endl;


                    string move_string; // Read the move command string

                    string move_from_coor; // Read the begin coordinate

                    string move_to_coor; // Read the end coordinate

                    while (!cin_move(move_string, move_from_coor, move_to_coor)) {
                        if (cin.eof()) {
                            endGameMessage(black_score, white_score);
                            return 0;
                        }
                        cin.clear();
                        cin.ignore();
                    }
                    
                    
                    
                } // Alick changed here------------------------------------------------------------------------------------------------
            }

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

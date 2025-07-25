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
#include "bot/bot.h"
#include <iostream>
#include <sstream>
#include <cstddef>
#include <vector>
#include <memory>
#include <optional>

#include <string>

using namespace std;

// Helper function to ensure input in range
template <typename T>
bool contains(const vector<T>& range, T value){
    return find(range.begin(), range.end(), value) != range.end();
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

void cin_move(Board &b, bool &resigned, string &fromCoord,
             string &toCoord, char &promotedTo, Color &currentPlayer,
            vector<char>& validPromote) {

    string player_input; // The player input
    Coordinate fromC;
    Coordinate toC;
    while (true) {
        cout << "Enter command:\n move [begin] [end]" << endl;
        getline(cin, player_input); // Get the whole line

        istringstream iss(player_input); // Make a istringstream
        string cmd;
        if (!(iss >> cmd)) {
            cout << "read unsuccessful. Enter command again" << endl;
            continue;
        }
        if (cmd != "move" && cmd != "resign") { // check for first part of command
            std::cout << "Unknown command: " << cmd << endl;
            continue;
        }
        // if resigned, exit game immediately
        if (cmd == "resign") {
            resigned = true;
            return;
        }
        // command "move coord1 coord2" is provided
        if (!(iss >> fromCoord >> toCoord)) {
            cout << "Syntax: move <from> <to> [chess]" << endl;
            continue;                                   // ask again
        }
        
        // Two coordinate arguments are checked
        if (!validCoord(fromCoord) || !validCoord(toCoord)) {
            std::cout << "Invalid coordinate arguments." << endl;
            continue;
        }
        fromC = convertCoord(fromCoord);
        toC = convertCoord(toCoord);
        // Only allowed to move chess on currentPlayer's side.
        if (b.getChessColor(fromC) != currentPlayer) {
            cout << "Invalid command. Please choose your own chess to move." << endl;
            continue;
        }
        // Input is a valid format move coord1 coord2
        if (b.moveChess(fromC, toC)) {
            // a promtion is needed.
            if (b.canPromote(fromC, toC)) {
                // read char successful
                if (iss >> promotedTo) {
                    if (contains(validPromote, promotedTo)) {
                    
                        if (currentPlayer == Color::WHITE) {
                            // only allowed to promote chess to own color
                            if(islower(static_cast<unsigned char>(promotedTo))) {
                                b.removeChess(toC); // remove destination chess, if any
                                b.removeChess(fromC); // remove the pawn
                                b.placeChess(toC, promotedTo); // add the new promoted chess
                                break;
                            }
                        } else {
                            if(isupper(static_cast<unsigned char>(promotedTo))) {
                                b.removeChess(toC); // remove destination chess, if any
                                b.removeChess(fromC); // remove the pawn
                                b.placeChess(toC, promotedTo); // add the new promoted chess
                                break;
                            }
                        }
                    }
                    cout << "Invalid promotion" << endl;
                    continue;
                }// read promote char
                cout << "Promotion argument cannot found. Invalid command." << endl;
                continue;
            } // check promotion
            // ordinary move for chess other than pawn
            b.removeChess(toC);
            b.simpleMove(fromC, toC);
            break;
        } // move valid check
    } // while loop
}

unique_ptr<Bot> createWhiteBot(Board* board, string& kind) {
    if (kind == "computer[1]") return make_unique<Bot>(board, Color::WHITE, 1);
    else if (kind == "computer[2]") return make_unique<Bot>(board, Color::WHITE, 2);
    else if (kind == "computer[3]") return make_unique<Bot>(board, Color::WHITE, 3);
    else if (kind == "computer[4]") return make_unique<Bot>(board, Color::WHITE, 4);

    return nullptr;
}
unique_ptr<Bot> createBlackBot(Board* board, string kind) {

    if (kind == "computer[1]") return make_unique<Bot>(board, Color::BLACK, 1);
    else if (kind == "computer[2]") return make_unique<Bot>(board, Color::BLACK, 2);
    else if (kind == "computer[3]") return make_unique<Bot>(board, Color::BLACK, 3);
    else if (kind == "computer[4]") return make_unique<Bot>(board, Color::BLACK, 4);

    return nullptr;
}

void endGameMessage(int black_score, int white_score) {
   cout << "The score so far is: " << endl;
   cout << "White: " << white_score << endl;
   cout << "Black: " << black_score << endl;
}


int main () {

    Coordinate intCoord;
    double blackScore = 0;
    double whiteScore = 0;
    char typeChar;
    // strings to store whether palyers are computer or human
    string playerMode;
    string currentPlayerType;
    string whitePlayer;
    string blackPlayer;
    string from;
    string end;
    char promotedTo;

    unique_ptr<Bot> whiteBot = nullptr;
    unique_ptr<Bot> blackBot = nullptr;

    bool resigned = false;

    string para; 
    string command;
    Color currentPlayer = Color::WHITE;
    vector<char> validParams = {'R', 'K', 'Q', 'P', 'B', 'N', 'r'
        , 'k', 'q', 'p', 'b', 'n'};
    vector<char> validPromote ={ 'R', 'Q', 'B', 'N', 'r',
         'q', 'b', 'n'};
    vector<string> players = {"human", "computer[1]", "computer[2]",
         "computer[3]", "computer[4]"};

    
    Board board;

    // outmost loop,allows for multiple gameplays
    while(true) {
        bool isQuit = false;
        cout << "Welcome to the Chess Game!" << endl;
        cout << "Choose game mode:\n"
                << "  setup - manual setup mode\n"
                << "  default - standard starting position\n"
                << "  quit - exit program " << endl;  
        
        // Mode selection stage
        while(true) {
            // Step one: initilizing a empty board
            if (!(cin >> command)) break;
            if (command == "quit"){
                isQuit = true;
                break;
            }
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
                                cin >> command;
                                continue;
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
                cout << board;
                
                break;
            } else{
                cout << "Invalid command, enter command again." << endl;
                continue;
            }
        }// while

        if (isQuit) {
            cout << "Game is terminated." << endl;
            break;
        }
        // determine if the palyers on both sides computers? humans? a mix?
        //   what level is the computer?
        while (true) {
            
            getline(cin, playerMode);
            istringstream iss2(playerMode);
            cout << "Enter:\ngame [white-player] [black-player], "
                << "players are either \"human\" or \"computer[1-4]\"." << endl;
            if (iss2 >> command) {
                // the command is correct
                if (command == "game") {
                    // success in reading first arg
                    if(iss2 >> whitePlayer) {
                        // arg1 in range
                        if (contains(players, whitePlayer)){
                            // read of second arg successful
                            if (iss2 >> blackPlayer) {
                                // arg2 in range
                                if (contains(players, blackPlayer)) {
                                    break;
                                }
                            }// if arg2
                        } // if contain
                    }// if arg1
                }// if game
                cout << "Invalid command." << endl;
                continue;
            }
            cout << "Read from input is unsuccessful." << endl;
        }// while

        // create bots, if exists
        if (whitePlayer != "human") {
            whiteBot = createWhiteBot(&board, whitePlayer);
        }
        if (blackPlayer != "human") {
            blackBot = createBlackBot(&board, blackPlayer);
        }
        // The actual playing section starts, repeatedly taking
        //  move commands until resign, draw, or checkmate.
        while (true) {
            Color opponent;
            if (currentPlayer == Color::BLACK){
                opponent = Color::WHITE;
            } else {
                opponent = Color::BLACK;
            }

            // Check draw at beginning of each turn:
            //   not checked but has no valid moves.
            //   Terminate current game immediately
            if ((!(board.isCheck(opponent)) &&
                board.noValidMoves(currentPlayer))) {
                cout << "Stalemate!" << endl;
                blackScore += 0.5;
                whiteScore += 0.5;
                break;
            }
            
            // move directly if white player is a bot
            if (currentPlayer == Color::WHITE && whitePlayer != "human") {
                while (true) {
                    cin >> command;
                    if (command == "move") {
                        command = whiteBot->move();
                        istringstream iss4(command);
                        iss4 >> from;
                        iss4 >> end;
                        
                        // deal with promotion and ordinary moves
                         if (iss4 >> promotedTo) {
                            board.removeChess(convertCoord(end));
                            board.removeChess(convertCoord(from));
                            board.placeChess(convertCoord(end), promotedTo);
                        } else {
                            board.removeChess(convertCoord(end));
                            board.simpleMove(convertCoord(from), convertCoord(end));
                        }
                        break;
                    } else if (command == "resign") {
                        if (currentPlayer == Color::BLACK) {
                            cout << "Black resigned! White wins!" << endl;
                            whiteScore++;
                        } else {
                            cout << "White resigned! Black wins!" << endl;
                            blackScore++;
                        }
                        break;
                    }
                    cout << "invalid command, enter \"move\" "
                        << "to let the computer make a move.\n"
                        << "Enter \"resign\" to give up current game." << endl;
                }
            // move directly if black player is a bot
            } else if (currentPlayer == Color::BLACK && blackPlayer != "human") {
                 while (true) {
                    cin >> command;
                    if (command == "move") {
                        command = blackBot->move();
                        // the command output from bot is garanteed to be valid.
                        istringstream iss5(command);
                        iss5 >> from;
                        iss5 >> end;
                        // deal with promotion and ordinary moves
                         if (iss5 >> promotedTo) {
                            board.removeChess(convertCoord(end));
                            board.removeChess(convertCoord(from));
                            board.placeChess(convertCoord(end), promotedTo);
                        } else {
                            board.removeChess(convertCoord(end));
                            board.simpleMove(convertCoord(from), convertCoord(end));
                        }
                        break;
                    } else if (command == "resign") {
                        if (currentPlayer == Color::BLACK) {
                            cout << "Black resigned! White wins!" << endl;
                            whiteScore++;
                        } else {
                            cout << "White resigned! Black wins!" << endl;
                            blackScore++;
                        }
                        break;
                    }
                    cout << "invalid command, enter \"move\" "
                        << "to let the computer make a move.\n"
                        << "Enter \"resign\" to give up current game." << endl;
                }
            } else { // current turn is a human player
                // take input from user and make the move
                cin_move(board, resigned, from, end, promotedTo,
                     currentPlayer, validPromote);
                if (resigned) {
                    if (currentPlayer == Color::BLACK) {
                        cout << "Black resigned! White wins!" << endl;
                        whiteScore++;
                    } else {
                        cout << "White resigned! Black wins!" << endl;
                        blackScore++;
                    }
                    break;
                }
            }// current turn move finished

            // examine checkmate situation at start of each turn
            //   (currentPlayer checkmates the opponent)
            if (board.isCheckmate(currentPlayer)) {
                // Add one score on currentPlayer
                if (currentPlayer == Color::WHITE) {
                    whiteScore++;
                } else {
                    blackScore++;
                }
                // terminate current game immediately
                break;
            } // check condition is determined together with draw at the
            //   beginning of the turn.
            switchPlayer(currentPlayer);
        }// while loop for each game
        endGameMessage(blackScore, whiteScore);
    } // outmost while
    cout << "Final Score:\n"
        << "White: " << whiteScore << "\n"
        << "Black: " << blackScore << endl;
}

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
#include <limits>

#include <string>

using namespace std;

// per function to ensure input in range
template <typename T>
bool contains(const vector<T>& range, T value){
    return find(range.begin(), range.end(), value) != range.end();
}

Coordinate convertCoord(string str) {
    char row = str[1];
    char col = str[0];
    cout << "col is: " << col << endl;
    int irow = row - '1';
    int icol = col - 'a';
    cout << "icol is: " << icol << endl;
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
            vector<char>& validPromote, bool& isQuit) {

    string player_input; // The player input
    Coordinate fromC;
    Coordinate toC;

    while (true) {
        // set all pawns to be not capable of en passant
        b.updateChess(currentPlayer);
        cout << b;

        cout << "Enter command:\n move [begin] [end]" << endl;
        getline(cin, player_input); // Get the whole line

        istringstream iss(player_input); // Make a istringstream
        string cmd;
        if (!(iss >> cmd)) {
            cout << "read unsuccessful. Enter command again" << endl;
            continue;
        }
        if (cmd != "move" && cmd != "resign" && cmd != "quit") { // check for first part of command
            std::cout << "Unknown command: " << cmd << endl;
            continue;
        }
        if (cmd == "quit") {
            isQuit = true;
            break;
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
        // cout << fromCoord << endl;
        // cout << toCoord << endl;
        fromC = convertCoord(fromCoord);
        toC = convertCoord(toCoord);
        // cout << fromC.row << endl;
        // cout << fromC.col << endl;
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
                            cout << " promoted to a : " << promotedTo << endl;
                            // only allowed to promote chess to own color
                            if(islower(static_cast<unsigned char>(promotedTo))) {
                                cout << "line 132" << endl;
                                b.removeChess(toC); // remove destination chess, if any
                                b.removeChess(fromC); // remove the pawn
                                b.placeChess(toC, promotedTo); // add the new promoted chess
                                break;
                            }
                        } else {
                            if(isupper(static_cast<unsigned char>(promotedTo))) {
                                cout << "line 140" << endl;
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
            
            cout << "line 163" << endl;
            break;
        } // move valid check
        if (cin.fail()) {
            cin.clear();   // reset fail bit
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    } // while loop
}

unique_ptr<Bot> createWhiteBot(Board* board, string& kind) {
    if (kind == "computer[1]") return make_unique<Bot>(board, Color::WHITE, 1);
    else if (kind == "computer[2]") return make_unique<Bot>(board, Color::WHITE, 2);
    else if (kind == "computer[3]") return make_unique<Bot>(board, Color::WHITE, 3);

    return nullptr;
}
unique_ptr<Bot> createBlackBot(Board* board, string kind) {

    if (kind == "computer[1]") return make_unique<Bot>(board, Color::BLACK, 1);
    else if (kind == "computer[2]") return make_unique<Bot>(board, Color::BLACK, 2);
    else if (kind == "computer[3]") return make_unique<Bot>(board, Color::BLACK, 3);

    return nullptr;
}

void endGameMessage(double black_score, double white_score) {
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
         "computer[3]"};

    
    

    // outmost loop,allows for multiple gameplays
    while(true) {
        Board board;
        bool isQuit = false;
        cout << "Welcome to the Chess Game!" << endl;
        
        
        cout << board;
        // Mode selection stage
        while(true) {
            cout << "Choose game mode:\n"
                << "  setup - manual setup mode\n"
                << "  default - standard starting position\n"
                << "  quit - exit program " << endl; 
            // Step one: initilizing a empty board
            if (cin.fail()) {
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            if (!(cin >> command)) break;
            
            if (command == "quit"){
                isQuit = true;
                break;
            }
            // Enter setup mode
            if (command == "setup") {
                // while loop for setup operations
                while (true) {
                    if (cin.fail()) {
                        cin.clear();   // reset fail bit
                        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                    string line;
                     cout << "place a chess:  + [Chesstype] [coordinate]\n"
                        << "remove a chess:  - [coordinate]\n"
                        << "make it color's turn to go next:  = [color]\n"
                        << "done:  Exits setup mode" << endl;
                    cout << board;
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

                                    } else { // the square has an existing chess
                                        board.removeChess(intCoord);
                                        board.placeChess(intCoord, typeChar);
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
                            cout << "line 317" << endl;
                            if (!board.isValidSetup()) {
                                cout << "line 319" << endl;
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
                    
                }// while loop for setup commands
                break;
            } else if (command == "default") {
                // initialize the 8*8 grid and attach all observers to squares
                // under default chess layout
                board.initChessesWithDefaultArrange();
                //-----------------------------------------------------test 
                cout << board;
                break;
            } else{
                cout << "Invalid command, enter command again." << endl;
                continue;
            }
            if (cin.fail()) {
                cin.clear();   // reset fail bit
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }// while for mode selection

        if (isQuit) {
            cout << "Game is terminated." << endl;
            break;
        }
        //-----------------------------------------------------------------------------------
        // Second while loop determine if the palyers on both sides computers? humans? a mix?
        //   what level is the computer?
        //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        while (true) {
            cout << "Enter:\ngame [white-player] [black-player], "
                << "players are either \"human\" or \"computer[1-4]\"." << endl;
            string playerType;
            if (!getline(cin, playerType)) {
                cout << "fail to read" << endl;
                continue;
            }
            istringstream iss2(playerType);
            if (iss2 >> command) {
                if (command == "game" &&
                    iss2 >> whitePlayer &&
                    contains(players, whitePlayer) &&
                    iss2 >> blackPlayer &&
                    contains(players, blackPlayer) &&
                    iss2.eof())                    // nothing extra on the line
                {
                    break;                        // success
                }
                if (command == "quit") {
                    isQuit = true;
                    break;
                }
            }

            cout << "Invalid command. Try again.\n";

            //---------- CLEAR ONLY IF NEEDED -------------
            if (cin.fail()) {
                cin.clear();   // reset fail bit
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }// while
        
        if (isQuit) {
            cout << "Game is terminated." << endl;
            break;
        }

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
            board.isCheck(opponent);
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
                   
                    if (cin.fail()){
                        cin.clear();   // reset fail bit
                        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                    cout << board;
                    cout << "Enter \"move\" to let the bot move" << endl;
                    cin >> command;

                    if (command == "move") {

                        // cout << "11111" << endl;

                        command = whiteBot->move();

                        // cout << "2222" << endl;

                        istringstream iss4(command);
                        iss4 >> from;
                        cout << "before bot move: " << from;
                        iss4 >> end;
                        cout << "before bot move: " << end;
                        
                        // deal with promotion and ordinary moves
                         if (iss4 >> promotedTo) {
                            board.removeChess(convertCoord(end));
                            board.removeChess(convertCoord(from));
                            board.placeChess(convertCoord(end), promotedTo);
                        } else {
                            cout << "reached 443" << endl;
                            board.removeChess(convertCoord(end));
                            cout << "reached 445" << endl;
                            board.simpleMove(convertCoord(from), convertCoord(end));
                            cout << "reached 447" << endl;
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
                    if (cin.fail()){
                        cin.clear();   // reset fail bit
                        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                    cout << "Enter \"move\" to let the bot move" << endl;
                    cin.clear();   // reset fail bit
                    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    cout << board;
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
                     currentPlayer, validPromote, isQuit);
                if (isQuit) {
                    break;
                }
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
            cout << "main 545" << endl;
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
            cout << "line 556" << endl;
            switchPlayer(currentPlayer);
        }// while loop for each game
        if (isQuit) {
            break;
        }
        cout << board;
        endGameMessage(blackScore, whiteScore);
    } // outmost while
    cout << "Final Score:\n"
        << "White: " << whiteScore << "\n"
        << "Black: " << blackScore << endl;
}

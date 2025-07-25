#include "observer/graphDisplay.h"

GraphDisplay::GraphDisplay(size_t n): numSquare{n} {
    xw = std::make_unique<Xwindow>(800,800);

    int resultingGridLength = WINDOW_SIZE; // use to subract until fit
    int mod = resultingGridLength % n; // module to store redundant edge
    resultingGridLength -= mod; // minus edge, so is real size of grid

    edgeSize = mod / 2; // mod is total size of two side of edge, so /2

    size = resultingGridLength / n; // length of one cell in px


    // init background
    xw->fillRectangle(0, 0, WINDOW_SIZE, WINDOW_SIZE, BACKGROUND_COLOR);
}

char GraphDisplay::convertChess(const Chess *pChess) {
    // if chess* is nullptr:
    if (pChess == nullptr) {
        return 'X';
    }

    Color chessColor = pChess->getColor();

    // if not nullptr, then it has a type
    switch (pChess->getType()) {
        // if it is Pawn, return p
        case ChessType::Pawn: {
            // if color is white, return capital letter
            if (chessColor == Color::WHITE) {
                return 'P';
            }
            // if color is black, return lower case letter
            else if (chessColor == Color::BLACK) {
                return 'p';
            }
            // in case of undefined behaviour, if receive Color::NOTHING, go to end of function
            break;
        }

        // if it is Knight, return n
        case ChessType::Knight: {
            if (chessColor == Color::WHITE) {
                return 'N';
            }
            else if (chessColor == Color::BLACK) {
                return 'n';
            }
            break;
        }

        // if it is Bishop, return b
        case ChessType::Bishop: {
            if (chessColor == Color::WHITE) {
                return 'B';
            }
            else if (chessColor == Color::BLACK) {
                return 'b';
            }
            break;
        }

        // if it is Rook, return r
        case ChessType::Rook: {
            if (chessColor == Color::WHITE) {
                return 'R';
            }
            else if (chessColor == Color::BLACK) {
                return 'r';
            }
            break;
        }

        // if it is Queen, return q
        case ChessType::Queen: {
            if (chessColor == Color::WHITE) {
                return 'Q';
            }
            else if (chessColor == Color::BLACK) {
                return 'q';
            }
            break;
        }  

        // if it is King, return k
        case ChessType::King: {
            if (chessColor == Color::WHITE) {
                return 'K';
            }
            else if (chessColor == Color::BLACK) {
                return 'k';
            }
            break;
        }
    }

    // return 'X' to indicate error
    return 'X';
}

Color GraphDisplay::switchColor(Color color) {
    if (color == Color::WHITE) return Color::BLACK;
    else if (color == Color::BLACK) return Color::WHITE;
    else return color;
}

void GraphDisplay::drawSquareByIndex(size_t row, size_t col, Color color) {
    // calc real position of the Square
    int resultX = col * size + edgeSize;
    int resultY = row * size + edgeSize;

    // convert color into given format
    int colorNum;
    if (color == Color::WHITE) colorNum = 2;
    else if (color == Color::BLACK) colorNum = 3;
    else colorNum = 0;

    xw->fillRectangle(resultX, resultY, size, size, colorNum);
}

void GraphDisplay::drawStringByIndex(size_t row, size_t col, std::string str, Color color) {
    int resultX = col * size + edgeSize + (size * 7 / 16);
    int resultY = row * size + edgeSize + (size * 9 / 16);

    int numColor;
    if (color == Color::WHITE) numColor = 1;
    else if (color == Color::BLACK) numColor = 0;
    else numColor = 0;

    xw->drawString(resultX, resultY, str, numColor);
}

// corner - 0 means top-left, !0 means bottom-right
void GraphDisplay::drawCharAtCorner(size_t row, size_t col, char c, Color color, int corner) {
    int resultX;
    int resultY;
    if (corner == 0) {
        // magic numbers to put letters to correct possition
        resultX = col * size + edgeSize + (size * 2 / 62);
        resultY = row * size + edgeSize + (size / 4) - (size * 2/ 62);
    }
    else {
        // magic numbers to put letters to correct possition
        resultX = col * size + edgeSize + (size * 7 / 8);
        resultY = row * size + edgeSize + size - (size * 4 / 62);
    }

    int numColor;
    if (color == Color::WHITE) numColor = 2;
    else if (color == Color::BLACK) numColor = 3;
    else numColor = 0;

    std::string str{c};

    xw->drawString(resultX, resultY, str, numColor);
}

void GraphDisplay::drawPicByIndex(size_t row, size_t col, char c) {
    int resultX = col * size + edgeSize;
    int resultY = row * size + edgeSize;
    string key = "";

    switch (c) {
        case 'b': {
            key = "BlackBishop";
            break;
        }
        case 'k': {
            key = "BlackKing";
            break;
        }
        case 'n': {
            key = "BlackKnight";
            break;
        }
        case 'p': {
            key = "BlackPawn";
            break;
        }
        case 'q': {
            key = "BlackQueen";
            break;
        }
        case 'r': {
            key = "BlackRook";
            break;
        }
        case 'B': {
            key = "WhiteBishop";
            break;
        }
        case 'K': {
            key = "WhiteKing";
            break;
        }
        case 'N': {
            key = "WhiteKnight";
            break;
        }
        case 'P': {
            key = "WhitePawn";
            break;
        }
        case 'Q': {
            key = "WhiteQueen";
            break;
        }
        case 'R': {
            key = "WhiteRook";
            break;
        }
    }

    xw->drawPic(key, resultX, resultY);
}



void GraphDisplay::notify(const Square& square) {
    // get coordinate and color of the Square
    Coordinate co = square.getCoordinate();

    // co.row is actual index - bottomleft is 0,0
    // rowIndex is index for window - topleft is 0,0

    size_t rowIndex = (numSquare - 1 - co.row);
    size_t colIndex = co.col;
    Color squareColor = square.getColor();

    // draw this square
    drawSquareByIndex(rowIndex, colIndex, squareColor);



    const Chess *theChess = square.getChess();

    // // draw chess by char
    // if (theChess) {
    //     std::string stringChess = convertChess(theChess);
    // }

    // drawStringByIndex(rowIndex, colIndex, stringChess, theChess->getColor());

    // draw chess by pics
    drawPicByIndex(rowIndex, colIndex, convertChess(theChess));
    

    // draw small number/letter indicate position
    if (co.row == 0) {
        drawCharAtCorner(rowIndex, colIndex, static_cast<char>('a' + co.col), switchColor(squareColor), 1);
    }
    if (co.col == 0) {
        drawCharAtCorner(rowIndex, colIndex, static_cast<char>('1' + co.row), switchColor(squareColor), 0);
    }
}


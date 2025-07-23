#include "observer/graphDisplay.h"

GraphDisplay::GraphDisplay(size_t n): numSquare{n} {
    xw = std::make_unique<Xwindow>();

    int resultingGridLength = WINDOW_SIZE; // use to subract until fit
    int mod = resultingGridLength % n; // module to store redundant edge
    resultingGridLength -= mod; // minus edge, so is real size of grid

    edgeSize = mod / 2; // mod is total size of two side of edge, so /2

    size = resultingGridLength / n; // length of one cell in px


    // init background
    xw->fillRectangle(0, 0, WINDOW_SIZE, WINDOW_SIZE, BACKGROUND_COLOR);
}

Color switchColor(Color color) {
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
    int resultY = row * size + edgeSize - (size * 7 / 16);

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
        resultX = col * size + edgeSize + 2;
        resultY = row * size + edgeSize + (size / 4) - 2;
    }
    else {
        // magic numbers to put letters to correct possition
        resultX = col * size + edgeSize + (size * 7 / 8);
        resultY = row * size + edgeSize + size - 4;
    }

    int numColor;
    if (color == Color::WHITE) numColor = 2;
    else if (color == Color::BLACK) numColor = 3;
    else numColor = 0;

    std::string str{c};

    xw->drawString(resultX, resultY, str, numColor);
}

void GraphDisplay::notify(const Square& square) {
    // get coordinate and color of the Square
    Coordinate co = square.getCoordinate();
    size_t rowIndex = 7 - co.row;
    size_t colIndex = co.col;
    Color squareColor = square.getColor();

    // draw this square
    drawSquareByIndex(rowIndex, colIndex, squareColor);


    // get chess
    const Chess *theChess = square.getChess();
    if (theChess) {
        std::string stringChess = "X"; // 'X' indicate error
        switch (theChess->getType()) {
            case ChessType::Pawn: {
                stringChess = "P";
                break;
            }
            case ChessType::Knight: {
                stringChess = "N";
                break;
            }
            case ChessType::Bishop: {
                stringChess = "B";
                break;
            }
            case ChessType::Rook: {
                stringChess = "R";
                break;
            }
            case ChessType::Queen: {
                stringChess = "Q";
                break;
            }
            case ChessType::King: {
                stringChess = "K";
                break;
            }
        }
        drawStringByIndex(rowIndex, colIndex, stringChess, theChess->getColor());
    }
    
    // draw small number/letter indicate position
    if (rowIndex == 1) {
        drawCharAtCorner(rowIndex, colIndex, static_cast<char>('a' + colIndex), switchColor(squareColor), 1);
    }
    if (colIndex == 1) {
        drawCharAtCorner(rowIndex, colIndex, static_cast<char>(1 + rowIndex), switchColor(squareColor), 0);
    }
}

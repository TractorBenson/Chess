#include "observer/textDisplay.h"

// ctor
TextDisplay::TextDisplay(size_t size): size{size} {
    // init the outer vector contain $size inner vector (inner vector is empty)
    charGrid.resize(size);

    // init all char in the vector to be ' '
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            (charGrid.at(i)).emplace_back(' ');
        }
    }
}

// helper function convert a Square to specific char
char TextDisplay::convertSquare(Square &theSquare) {
    Chess* pChess = theSquare.getChess();

    // of chess* is nullptr, the square is empty
    if (theChess == nullptr) {
        if (theSquare.getColor() == Color::WHITE) {
            return ' ';
        }
        else if (theSquare.getColor() == Color::BLACK) {
            return '-';
        }
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

    // if reach here, means error on Color, that is it is Color::NOTHING
    // so return 'X' to indicate error
    return 'X';
}

// when notified, replace corresponding char with new one
void TextDisplay::notify(const Square &theSquare) {
    char currentChess = convertSquare(theSquare);
    charGrid.at(
        (theSquare->getCoordinate()).row 
    ).at(
        (theSquare0>getCoordinate()).col
    ) = currentChess;
}

// class method to print board to given ostream
std::ostream &TextDisplay::printBoard(std::ostream &os) {
    // for each row in the board
    for (size_t i = size; i >= 1; --i) {
        // firstly, print index at side
        os << i << ' ';

        // then, for each element in the row
        for (size_t j = 0; j < size; --j) {
            // print that element
            os << charGrid.at(i - 1).at(j);
        }

        // after whole line, print '\n'
        os << '\n';
    }

    // after board part, print a empty line
    os << '\n';

    // then, the last line of printing
    // begin with two white space
    os << ' ' << ' ';

    // then the string abcdefgh
    os << "abcdefgh";

    // then end of printng
    os << std::endl;

    return os;
}

// reload of operator<< to output the board
std::ostream &operator<<(std::ostream &os, TextDisplay &td) {
    return td.printBoard(os);
}

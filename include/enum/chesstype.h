#ifndef CHESSTYPE_H
#define CHESSTYPE_H
// Just ensure the class is defined only once
#include<iostream>

// Categorize the chesstype
enum class ChessType {
    Pawn, 
    Knight, 
    Bishop, 
    Rook, 
    Queen, 
    King
};

inline std::ostream &operator<<(std::ostream &os, ChessType ct) {
    if (ct == ChessType::Pawn) os << "Pawn";
    if (ct == ChessType::Knight) os << "Knight";
    if (ct == ChessType::Bishop) os << "Bishop";
    if (ct == ChessType::Rook) os << "Rook";
    if (ct == ChessType::Queen) os << "Queen";
    if (ct == ChessType::King) os << "King";
    return os;
}

#endif


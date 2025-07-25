#ifndef COLOR_H
#define COLOR_H
// Just ensure the class is defined only once
#include <iostream>

// Categorize the colors
enum class Color {
    WHITE, 
    BLACK, 
    NOTHING
};

inline std::ostream &operator<<(std::ostream &os, Color color) {
    if (color == Color::WHITE) os << "WHITE";
    if (color == Color::BLACK) os << "BLACK";
    if (color == Color::NOTHING) os << "NOTHING";
    return os;
}

#endif


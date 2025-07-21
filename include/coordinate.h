#ifndef COORDINATE_H
#define COORDINATE_H
// Just ensure the class is defined only once

struct Coordinate {
    int row;
    int col;
    friend class chess;
};

#endif
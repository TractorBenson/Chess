#ifndef COORDINATE_H
#define COORDINATE_H
// Just ensure the class is defined only once

struct Coordinate {
    int row;
    int col;
};


bool operator==(Coordinate c1, Coordinate c2) {
    if (c1.row == c2.row) {
        return c1.col == c2.col;
    }
    return false;
}

bool operator<(Coordinate c1, Coordinate c2) {
    if (c1.row != c2.row) {
        return c1.row < c2.row;
    }
    return c1.col < c2.col;
}

#endif
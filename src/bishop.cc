#include <cstddef>
#include "algorithm"
#include "bishop.h"
#include "chess.h"
#include "board.h"

bool Bishop::isValidMove(const Board &theBoard, Coordinate begin, 
                         Coordinate end) {
    int diff_x_coordinate = end.col - begin.col;
    int diff_y_coordinate = end.row - begin.row;
    if (diff_x_coordinate != diff_y_coordinate & 
        diff_x_coordinate != diff_y_coordinate * -1) {
        return false;
    }
    else {
        int steps_to_check_obstacle = diff_x_coordinate;
        if (diff_x_coordinate < 0) {
            steps_to_check_obstacle *= -1;
        }
        Coordinate one_step{diff_x_coordinate / steps_to_check_obstacle, 
                              diff_y_coordinate / steps_to_check_obstacle};
        Coordinate current_check_step{begin};
        for (int i = 1; i < steps_to_check_obstacle - 1; i++) {
            
        }
    }
}
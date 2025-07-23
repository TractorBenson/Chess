#include "square/square.h"
#include "chess/chess.h"
#include "chess/king.h"

struct MoveBackup {
    Square* from;
    Square* to;
    Chess* movedC;
    Chess* capturedC;
    King* movedKing;
};

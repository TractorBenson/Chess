#ifndef MOVEBACKUP_H
#define MOVEBACKUP_H

class Square;
class Chess;
class King;
struct MoveBackup {
    Square* from;
    Square* to;
    Chess* movedC;
    Chess* capturedC;
    King* movedKing;
};

#endif

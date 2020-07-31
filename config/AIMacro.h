#ifndef AIMACRO_H
#define AIMACRO_H

typedef unsigned char uint8;
typedef unsigned char typeQX;
typedef unsigned int  uint32;

struct Point
{
    int x;
    int y;
    uint32 priority;

    bool operator<(const Point & other) const{
        return this->priority < other.priority;
    }

    Point():
    priority(0x3f3f3f){};
};

#define MaxSearchDepth 2

#define AI_1 1
#define AI_2 2
#define AI_3 3
#define AI_4 4
#define AI_5 5
#define OP_1 6
#define OP_2 7
#define OP_3 8
#define OP_4 9
#define OP_5 10

#define score0 1
#define score1 5
#define score2 25
#define score3 125
#define score4 6250
#define INF                0x3f3f3f3f
#define TenOne             0x3ff
#define HashTableSize      0x2ab

#endif
// AIMACRO_H

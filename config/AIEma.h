#ifndef AIEMA_H
#define AIEMA_H

#include "AIMacro.h"

#include <QThread>

class AI : public QThread
{
    Q_OBJECT

private:
    bool   AiSide;
    int    crtDecNum;
    int    AIboard[15][15];

    uint32 Row[15];
    uint32 Col[15];
    uint32 NorthEast[21];
    uint32 SouthEast[21];

    uint32 AINumTypeQX[5];
    uint32 OPNumTypeQX[5];

    typeQX HashTable[HashTableSize];

    Point  FirstThree[3];
    std::pair<int,int>  NextStep;

    void initHash();
    void initCnts();

    void getNumTypeQX();
    int  getBoardScore();
    uint32 getPriority(int _x,int _y);

    void SwapBoardColor();

    void takeChess(int x,int y);
    void putChess(int x,int y,uint8 AIorOP,int color);

    int  MiniMaxSearch(uint32 depth, int alpha, int beta);

public:
    AI();
    std::pair<int, int> AIMakeADecision(int Board[][15], int crtDecisionNum);

};
#endif
// AIEMA_H

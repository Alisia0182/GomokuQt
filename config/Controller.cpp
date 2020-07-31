#include "Debug.h"
#include "Controller.h"
#include <string.h>
#include <QDebug>
Controller::Controller():
    ema()
{}

bool Controller::isWin(std::pair<int,int> crtChoice)
{
    int x = crtChoice.first;
    int y = crtChoice.second;

    int x1 = x - 4,y1 = y;

    for(; x1 <= x; ++x1)
        if(x1 >= 0 && x1 + 4 < 15
           && GameBoard[x1 + 1][y1] == GameBoard[x1][y1]
           && GameBoard[x1 + 2][y1] == GameBoard[x1][y1]
           && GameBoard[x1 + 3][y1] == GameBoard[x1][y1]
           && GameBoard[x1 + 4][y1] == GameBoard[x1][y1])
            return true;

    x1 = x; y1 = y - 4;
    for(;y1 <= y; ++y1)
        if(y1 >= 0 && y1 + 4 < 15
           && GameBoard[x1][y1 + 1] == GameBoard[x1][y1]
           && GameBoard[x1][y1 + 2] == GameBoard[x1][y1]
           && GameBoard[x1][y1 + 3] == GameBoard[x1][y1]
           && GameBoard[x1][y1 + 4] == GameBoard[x1][y1])
            return true;

    x1 = x - 4;y1 = y - 4;
    for(;x1 <= x; ++x1,++y1)
        if(x1 >= 0 && y1 >= 0 && x1 + 4 < 15 && y1 + 4 < 15
           &&GameBoard[x1 + 1][y1 + 1] == GameBoard[x1][y1]
           &&GameBoard[x1 + 2][y1 + 2] == GameBoard[x1][y1]
           &&GameBoard[x1 + 3][y1 + 3] == GameBoard[x1][y1]
           &&GameBoard[x1 + 4][y1 + 4] == GameBoard[x1][y1])
            return true;

    x1 = x - 4;y1 = y + 4;
    for(;x1 <= x; ++x1,--y1){
        if(x1 >= 0 && x1 + 4 < 15 && y1 < 15 && y1 - 4 >= 0
           && GameBoard[x1 + 1][y1 - 1] == GameBoard[x1][y1]
           && GameBoard[x1 + 2][y1 - 2] == GameBoard[x1][y1]
           && GameBoard[x1 + 3][y1 - 3] == GameBoard[x1][y1]
           && GameBoard[x1 + 4][y1 - 4] == GameBoard[x1][y1])
            return true;
}
    return false;
}

inline void Controller::SwapBoard()
{
    for(int i = 0;i < 15; ++i)
        for(int j = 0;j < 15; ++j)
        {
            if(GameBoard[i][j] != -1)
                GameBoard[i][j] = 1 - GameBoard[i][j];
        }
}

bool Controller::PutHumanChess(std::pair<int, int> HumanChess)
{

    if(HumanChess.first >= 0){
        GameBoard[HumanChess.first][HumanChess.second] = crtColor;\
    }else{
        SwapBoard();
    }

    History[crtDecisionNum++] = HumanChess;
    if(crtDecisionNum >= 9)
        return isWin(HumanChess);
    else
        return false;
}



bool Controller::PutAIChess()
{

    std::pair<int,int> AIChess = ema.AIMakeADecision(GameBoard,crtDecisionNum);
#ifdef PVEDebug
    qDebug() << "AI:\t" << AIChess.first << ' ' << AIChess.second;
#endif
    if(AIChess.first >= 0){
        GameBoard[AIChess.first][AIChess.second] = crtColor;
    }
    else {
        SwapBoard();
    }
    History[crtDecisionNum++] = AIChess;
    if(crtDecisionNum >= 9)return isWin(AIChess);
    else return false;
}

void Controller::ChangePlayer()
{
    crtColor = !crtColor;
    if(crtMode == PVE){

        if(crtStatus == AIThinking)
            crtStatus = HumanThinking;
        else if(crtStatus == HumanThinking)
            crtStatus = AIThinking;

        if(crtPlayerSide == EMA)
            crtPlayerSide = H1;
        else if(crtPlayerSide == H1)
            crtPlayerSide = EMA;
    }
    else if(crtMode == PVP)
    {
        if(crtPlayerSide == H1)
            crtPlayerSide = H2;
        else if(crtPlayerSide == H2)
            crtPlayerSide = H1;
    }
}
void Controller::initPVEGame(bool H1Side)
{
    crtMode = PVE;
    crtColor = 0;

    if(H1Side) {
        crtPlayerSide = EMA;
        crtStatus = AIThinking;
    }else  {
        crtPlayerSide = H1;
        crtStatus = HumanThinking;
    }
    crtDecisionNum = 0;
    crtH1Side = H1Side;
    memset(GameBoard, -1,sizeof(GameBoard));
    memset(History,0,sizeof(History));

    if(crtStatus == AIThinking){
        PutAIChess();
        ChangePlayer();
    }
}

void Controller::initPVPGame(bool H1Side)
{
    crtMode = PVP;
    crtColor = 0;

    if(H1Side) {
        crtPlayerSide = H2;
    }else  {
        crtPlayerSide = H1;
    }

    crtStatus = HumanThinking;
    crtDecisionNum = 0;
    crtH1Side = H1Side;

    memset(GameBoard, -1,sizeof(GameBoard));
    memset(History,0,sizeof(History));

}

void Controller::Retract()
{
    std::pair<int,int> recent;
    if(crtMode == PVP){
        if(crtDecisionNum >= 1)
        {
            recent = History[--crtDecisionNum];

            if(recent.first < 0)
            {
                SwapBoard();
            }
            else
            {
                GameBoard[recent.first][recent.second] = -1;
            }
            ChangePlayer();
        }
    }else if(crtMode == PVE && crtPlayerSide != EMA)
    {
        if(crtDecisionNum >= 2){

            recent = History[--crtDecisionNum];
            if(recent.first < 0)
            {
                SwapBoard();
            }else{
                GameBoard[recent.first][recent.second] = -1;
            }

            recent = History[--crtDecisionNum];
            if(recent.first < 0)
            {
                SwapBoard();
            }else{
                GameBoard[recent.first][recent.second] = -1;
            }
        }
    }
}




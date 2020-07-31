#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "AIEma.h"
#include <utility>

enum GameMode
{
    PVP,
    PVE,
};

enum GameStatus
{
    AIThinking,
    HumanThinking,
    H1WIN,
    H1LOSE
};
enum PlayerSide
{
    EMA,
    H1,
    H2
};

class Controller{
private:
    AI ema;

    std::pair<int,int> History[225];

    bool isWin(std::pair<int,int> crtChoice);
    inline void SwapBoard();

public:

    int  crtDecisionNum;
    bool crtH1Side;

    bool crtColor;
    int GameBoard[15][15];
    GameStatus crtStatus;
    GameMode   crtMode;
    PlayerSide crtPlayerSide;
    void ChangePlayer();

    Controller();

    void initPVEGame(bool H1Side);
    void initPVPGame(bool H1Side);

    bool PutHumanChess(std::pair<int,int>HumanChess);
    bool PutAIChess();

    void Retract();


};

#endif
// CONTROLLER_H

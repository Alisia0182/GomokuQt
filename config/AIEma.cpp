#include "AIEma.h"
#include <queue>

AI::AI():
    AiSide(0)
{
    initHash();
    initCnts();
    memset(AIboard, -1, sizeof(AIboard));
}

void AI::initHash()
{
    memset(HashTable,0,sizeof(HashTable));

    for(uint32 i = 0;i < 5; ++i){
        HashTable[0b01 << (i << 1)] = AI_1;
        HashTable[0b10 << (i << 1)] = OP_1;
    }

    for(uint32 i1 = 0;i1 < 4; ++ i1)
        for(uint32 i2 = i1 + 1; i2 < 5; ++ i2){
            HashTable[(0b01 << (i1 << 1)) | (0b01 << (i2 << 1))] = AI_2;
            HashTable[(0b10 << (i1 << 1)) | (0b10 << (i2 << 1))] = OP_2;
        }

    for(uint32 i1 = 0;i1 < 3; ++ i1)
        for(uint32 i2 = i1 + 1; i2 < 4; ++ i2)
            for(uint32 i3 = i2 + 1;i3 < 5; ++ i3){
                HashTable[(0b01 << (i1 << 1)) | (0b01 << (i2 << 1)) | (0b01 << (i3 << 1))] = AI_3;
                HashTable[(0b10 << (i1 << 1)) | (0b10 << (i2 << 1)) | (0b10 << (i3 << 1))] = OP_3;
            }

    HashTable[0b0101010100] = AI_4;
    HashTable[0b0101010001] = AI_4;
    HashTable[0b0101000101] = AI_4;
    HashTable[0b0100010101] = AI_4;
    HashTable[0b0001010101] = AI_4;

    HashTable[0b1010101000] = OP_4;
    HashTable[0b1010100010] = OP_4;
    HashTable[0b1010001010] = OP_4;
    HashTable[0b1000101010] = OP_4;
    HashTable[0b0010101010] = OP_4;

    HashTable[0b0101010101] = AI_5;
    HashTable[0b1010101010] = OP_5;
}

void AI::initCnts()
{
    memset(Row,0,sizeof(Row));
    memset(Col,0,sizeof(Col));
    memset(NorthEast,0,sizeof(NorthEast));
    memset(SouthEast,0,sizeof(SouthEast));
    memset(AINumTypeQX,0,sizeof(AINumTypeQX));
    memset(OPNumTypeQX,0,sizeof(OPNumTypeQX));
}

void AI::putChess(int x,int y,uint8 AIorOP,int color)
{
    AIboard[x][y] = color;
    Row[x] |= (AIorOP << (28 - y * 2 ));
    Col[y] |= (AIorOP << (28 - x * 2 ));

    if(x + y - 4 >= 0 && x + y - 4 < 21) {
        uint32 NorthEastIndex = x + y - 4;
        if (NorthEastIndex < 11) {
            NorthEast[NorthEastIndex] |= (AIorOP << (x << 1));
        } else {
            NorthEast[NorthEastIndex] |= (AIorOP << (28 - (y << 1)));
        }
    }

    if(x - y + 10 >= 0 && x - y + 10 < 21) {
        uint32 SouthEastIndex = x - y + 10;
        if (SouthEastIndex < 11) {
            SouthEast[SouthEastIndex] |= (AIorOP << (28 - (y << 1)));
        } else {
            SouthEast[SouthEastIndex] |= (AIorOP << (28 - (x << 1)));
        }
    }
}

void AI::takeChess(int x,int y)
{
    AIboard[x][y] = -1;
    Row[x] &= ~(0b11 << (28 - (y << 1)));
    Col[y] &= ~(0b11 << (28 - (x << 1)));

    if(x + y - 4 >= 0 && x + y - 4 < 21) {
        uint32 NorthEastIndex = x + y - 4;
        if (NorthEastIndex < 11) {
            NorthEast[NorthEastIndex] &= ~(0b11 << (x << 1));
        } else {
            NorthEast[NorthEastIndex] &= ~(0b11 << (28 - (y << 1)));
        }
    }

    if(x - y + 10 >= 0 && x - y + 10 < 21) {
        uint32 SouthEastIndex = x - y + 10;
        if (SouthEastIndex < 11) {
            SouthEast[SouthEastIndex] &= ~(0b11 << (28 - (y << 1)));
        } else {
            SouthEast[SouthEastIndex] &= ~(0b11 << (28 - (x << 1)));
        }
    }
}

void AI::SwapBoardColor(){
    Point tmp;
    bool color;
    for(int i = 0;i < 3; ++i){
        tmp = FirstThree[i];
        color = !AIboard[tmp.x][tmp.y];
        takeChess(tmp.x,tmp.y);
        if(AiSide == color)
            putChess(tmp.x,tmp.y,0b01,color);
        else
            putChess(tmp.x,tmp.y,0b10,color);
    }
}

uint32 AI::getPriority(int _x, int _y){
    uint8 emptyNum = 0,
          BlackNum = 0,
          WhiteNum = 0,
          num      = 0;

    uint8   northSide = _x - 4 > 0  ? _x - 4 : 0,
            southSide = _x + 4 < 14 ? _x + 4 : 14,
            westSide  = _y - 4 > 0  ? _y - 4 : 0,
            eastSide  = _y + 4 < 14 ? _y + 4 : 14;

    uint8 xp1 = _x, xp2 = _x,
          yp1 = westSide,
          yp2 = westSide;

    uint32 NumTypeQX[5];
    memset(NumTypeQX,0,sizeof(NumTypeQX));

    for(;yp2 <= eastSide; ++ yp2){
        ++num;
        if(AIboard[xp2][yp2] == -1) ++emptyNum;
        else if(AIboard[xp2][yp2] == 0) ++BlackNum;
        else if(AIboard[xp2][yp2] == 1) ++WhiteNum;
        if(num == 5){
            if(emptyNum + BlackNum == 5 || emptyNum + WhiteNum == 5)
                ++ NumTypeQX[4 - emptyNum];

            --num;++yp1;
            if(AIboard[xp1][yp1] == -1) --emptyNum;
            else if(AIboard[xp1][yp1] == 0) --BlackNum;
            else if(AIboard[xp1][yp1] == 1) --WhiteNum;
        }
    }

    num = 0;
    emptyNum = BlackNum = WhiteNum = 0;
    xp1 = xp2 = northSide;
    yp1 = yp2 = _y;
    for(;xp2 <= southSide; ++ xp2){
        ++ num;
        if(AIboard[xp2][yp2] == -1) ++ emptyNum;
        else if(AIboard[xp2][yp2] == 0) ++ BlackNum;
        else if(AIboard[xp2][yp2] == 1) ++ WhiteNum;

        if(num == 5){
            if(emptyNum + BlackNum == 5 || emptyNum + WhiteNum == 5)
                ++ NumTypeQX[4 - emptyNum];

            --num; ++xp1;
            if(AIboard[xp1][yp1] == -1) --emptyNum;
            else if(AIboard[xp1][yp1] == 0) -- BlackNum;
            else if(AIboard[xp1][yp1] == 1) --WhiteNum;
        }
    }

    num = 0;
    emptyNum = BlackNum = WhiteNum = 0;
    xp1 = xp2 = _x - 4;
    yp1 = yp2 = _y - 4;
    for(;xp2 <= southSide && yp2 <= eastSide; ++ xp2, ++ yp2){
        if(xp2 < northSide || yp2 < westSide){ //解决西北越界
            ++xp1; ++yp1;
            continue;
        }
        ++ num;
        if (AIboard[xp2][yp2] == -1) ++emptyNum;
        else if (AIboard[xp2][yp2] == 0) ++BlackNum;
        else if (AIboard[xp2][yp2] == 1) ++WhiteNum;

        if(num == 5){
            if(emptyNum + BlackNum == 5 || emptyNum + WhiteNum == 5)
                ++ NumTypeQX[4 - emptyNum];

            --num; ++ xp1; ++ yp1;
            if(AIboard[xp1][yp1] == -1) --emptyNum;
            else if(AIboard[xp1][yp1] == 0) --BlackNum;
            else if(AIboard[xp1][yp1] == 1) --WhiteNum;
        }
    }

    num = 0;
    emptyNum = BlackNum = WhiteNum = 0;
    xp1 = xp2 = _x + 4;
    yp1 = yp2 = _y - 4;
    for(;xp2 >= northSide && yp2 <= eastSide; --xp2, ++ yp2){
        if(xp2 > southSide || yp2 < westSide){//解决西南越界
            --xp1; ++ yp1;
            continue;
        }
        ++num;
        if(AIboard[xp2][yp2] == -1) ++emptyNum;
        else if(AIboard[xp2][yp2] == 0) ++ BlackNum;
        else if(AIboard[xp2][yp2] == 1) ++ WhiteNum;

        if(num == 5){
            if(emptyNum + BlackNum == 5 || emptyNum + WhiteNum == 5)
                ++ NumTypeQX[4 - emptyNum];

            --num; --xp1; ++ yp1;
            if(AIboard[xp1][yp1] == -1) --emptyNum;
            else if(AIboard[xp1][yp1] == 0) -- BlackNum;
            else if(AIboard[xp1][yp1] == 1) -- WhiteNum;
        }
    }
    uint32 rst(0);
    rst += NumTypeQX[0] * score0;
    rst += NumTypeQX[1] * score1;
    rst += NumTypeQX[2] * score2;
    rst += NumTypeQX[3] * score3;
    rst += NumTypeQX[4] * score4;

    return rst;
}

void AI::getNumTypeQX(){
    memset(AINumTypeQX,0,sizeof(AINumTypeQX));
    memset(OPNumTypeQX,0,sizeof(OPNumTypeQX));

    uint32 crtLine;
    typeQX tmpQX;

    for(int i = 0;i < 15; ++i){
        crtLine = Row[i];
        for(int j = 0;j < 11; ++j){
            tmpQX = HashTable[crtLine & TenOne];
            if(tmpQX){
                if(tmpQX > 5)
                    ++OPNumTypeQX[tmpQX - 6];
                else
                    ++AINumTypeQX[tmpQX - 1];
            }
            crtLine >>= 2;
        }
    }

    for(int i = 0;i < 15; ++i){
        crtLine = Col[i];
        for(int j = 0;j < 11; ++j){
            tmpQX = HashTable[crtLine & TenOne];
            if(tmpQX){
                if(tmpQX > 5)
                    ++OPNumTypeQX[tmpQX - 6];
                else
                    ++AINumTypeQX[tmpQX - 1];
            }
            crtLine >>= 2;
        }
    }

    int Counter;

    for(int i = 0;i < 21; ++i){
        crtLine = NorthEast[i];
        if(i < 11) Counter = i + 1;
        else Counter = 21 - i;
        for(int j = 0;j < Counter; ++j){
            tmpQX = HashTable[crtLine & TenOne];
            if(tmpQX){
                if(tmpQX > 5)
                    ++OPNumTypeQX[tmpQX - 6];
                else
                    ++AINumTypeQX[tmpQX - 1];
            }
            crtLine >>= 2;
        }
    }

    for(int i = 0;i < 21; ++i){
        crtLine = SouthEast[i];
        if(i < 11) Counter = i + 1;
        else Counter = 21 - i;
        for(int j = 0;j < Counter; ++j){
            tmpQX = HashTable[crtLine & TenOne];
            if(tmpQX){
                if(tmpQX > 5)
                    ++OPNumTypeQX[tmpQX - 6];
                else
                    ++AINumTypeQX[tmpQX - 1];
            }
            crtLine >>= 2;
        }
    }
}

int AI::getBoardScore()
{
    int rst = 0;

    rst += AINumTypeQX[0] * score0;
    rst += AINumTypeQX[1] * score1;
    rst += AINumTypeQX[2] * score2;
    rst += AINumTypeQX[3] * score3;
    rst += AINumTypeQX[4] * score4;

    rst -= OPNumTypeQX[0] * score0;
    rst -= OPNumTypeQX[1] * score1;
    rst -= OPNumTypeQX[2] * score2;
    rst -= OPNumTypeQX[3] * score3;
    rst -= OPNumTypeQX[4] * score4;

    return rst;
}

std::pair<int, int> AI::AIMakeADecision
(int (*Board)[15],int crtDecisionNum)
{
     crtDecNum = crtDecisionNum;
     if(crtDecisionNum == 0){
         return std::make_pair(0,0);
     }

     if(crtDecisionNum % 2)
         AiSide = 1;
     else
         AiSide = 0;

     int cnt = 0;
     for(int i = 0;i < 15; ++ i)
         for(int j = 0;j < 15; ++j)
     {
        AIboard[i][j] = Board[i][j];
        if(crtDecisionNum <= 3 && Board[i][j] != -1){
            FirstThree[cnt].x   = i;
            FirstThree[cnt++].y = j;
        }
     }

     if(crtDecisionNum <= 2)
     {
         do{
            NextStep.first = FirstThree[crtDecisionNum - 1].x  + (rand() % 3) - 1;
            NextStep.second = FirstThree[crtDecisionNum - 1].y  + (rand() % 3) - 1;
         }while(NextStep.first < 1 || NextStep.first > 13
                || NextStep.second < 1 || NextStep.second > 13
                ||AIboard[NextStep.first][NextStep.second] != -1);
         return NextStep;
     }

     MiniMaxSearch(0, -INF, INF);
     return NextStep;
}

int AI::MiniMaxSearch(uint32 depth, int alpha, int beta){
    if(depth == MaxSearchDepth){
        getNumTypeQX();
        return getBoardScore();
    }

    std::priority_queue<Point> ChoiceBuffer;
    Point choice;
    int   score;

    for(uint32 i = 0;i < 15; ++i)
        for(uint32 j = 0;j < 15; ++j){
            if(AIboard[i][j] == -1){
                choice.x = i;
                choice.y = j;
                choice.priority = getPriority(choice.x,choice.y);
                ChoiceBuffer.push(choice);
            }

        }

    if(crtDecNum == 3 && depth == 0 ){
        Point tmp;
        tmp.x = -1;
        tmp.y = -1;
        ChoiceBuffer.push(tmp);
    }

    if(! depth % 2){
        while(!ChoiceBuffer.empty()){
            choice = ChoiceBuffer.top();
            ChoiceBuffer.pop();

            if(choice.x == -1)
                SwapBoardColor();
            else
                putChess(choice.x,choice.y,0b01,AiSide);

            score = MiniMaxSearch(depth + 1, alpha,beta);

            if(choice.x == -1)
                SwapBoardColor();
            else
                takeChess(choice.x,choice.y);

            if(score > alpha){
                alpha = score;
                if(!depth){
                    NextStep = std::make_pair(choice.x,choice.y);
                }
            }
            if(alpha >= beta){
                return alpha;
            }
        }
        return alpha;
    }
    else{
        while(!ChoiceBuffer.empty()){
         choice = ChoiceBuffer.top();
         ChoiceBuffer.pop();

         putChess(choice.x,choice.y,0b10,1-AiSide);
         score = MiniMaxSearch(depth + 1,alpha,beta);
         takeChess(choice.x,choice.y);

         if(score < beta){
             beta = score;
         }
         if(alpha >= beta){
             return beta;
         }
        }
        return beta;
    }
}

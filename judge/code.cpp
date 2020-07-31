#include "AIController.h"
#include <queue>
#include <utility>
#include <cstring>

typedef unsigned char uint8;
typedef unsigned char typeQX;
typedef unsigned int  uint32;

struct point{
    int x;
    int y;
    uint32 priority;

    bool operator<(const point & other) const{
        return this->priority < other.priority;
    }

    point(uint32 _x, uint32 _y);
    point():
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
#define score4 625
#define INF                0x3f3f3f3f
#define TenOne             0x3ff
#define HashTableSize      0x2ab //0b1010101011

extern int  ai_side;
std::string ai_name = "Ema";

int turn = 0;
int board[15][15];

uint32 Row[15],
       Col[15],
       NorthEast[21],
       SouthEast[21],
       AINumTypeQX[5],
       OPNumTypeQX[5];
typeQX HashTable[HashTableSize];
point  First,   Second,
       Third,   NextStep;

int  MiniMaxSearch(uint32 depth, int alpha, int beta);
int  getBoardScore();
void getNumTypeQX();
void SwapBoardColor();
void ReSwBoardColor();
void takeChess(uint32 x,uint32 y);
void putChess(uint32 x,uint32 y,uint8 AIorOP,int color);

void init() {
    memset(board, -1, sizeof(board));
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

std::pair<int, int> action(std::pair<int, int> loc) {
    ++turn;

    if(turn == 1){
        if(ai_side == 0){
            First.x = First.y = 0;
            putChess(First.x,First.y,0b01,ai_side);
            return std::make_pair(First.x,First.y);
        }
        else {
            First.x = loc.first; First.y = loc.second;
            putChess(loc.first,loc.second,0b10,1 - ai_side);

            uint32 xp, yp;
            do{
               xp = loc.first  + (rand() % 3) - 1;
               yp = loc.second + (rand() % 3) - 1;
            }while(xp < 1 || xp > 13 || yp < 1 || yp > 13 || (xp == loc.first && yp == loc.second));

            Second.x = xp; Second.y = yp;
            putChess(xp,yp,0b01,ai_side);
            return std::make_pair(xp,yp);
        }
    }
    else if(turn == 2){
        if(ai_side == 0){
            Second.x = loc.first;
            Second.y = loc.second;
            putChess(loc.first,loc.second,0b10,1 - ai_side);

            uint32 xp, yp;
            do{
                xp = loc.first  + (rand() % 3) - 1;
                yp = loc.second + (rand() % 3) - 1;
            }while(xp < 1 || xp > 13 || yp < 1 || yp > 13 || board[xp][yp] != -1);

            /*
            if(loc.first == 7 && loc.second > 5 && loc.second < 8){
                NextStep.y = loc.second;
                NextStep.x = 6 + ((rand() % 2) * 2);
            }
            else if(loc.second == 7 && loc.first > 5 && loc.first < 8){
                NextStep.x = loc.first;
                NextStep.y = 6 + (rand() % 2) * 2;
            }
            else if(loc.first > 5 && loc.first < 8 && loc.second > 5 && loc.second < 8){
                if(rand() % 2){
                    NextStep.x = loc.first;
                    NextStep.y = 14 - loc.second;
                }
                else{
                    NextStep.y = loc.second;
                    NextStep.x = 14 - loc.first;
                }
            }
            else {
            NextStep.x = 6 + (rand() % 2) * 2;
            NextStep.y = 6 + (rand() % 2) * 2;
            }*/
            NextStep.x = xp;
            NextStep.y = yp;

            Third.x = NextStep.x;
            Third.y = NextStep.y;
            putChess(NextStep.x,NextStep.y,0b01,ai_side);

            return std::make_pair(NextStep.x,NextStep.y);
        }
        else{
            Third.x = loc.first;
            Third.y = loc.second;
        }
    }

    if(loc.first == -1 && turn == 3 && ai_side == 0)//对方选了swap
        SwapBoardColor();
    else
        putChess(loc.first,loc.second,0b10,1 - ai_side);


    MiniMaxSearch(0,-INF,INF);
    if(NextStep.x == -1) {
        SwapBoardColor();
        return std::make_pair(-1,-1);
    }
    else
        putChess(NextStep.x,NextStep.y,0b01,ai_side);

    return std::make_pair(NextStep.x,NextStep.y);
}

int MiniMaxSearch(uint32 depth, int alpha, int beta){

    if(depth == MaxSearchDepth){
        getNumTypeQX();
        return getBoardScore();
    }

    std::priority_queue<point> ChoiceBuffer;
    point choice;
    int   score;

    for(uint32 i = 0;i < 15; ++i)
        for(uint32 j = 0;j < 15; ++j){
            if(board[i][j] == -1)
                ChoiceBuffer.push(point(i,j));
        }

    if(turn == 2 && depth == 0 && ai_side == 1){
        point tmp;
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
                putChess(choice.x,choice.y,0b01,ai_side);

            score = MiniMaxSearch(depth + 1, alpha,beta);

            if(choice.x == -1)
                ReSwBoardColor();
            else
                takeChess(choice.x,choice.y);

            if(score > alpha){
                alpha = score;
                if(!depth){
                    NextStep.x = choice.x;
                    NextStep.y = choice.y;
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

         putChess(choice.x,choice.y,0b10,1-ai_side);
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
void getNumTypeQX(){
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

    uint32 Counter;

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

int getBoardScore(){
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

point::point(uint32 _x, uint32 _y):
    x(_x),y(_y){
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
        if(board[xp2][yp2] == -1) ++emptyNum;
        else if(board[xp2][yp2] == 0) ++BlackNum;
        else if(board[xp2][yp2] == 1) ++WhiteNum;
        if(num == 5){
            if(emptyNum + BlackNum == 5 || emptyNum + WhiteNum == 5)
                ++ NumTypeQX[4 - emptyNum];

            --num;++yp1;
            if(board[xp1][yp1] == -1) --emptyNum;
            else if(board[xp1][yp1] == 0) --BlackNum;
            else if(board[xp1][yp1] == 1) --WhiteNum;
        }
    }

    num = 0;
    emptyNum = BlackNum = WhiteNum = 0;
    xp1 = xp2 = northSide;
    yp1 = yp2 = _y;
    for(;xp2 <= southSide; ++ xp2){
        ++ num;
        if(board[xp2][yp2] == -1) ++ emptyNum;
        else if(board[xp2][yp2] == 0) ++ BlackNum;
        else if(board[xp2][yp2] == 1) ++ WhiteNum;

        if(num == 5){
            if(emptyNum + BlackNum == 5 || emptyNum + WhiteNum == 5)
                ++ NumTypeQX[4 - emptyNum];

            --num; ++xp1;
            if(board[xp1][yp1] == -1) --emptyNum;
            else if(board[xp1][yp1] == 0) -- BlackNum;
            else if(board[xp1][yp1] == 1) --WhiteNum;
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
        if (board[xp2][yp2] == -1) ++emptyNum;
        else if (board[xp2][yp2] == 0) ++BlackNum;
        else if (board[xp2][yp2] == 1) ++WhiteNum;

        if(num == 5){
            if(emptyNum + BlackNum == 5 || emptyNum + WhiteNum == 5)
                ++ NumTypeQX[4 - emptyNum];

            --num; ++ xp1; ++ yp1;
            if(board[xp1][yp1] == -1) --emptyNum;
            else if(board[xp1][yp1] == 0) --BlackNum;
            else if(board[xp1][yp1] == 1) --WhiteNum;
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
        if(board[xp2][yp2] == -1) ++emptyNum;
        else if(board[xp2][yp2] == 0) ++ BlackNum;
        else if(board[xp2][yp2] == 1) ++ WhiteNum;

        if(num == 5){
            if(emptyNum + BlackNum == 5 || emptyNum + WhiteNum == 5)
                ++ NumTypeQX[4 - emptyNum];

            --num; --xp1; ++ yp1;
            if(board[xp1][yp1] == -1) --emptyNum;
            else if(board[xp1][yp1] == 0) -- BlackNum;
            else if(board[xp1][yp1] == 1) -- WhiteNum;
        }
    }

    this->priority  = 0;
    priority += NumTypeQX[0] * score0;
    priority += NumTypeQX[1] * score1;
    priority += NumTypeQX[2] * score2;
    priority += NumTypeQX[3] * score3;
    priority += NumTypeQX[4] * score4;
}

void takeChess(uint32 x,uint32 y){
    board[x][y] = -1;
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

void putChess(uint32 x,uint32 y,uint8 AIorOP,int color){
    board[x][y] = color;
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

void SwapBoardColor(){
    takeChess( First.x, First.y);
    takeChess(Second.x,Second.y);
    takeChess( Third.x, Third.y);

    putChess(First.x ,First.y ,0b10 - ai_side,1);
    putChess(Second.x,Second.y,0b01 + ai_side,0);
    putChess(Third.x ,Third.y ,0b10 - ai_side,1);
}

void ReSwBoardColor(){
    takeChess(First.x ,First.y );
    takeChess(Second.x,Second.y);
    takeChess(Third.x ,Third.y );

    putChess(First.x ,First.y ,0b01 + ai_side,0);
    putChess(Second.x,Second.y,0b10 - ai_side,1);
    putChess(Third.x ,Third.y ,0b01 + ai_side,0);
}

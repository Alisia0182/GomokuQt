#include "Debug.h"
#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMouseEvent>
#include <QTimer>
#include "GamePainter.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    GamePtr = new Controller;
    PVEGameNum = PVPGameNum = 0;
    AISucNum = 0;
    H1SucNum = 0;
    H2SucNum = 0;

    setMouseTracking(true);
    setFixedSize(GradSize * GradNumWidth,
                 GradSize * GradNumHeight);

    initButton();
    initConnections();
    //SetPVPGame(0);
    SetPVEGame(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::paintEvent(QPaintEvent * event)
{
    GamePainter painter(this);
    painter.DrawGrid();
    painter.DrawText();
    QFont font;
    font.setFamily("Microsoft Yahei");
    font.setPointSize(10);
    font.setCapitalization(QFont::SmallCaps);
    painter.setFont(font);

    painter.drawText(GradSize * (WestSideinGrad + 14 + 2),
             GradSize * (NorthSideinGrad + 2 ),
             "战绩：" + QString::number(AISucNum) + "/" + QString::number(PVEGameNum));

    painter.drawText(GradSize * (WestSideinGrad + 14 + 2),
             GradSize * (NorthSideinGrad + 7 ),
             "战绩：" + QString::number(H1SucNum) + "/" + QString::number(PVEGameNum + PVPGameNum));

    painter.drawText(GradSize * (WestSideinGrad + 14 + 2),
             GradSize * (NorthSideinGrad + 12 ),
             "战绩：" + QString::number(H2SucNum) + "/" + QString::number(PVPGameNum));

    painter.DrawChess(GamePtr->GameBoard);

    if(GamePtr->crtStatus == HumanThinking
       && GamePtr->GameBoard[NearestPoint.first][NearestPoint.second] == -1 )
        painter.DrawMouseMark(NearestPoint);
}

void MainWindow::initButton()
{
    for(int i = 0;i < 5; ++ i)
    {
        ControlButton[i].setParent(this);
        ControlButton[i].setGeometry(ControlButtonWest + i * ControlButtonSpacing,
                                     ControlButtonNorth,
                                     ControlButtonWidth,
                                     ControlButtonHeight);
        ControlButton[i].setStyleSheet(ControlButtonStyle);
    }

    for(int i = 0;i < 2; ++ i)
    {
        TitleButton[i].setParent(this);
        TitleButton[i].setGeometry(TitleButtonWest + i * TitleButtonSpacing,
                                   TitleButtonNorth,
                                   TitleButtonWidth,
                                   TitleButtonHeight);
        TitleButton[i].setStyleSheet(TitleButtonStyle[i]);

    }

    TitleButton[0].setText("—");
    TitleButton[1].setText("×");

    ControlButton[0].setText("当前：人机");
    ControlButton[1].setText("H1执黑");
    ControlButton[2].setText("重开");
    ControlButton[3].setText("悔棋");
    ControlButton[4].setText("Swap");

}

void MainWindow::initConnections()
{

    connect(TitleButton + 0, &UIButton::clicked,this,&MainWindow::showMinimized);
    connect(TitleButton + 1, &UIButton::clicked,this,&MainWindow::close);

    connect(ControlButton + GameModeButton, &UIButton::clicked,
                [&]()
                {
                    if(ControlButton[0].text() == "当前：人机")
                    {
                        ControlButton[0].setText("当前：人人");
                        SetPVPGame(GamePtr->crtH1Side);
                    }else if(ControlButton[0].text() == "当前：人人")
                    {
                        ControlButton[0].setText("当前：人机");
                        SetPVEGame(GamePtr->crtH1Side);
                    }
                }
            );

    connect(ControlButton + H1ColorButton, &UIButton::clicked,
                [&]()
                {
                    /*
                    if      (ControlButton[1].text() == "当前：H1执黑")
                    {
                             ControlButton[1].setText("当前：H1执白");
                    }else if(ControlButton[1].text() == "当前：H1执白")
                    {
                             ControlButton[1].setText("当前：H1执黑");
                    }*/
                    if(ControlButton[1].text() == "H1执黑")
                    {
                        ControlButton[1].setText("H1执白");
                    }
                    else if(ControlButton[1].text() == "H1执白")
                    {
                        ControlButton[1].setText("H1执黑");
                    }else if(ControlButton[1].text() == "H2执黑")
                    {
                        ControlButton[1].setText("H2执白");
                    }else if(ControlButton[1].text() == "H2执白")
                    {
                        ControlButton[1].setText("H2执黑");
                    }
                    if(ControlButton[1].text() != "AI执白"
                    && ControlButton[1].text() != "AI执黑")
                    {
                        if(GamePtr->crtMode == PVP)
                            SetPVPGame(!GamePtr->crtH1Side);
                        else if(GamePtr->crtMode == PVE)
                            SetPVEGame(!GamePtr->crtH1Side);
                    }
                }
            );

    connect(ControlButton + RestartButton, &UIButton::clicked,
                [&]()
                {
                    if(GamePtr->crtMode == PVP)
                        SetPVPGame(GamePtr->crtH1Side);
                    else if(GamePtr->crtMode == PVE)
                        SetPVEGame(GamePtr->crtH1Side);
                }
            );

    connect(ControlButton + RetractButton, &UIButton::clicked,
                [&]()
                {
                    GamePtr->Retract();
                    update();
                }
            );


    connect(ControlButton + SwapButton, &UIButton::clicked,
                [&]()
                {
                    if(GamePtr->crtDecisionNum == 3
                    && GamePtr->crtPlayerSide  != EMA)
                    {
                        GamePtr->PutHumanChess(std::make_pair(-1,-1));
                        update();

                        GamePtr->ChangePlayer();
                        if(GamePtr->crtPlayerSide == EMA){
                            GamePtr->PutAIChess();
                            update();
                            GamePtr->ChangePlayer();
                        }
                    }
                }
            );
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    int x = event->x();
    int y = event->y();

    if(   x > GradSize *  WestSideinGrad  - RecogRadius
       && x < GradSize * (WestSideinGrad  + 14) + RecogRadius
       && y > GradSize *  NorthSideinGrad - RecogRadius
       && y < GradSize * (NorthSideinGrad + 14) + RecogRadius)
    {
        int coord1 = (y - GradSize * NorthSideinGrad) / GradSize;
        int coord2 = (x - GradSize * WestSideinGrad ) / GradSize;

        int distSquare;

        for(int i = 0;i < 2; ++i, ++coord1)
            for(int j = 0;j < 2; ++j, ++coord2)
            {
                distSquare =  (x - coord2 * GradSize - GradSize * WestSideinGrad)
                            * (x - coord2 * GradSize - GradSize * WestSideinGrad)
                            + (y - coord1 * GradSize - GradSize * NorthSideinGrad)
                            * (y - coord1 * GradSize - GradSize * NorthSideinGrad);
                if((distSquare < RecogRadius * RecogRadius)
                        && coord1 >= 0  && coord1 < 15
                        && coord2 >= 0  && coord2 < 15
                        && GamePtr->GameBoard[coord1][coord2] == -1
                        )
                {

                    NearestPoint.first  = coord1;
                    NearestPoint.second = coord2;
                    update();
                    return;
                }
            }
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{

    if(GamePtr->crtStatus == HumanThinking
       && GamePtr->GameBoard[NearestPoint.first][NearestPoint.second] == -1)
    {
        if(GamePtr->crtMode == PVE)
        {
            PVERound();
        }else if(GamePtr->crtMode == PVP)
        {
            PVPRound();
        }
    }
}
void MainWindow::SetPVEGame(bool h1Side)
{
    if(h1Side){
        ControlButton[1].setText("AI执黑");
    }
    else {
        ControlButton[1].setText("H1执黑");
    }
    GamePtr->initPVEGame(h1Side);

    if(ControlButton[1].text() == "AI执黑")
        ControlButton[1].setText("H1执白");

    update();
}

void MainWindow::SetPVPGame(bool h1Side)
{
    if(h1Side){
        ControlButton[1].setText("H2执黑");
    }
    else {
        ControlButton[1].setText("H1执黑");
    }
    GamePtr->initPVPGame(h1Side);
    update();
}

void MainWindow::PVERound()
{
    bool Win = GamePtr->PutHumanChess(NearestPoint);

    update();

    if(Win){
      ++PVEGameNum;
      ++H1SucNum;
      SetPVEGame(GamePtr->crtH1Side);
    }
    else{

      GamePtr->ChangePlayer();

      if(ControlButton[1].text() == "H1执白")
          ControlButton[1].setText("AI执黑");
      else if(ControlButton[1].text() == "H1执黑")
          ControlButton[1].setText("AI执白");

      bool Win2 = GamePtr->PutAIChess();
      update();

      if(Win2){
          ++PVEGameNum;
          ++AISucNum;
         SetPVEGame(GamePtr->crtH1Side);
      }else{
          GamePtr->ChangePlayer();

          if(ControlButton[1].text() == "AI执白")
              ControlButton[1].setText("H1执黑");
          else if(ControlButton[1].text() == "AI执黑")
              ControlButton[1].setText("H1执白");

      }
    }
}

void MainWindow::PVPRound()
{
    bool Win = GamePtr->PutHumanChess(NearestPoint);
    update();

    if(Win){
        ++PVPGameNum;
        if(GamePtr->crtPlayerSide == H1) ++H1SucNum;
        else ++H2SucNum;
        SetPVPGame(GamePtr->crtH1Side);//Set里会update
    }
    else{
        GamePtr->ChangePlayer();
        if(ControlButton[1].text() == "H1执白")
            ControlButton[1].setText("H2执黑");
        else if(ControlButton[1].text() == "H1执黑")
            ControlButton[1].setText("H2执白");
        else if(ControlButton[1].text() == "H2执白")
            ControlButton[1].setText("H1执黑");
        else if(ControlButton[1].text() == "H2执黑")
            ControlButton[1].setText("H1执白");
    }

}



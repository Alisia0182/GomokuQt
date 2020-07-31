#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Controller.h"
#include "UIButton.h"

#include <QMainWindow>
#include <utility>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    int PVPGameNum;
    int PVEGameNum;
    int AISucNum;
    int H1SucNum;
    int H2SucNum;

    Ui::MainWindow *ui;
    Controller * GamePtr;

    UIButton ControlButton[5];
    UIButton TitleButton[2];

    std::pair<int,int> NearestPoint;

    void initButton();
    void initConnections();

    void SetPVEGame(bool h1Side);
    void SetPVPGame(bool h1Side);

    void PVERound();
    void PVPRound();

protected:
    void mouseMoveEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);



public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void paintEvent(QPaintEvent * event);
};

#endif // MAINWINDOW_H

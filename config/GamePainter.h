#ifndef GAMEPAINTER_H
#define GAMEPAINTER_H

#include "GamePainterMacro.h"

#include <QBrush>
#include <QColor>
#include <QPen>
#include <QPainter>
#include <utility>

class GamePainter : public QPainter
{
public:
    GamePainter(QPaintDevice * painter = Q_NULLPTR);
    void DrawGrid();
    void DrawMouseMark(std::pair<int,int>);
    void DrawChess(int _Board[][15]);
    void DrawText();
};

#endif // GAMEPAINTER_H

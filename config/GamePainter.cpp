#include "GamePainter.h"
#include "mainwindow.h"
#include "Debug.h"
#include <QDebug>
GamePainter::GamePainter(QPaintDevice * parent):
    QPainter(parent){
    setRenderHint(QPainter::Antialiasing,true);
}

void GamePainter::DrawGrid()
{
    QFont font;
    font.setFamily("Microsoft Yahei");
    font.setPointSize(10);
    font.setCapitalization(QFont::SmallCaps);
    setFont(font);

    for(int i = 0; i < 15; ++ i)
    {
        drawText(GradSize * WestSideinGrad - 80,
                 GradSize * (NorthSideinGrad + i) + 10,
                 QString::number(i + 1));

        drawLine(GradSize * WestSideinGrad,
                 GradSize * (NorthSideinGrad + i),
                 GradSize * (WestSideinGrad + 14),
                 GradSize * (NorthSideinGrad + i) );

        drawText(GradSize * (WestSideinGrad + i) ,
                 GradSize * NorthSideinGrad - 50,
                 Alphabet[i]                         );

        drawLine(GradSize * (WestSideinGrad + i),
                 GradSize * NorthSideinGrad,
                 GradSize * (WestSideinGrad + i),
                 GradSize * (NorthSideinGrad + 14));
    }

    QBrush Brush;
    Brush.setStyle(Qt::SolidPattern);
    setBrush(Brush);
    for(int i = 3;i <= 11; i += 4)
        for(int j = 3;j <= 11; j += 4){
            if(i == j ||i + j == 14 ){
                drawEllipse(GradSize * (WestSideinGrad + i) - MarkRadius,
                            GradSize * (NorthSideinGrad +j) - MarkRadius,
                            MarkRadius * 2, MarkRadius * 2);
            }
        }
}

void GamePainter::DrawMouseMark(std::pair<int,int> _point)
{
    QPen prevPen = this->pen();
    QRadialGradient radi;
    radi.setColorAt(0, qRgb(238, 238, 238));

    QPen pen;
    pen.setColor(qRgb(13, 144, 233));
    pen.setWidth(2);
    pen.setStyle(Qt::DotLine);

    setPen(pen);
    setBrush(radi);

    drawEllipse(GradSize * (WestSideinGrad  + _point.second) - ChessRadius,
                GradSize * (NorthSideinGrad +_point.first) - ChessRadius,
                ChessRadius * 2, ChessRadius * 2);
    setPen(prevPen);
}

void GamePainter::DrawChess(int _Board[][15])
{
    for (int i = 0; i < 15; ++i)
        for (int j = 0; j < 15; ++j)
        {
            if (!_Board[i][j])
            {
                setPen(Qt::NoPen);
                QRadialGradient radi;
                radi.setColorAt(0,qRgb(58, 58, 58));
                this->setBrush(radi);

                drawEllipse(GradSize * (WestSideinGrad  + j) - ChessRadius,
                            GradSize * (NorthSideinGrad + i) - ChessRadius,
                            ChessRadius * 2, ChessRadius * 2);
            }
            else if (_Board[i][j] == 1)
            {
                QRadialGradient radi;
                radi.setColorAt(0,qRgb(58, 58, 58));
                this->setBrush(radi);
                this->drawEllipse(GradSize * (WestSideinGrad  + j) - ChessRadius,
                                  GradSize * (NorthSideinGrad + i) - ChessRadius,
                                  ChessRadius * 2, ChessRadius * 2);

                radi.setColorAt(0,qRgb(238, 238, 238));
                this->setBrush(radi);
                this->drawEllipse(GradSize * (WestSideinGrad  + j) - ChessRadius + 2,
                                  GradSize * (NorthSideinGrad + i) - ChessRadius + 2,
                                  ChessRadius * 2 - 4, ChessRadius * 2 - 4);
            }
        }
}

void GamePainter::DrawText()
{
    QFont font;
    font.setFamily("Microsoft Yahei");
    font.setPointSize(10);
    font.setCapitalization(QFont::SmallCaps);
    setFont(font);

    drawText(GradSize * (WestSideinGrad + 14 + 3),
             GradSize * (NorthSideinGrad ),
             "AI");

    drawText(GradSize * (WestSideinGrad + 14 + 3),
             GradSize * (NorthSideinGrad + 5 ),
             "H1");

    drawText(GradSize * (WestSideinGrad + 14 + 3),
             GradSize * (NorthSideinGrad + 10 ),
             "H2");
}

#ifndef UIBUTTONS_H
#define UIBUTTONS_H

#include <QCloseEvent>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QString>
enum ControlButtonName
{
    GameModeButton,
    H1ColorButton,
    RestartButton,
    RetractButton,
    SwapButton
};

const QString ControlButtonStyle =
        // 操作按钮风格（扁平，圆角，黑白）
        "QPushButton"
        "{"
            "background-color:rgba(244,244,244,200);"	// 背景色（也可以设置图片）
            "border-style:outset;"				// 边框样式（inset/outset）
            "border-width:2px;"				// 边框宽度像素
            "border-radius:18px;"				// 边框圆角半径像素
            "border-color:rgba(68,68,68,220);"		// 边框颜色
            "font:bold 26px;"				// 字体，字体大小
            "color:rgba(58,58,58,230);"			// 字体颜色
            "padding:4px;"					// 填衬
        "}"
        //鼠标按下样式
        "QPushButton:pressed"
        "{"
            "background-color:rgba(42,42,42,255);"
            "border-width:4px;"				// 边框宽度像素
            "border-color:rgba(234,234,234,255);"
            "border-style:inset;"
            "color:rgba(200,200,200,255);"
        "}"
        //鼠标悬停样式
        "QPushButton:hover"
        "{"
            "background-color:rgba(48,48,48,230);"
            "border-color:rgba(250,250,250,250);"
            "color:rgba(250,250,250,250);"
        "}";

const QString BubbleButtonStyle =
        // 气泡按钮风格（方块，扁平，黑白）
        "QPushButton"
        "{"
            "background-color:rgba(250,250,250,230);"	// 背景色（也可以设置图片）
            "border-style:outset;"				// 边框样式（inset/outset）
            "border-width:2px;"				// 边框宽度像素
            "border-radius:15px;"				// 边框圆角半径像素
            "border-color:rgba(68,68,68,220);"		// 边框颜色
            "padding:6px;"					// 填衬
        "}";

const QString TitleButtonStyle[] = {
        // 标题栏按钮风格（扁平，圆角，黑白）
        "QPushButton"
        "{"
            "background-color:rgba(68,68,68,0);"		// 背景色（也可以设置图片）
            "border-style:outset;"				// 边框样式（inset/outset）
            "border-width:2px;"				// 边框宽度像素
            "border-radius:15px;"				// 边框圆角半径像素
            "border-color:rgba(68,68,68,0);"		// 边框颜色
            "font:bold 20px;"				// 字体，字体大小
            "color:rgba(68,68,68,240);"			// 字体颜色
        "}"
        //鼠标按下样式
        "QPushButton:pressed"
        "{"
            "font:bold 9px;"
        "}",

        "QPushButton"
        "{"
            "background-color:rgba(68,68,68,0);"
            "border-style:outset;"
            "border-width:2px;"
            "border-radius:15px;"
            "border-color:rgba(68,68,68,0);"
            "font:bold 23px;"
            "color:rgba(68,68,68,240);"
        "}"
        //鼠标按下样式
        "QPushButton:pressed"
        "{"
            "font:bold 19px;"
        "}"
        //鼠标悬停样式（无）
        };

enum ControlButtonSizeParam
{
    ControlButtonWidth  = 60 * 3,
    ControlButtonHeight = 60,
    ControlButtonSpacing= 60 * 4,
    ControlButtonNorth  = 60 * 19,
    ControlButtonWest   = 60 * 2
};

enum TitleButtonSizeParam
{
    TitleButtonWidth  = 60,
    TitleButtonHeight = 60,
    TitleButtonSpacing= 60,
    TitleButtonNorth  = 15,
    TitleButtonWest   = 60 * 21
};

class UIButton : public QPushButton
{
    public:
    UIButton(QWidget * parent = Q_NULLPTR);
    virtual void closeEvent(QEvent * event);
};

#endif // UIBUTTONS_H

#ifndef FIGURE_H
#define FIGURE_H

#include <../helper_class/helper_class.h>
#include <QColor>
#include <QtPlugin>

class Figure
{
public:

    Figure(int **figure, int type, Qt::GlobalColor color);
    Figure(const Figure &figure);
    virtual ~Figure();

    int **getFigure();
    void setFigure(int **figure);

    int getN();
    int getM();

    void setN(int N);
    void setM(int M);

    int getX();
    int getY();

    void setX(int x);
    void setY(int y);

    int getOffsetX();
    int getOffsetY();

    void setOffsetX(int offsetX);
    void setOffsetY(int offsetY);

    void setType(int type);
    int getType();

    void setColor(Qt::GlobalColor color);
    Qt::GlobalColor getColor();

    bool isBlocked();
    void unblock();
    void block();

    virtual void reInitialize();
    virtual void initializeFigure() = 0;
protected:

    Figure();

    int mN;
    int mM;

    int mX;
    int mY;

    int mOffsetX;
    int mOffsetY;

    int **mFigure;
    int mType;

    bool mIsBlocked;
    Qt::GlobalColor mColor;
};

Q_DECLARE_INTERFACE(Figure, "Tetris.Figure");

#endif // FIGURE_H

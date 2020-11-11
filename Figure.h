#ifndef FIGURE_H
#define FIGURE_H

#include <helper_class/include/helper_class.h>

class Figure
{
public:

    Figure(int **figure, int type);
    Figure(const Figure &figure);
    ~Figure();

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

    bool isBlocked();
    void unblock();
    void block();
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
};

#endif // FIGURE_H

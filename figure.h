#ifndef FIGURE_H
#define FIGURE_H

#include <map.h>

class Figure
{
public:

    Figure(int **figure, int type);
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
};

#endif // FIGURE_H
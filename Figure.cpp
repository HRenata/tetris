#include "Figure.h"
#include <iostream>

Figure::Figure(int **figure, int type, Qt::GlobalColor color)
{
    this->mN = 3;
    this->mM = 3;

    //change
    this->mX = 1;
    this->mY = Map::mN / 2;

    this->mOffsetX = Map::mOffsetX;
    this->mOffsetY = Map::mOffsetY;

    this->mFigure = new int*[this->mN];
    this->setFigure(figure);
    this->mColor = color;
    this->mType = type;

    this->mIsBlocked = false;
}

Figure::Figure(const Figure &figure)
{
    this->mN = figure.mN;
    this->mM = figure.mM;

    this->mX = figure.mX;
    this->mY = figure.mY;

    this->mOffsetX = figure.mOffsetX;
    this->mOffsetY = figure.mOffsetY;

    this->mFigure = new int*[this->mN];
    for(int i = 0; i < this->mN; ++i)
    {
        this->mFigure[i] = new int[this->mM];
        for(int j = 0; j < this->mM; ++j)
        {
            this->mFigure[i][j] = figure.mFigure[i][j];
        }
    }

    this->mType = figure.mType;
    this->mColor = figure.mColor;
    this->mIsBlocked = figure.mIsBlocked;
}

Figure::Figure()
{
    this->mN = 3;
    this->mM = 3;

    //change
    this->mX = 1;
    this->mY = Map::mN / 2;

    this->mOffsetX = Map::mOffsetX + (this->mY - 1) * Map::mCellWidth;
    this->mOffsetY = Map::mOffsetY;

    this->mFigure = new int*[this->mN];
    for(int i = 0; i < this->mN; ++i)
    {
        this->mFigure[i] = new int[this->mM];
        for(int j = 0; j < this->mM; ++j)
        {
            this->mFigure[i][j] = 0;
        }
    }
    this->mIsBlocked = false;
}

void Figure::reInitialize()
{
    this->mN = 3;
    this->mM = 3;

    //change
    this->mX = 1;
    this->mY = Map::mN / 2;

    this->mOffsetX = Map::mOffsetX + (this->mY - 1) * Map::mCellWidth;
    this->mOffsetY = Map::mOffsetY;

    this->mFigure = new int*[this->mN];
    for(int i = 0; i < this->mN; ++i)
    {
        this->mFigure[i] = new int[this->mM];
        for(int j = 0; j < this->mM; ++j)
        {
            this->mFigure[i][j] = 0;
        }
    }
    this->mIsBlocked = false;
}

int Figure::getN()
{
    return this->mN;
}

int Figure::getM()
{
    return this->mM;
}

void Figure::setN(int N)
{
    this->mN = N;
}

void Figure::setM(int M)
{
    this->mM = M;
}

int Figure::getX()
{
    return this->mX;
}

int Figure::getY()
{
    return this->mY;
}

void Figure::setX(int x)
{
    this->mX = x;
}

void Figure::setY(int y)
{
    this->mY = y;
}

int Figure::getOffsetX()
{
    return this->mOffsetX;
}

int Figure::getOffsetY()
{
    return this->mOffsetY;
}

void Figure::setOffsetX(int offsetX)
{
    this->mOffsetX = offsetX;
}

void Figure::setOffsetY(int offsetY)
{
    this->mOffsetY = offsetY;
}

int **Figure::getFigure()
{
    return this->mFigure;
}

void Figure::setType(int type)
{
    this->mType = type;
}

int Figure::getType()
{
    return this->mType;
}

void Figure::setColor(Qt::GlobalColor color)
{
    this->mColor = color;
}

Qt::GlobalColor Figure::getColor()
{
    return this->mColor;
}

bool Figure::isBlocked()
{
    return mIsBlocked;
}

void Figure::unblock()
{
    this->mIsBlocked = false;
}

void Figure::block()
{
    this->mIsBlocked = true;
}

void Figure::setFigure(int **figure)
{
    for(int i = 0; i < this->mN; ++i)
    {
        this->mFigure[i] = new int[this->mM];
        for(int j = 0; j < this->mM; ++j)
        {
            this->mFigure[i][j] = figure[i][j];
        }
    }
}

Figure::~Figure()
{
    for(int i = 0; i < this->mN; ++i)
    {
        delete[] this->mFigure[i];
    }
    delete[] this->mFigure;
}

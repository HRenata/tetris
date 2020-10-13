#include "figure.h"
#include <iostream>

Figure::Figure(int **figure, int type)
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
    this->mType = type;
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

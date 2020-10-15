#include "FigureZ.h"

FigureZ::FigureZ() : Figure()
{
    this->mType = 2;
    this->mFigure[0][0] = this->mType;
    this->mFigure[0][1] = this->mType;
    this->mFigure[1][1] = this->mType;
    this->mFigure[1][2] = this->mType;
}

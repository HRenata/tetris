#include "FigureO.h"
#include <iostream>
FigureO::FigureO() : Figure()
{
    this->mType = 1;
    this->mFigure[1][0] = this->mType;
    this->mFigure[1][1] = this->mType;
    this->mFigure[2][0] = this->mType;
    this->mFigure[2][1] = this->mType;
}

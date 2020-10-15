#include "FigureL.h"

FigureL::FigureL() : Figure()
{
    this->mType = 3;
    this->mFigure[0][1] = this->mType;
    this->mFigure[1][1] = this->mType;
    this->mFigure[2][1] = this->mType;
    this->mFigure[2][2] = this->mType;
}

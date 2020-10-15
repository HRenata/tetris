#include "Controller.h"

Controller::Controller()
{

}

Controller::~Controller()
{

}

bool Controller::hasCollisions(Figure *figure)
{
    for(int i = 0; i < figure->getN(); ++i)
    {
        for(int j = 0; j < figure->getM(); ++j)
        {
            if(figure->getFigure()[i][j] != 0
               && Map::mMap[figure->getY() + j - 1][figure->getX() + i - 1] != 0)
            {
                return true;
            }
        }
    }
    return false;
}

bool Controller::hasCollisionsWhenMovingLeft(Figure *figure)
{
    if(figure->getY() < 0 - 1)
    {
        return true;
    }
    else if(figure->getY() == 0 - 1)
    {
        for(int i = 0; i < figure->getN(); ++i)
        {
            if(figure->getFigure()[i][1] != 0)
            {
                return true;
            }
        }
        return false;
    }
    else if(figure->getY() == 0)
    {
        for(int i = 0; i < figure->getN(); ++i)
        {
            if(figure->getFigure()[i][0] != 0)
            {
                return true;
            }
        }
        return false;
    }
    else
    {
        return this->hasCollisions(figure);
    }
}

bool Controller::hasCollisionsWhenMovingRight(Figure *figure)
{
    if(figure->getY() > Map::mN)
    {
        return true;
    }
    else if(figure->getY() == Map::mN)
    {
        for(int i = 0; i < figure->getN(); ++i)
        {
            if(figure->getFigure()[i][1] != 0)
            {
                return true;
            }
        }
        return false;
    }
    else if(figure->getY() == Map::mN - 1)
    {
        for(int i = 0; i < figure->getN(); ++i)
        {
            if(figure->getFigure()[i][2] != 0)
            {
                return true;
            }
        }
        return false;
    }
    else
    {
        return this->hasCollisions(figure);
    }
}

bool Controller::hasCollisionsWhenMovingUp(Figure *figure)
{
    if(figure->getX() < 0 - 1)
    {
        return true;
    }
    else if(figure->getX() == 0 - 1)
    {
        for(int j = 0; j < figure->getM(); ++j)
        {
            if(figure->getFigure()[1][j] != 0)
            {
                return true;
            }
        }
        return false;
    }
    else if(figure->getX() == 0)
    {
        for(int j = 0; j < figure->getM(); ++j)
        {
            if(figure->getFigure()[0][j] != 0)
            {
                return true;
            }
        }
        return false;
    }
    else
    {
        return this->hasCollisions(figure);
    }
}

bool Controller::hasCollisionsWhenMovingDown(Figure *figure)
{
    if(figure->getX() > Map::mM)
    {
        return true;
    }
    else if(figure->getX() == Map::mM)
    {
        for(int j = 0; j < figure->getM(); ++j)
        {
            if(figure->getFigure()[1][j] != 0)
            {
                return true;
            }
        }
        return false;
    }
    else if(figure->getX() == Map::mM - 1)
    {
        for(int j = 0; j < figure->getM(); ++j)
        {
            if(figure->getFigure()[2][j] != 0)
            {
                return true;
            }
        }
        return false;
    }
    else
    {
        return this->hasCollisions(figure);
    }
}

bool Controller::hasCollisionsWhenRotation(Figure *figure)
{
    if(this->hasCollisionsWhenMovingUp(figure) ||
       this->hasCollisionsWhenMovingDown(figure) ||
       this->hasCollisionsWhenMovingLeft(figure) ||
       this->hasCollisionsWhenMovingRight(figure))
    {
        return true;
    }
    return false;
}

bool Controller::movementLeft(Figure *figure)
{
    figure->setY(figure->getY() - 1);
    if(this->hasCollisionsWhenMovingLeft(figure))
    {
        figure->setY(figure->getY() + 1);
    }
    else
    {
        figure->setOffsetX(figure->getOffsetX() - Map::mCellWidth);
    }

    return this->isPossibleMovementDown(figure);
}

bool Controller::movementRight(Figure *figure)
{
    figure->setY(figure->getY() + 1);
    if(this->hasCollisionsWhenMovingRight(figure))
    {
        figure->setY(figure->getY() - 1);
    }
    else
    {
        figure->setOffsetX(figure->getOffsetX() + Map::mCellHeight);
    }

    return this->isPossibleMovementDown(figure);
}

bool Controller::movementDown(Figure *figure)
{
    figure->setX(figure->getX() + 1);
    if(this->hasCollisionsWhenMovingDown(figure))
    {
        figure->block();
        figure->setX(figure->getX() - 1);
    }
    else
    {
        figure->setOffsetY(figure->getOffsetY() + Map::mCellHeight);
    }
    return !figure->isBlocked();
}

bool Controller::rotation(Figure *figure)
{
    this->rotateClockWise(figure);
    if(this->hasCollisionsWhenRotation(figure))
    {
        this->rotateCounterClockWise(figure);
    }

    return this->isPossibleMovementDown(figure);
}

void Controller::rotateClockWise(Figure *figure)
{
    int **tempFigure = new int*[figure->getN()];
    for(int i = 0; i < figure->getN(); ++i)
    {
        tempFigure[i] = new int[figure->getM()];
        for(int j = 0; j < figure->getM(); ++j)
        {
            tempFigure[i][j] = figure->getFigure()[i][j];
        }
    }

    for(int i = 0; i < figure->getN(); ++i)
    {
        for(int j = 0, k = figure->getM() - 1; j < figure->getM(); ++j, --k)
        {
            figure->getFigure()[i][j] = tempFigure[k][i];
        }
    }

    for(int i = 0; i < figure->getN(); ++i)
    {
        delete[] tempFigure[i];
    }
    delete[] tempFigure;
}

void Controller::rotateCounterClockWise(Figure *figure)
{
    int **tempFigure = new int*[figure->getN()];
    for(int i = 0; i < figure->getN(); ++i)
    {
        tempFigure[i] = new int[figure->getM()];
        for(int j = 0; j < figure->getM(); ++j)
        {
            tempFigure[i][j] = figure->getFigure()[i][j];
        }
    }

    for(int i = 0, k = figure->getN() - 1; i < figure->getN(); ++i, --k)
    {
        for(int j = 0; j < figure->getM(); ++j)
        {
            figure->getFigure()[i][j] = tempFigure[j][k];
        }
    }

    for(int i = 0; i < figure->getN(); ++i)
    {
        delete[] tempFigure[i];
    }
    delete[] tempFigure;
}

void Controller::lockFigure(Figure *figure)
{
    for(int i = 0; i < figure->getN(); ++i)
    {
        for(int j = 0; j < figure->getM(); ++j)
        {
            if(figure->getFigure()[i][j] != 0)
            {
                Map::mMap[figure->getY() + j - 1][figure->getX() + i - 1] = figure->getFigure()[i][j];
            }
        }
    }
    figure = new FigureZ();
}

bool Controller::isPossibleMovementDown(Figure *figure)
{
    bool isPossibleMovementDown = true;
    figure->setX(figure->getX() + 1);
    if(this->hasCollisionsWhenMovingDown(figure))
    {
        isPossibleMovementDown = false;
    }
    else
    {
        figure->unblock();
    }
    figure->setX(figure->getX() - 1);
    return isPossibleMovementDown;
}

void Controller::startGame()
{
    Game::mGameIsActive = true;
    Game::mScore = 0;
    for(int i = 0; i < Map::mN; ++i)
    {
        for(int j = 0; j < Map::mM; ++j)
        {
            Map::mMap[i][j] = 0;
        }
    }
}

void Controller::resetGame()
{
    this->startGame();
}

void Controller::endGame()
{
    Game::mGameIsActive = false;
}

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <icallbacklistener.h>
#include <map.h>

#include <figureo.h>
#include <figurel.h>
#include <figurez.h>

class Controller : public ICallbackListener
{
public:
    Controller();
    ~Controller();

    virtual bool movementLeft(Figure *figure);
    virtual bool movementRight(Figure *figure);
    virtual bool movementDown(Figure *figure);
    virtual bool rotation(Figure *figure);
    virtual bool hasCollisions(Figure *figure);
    virtual void lockFigure(Figure *figure);

    bool hasCollisionsWhenMovingLeft(Figure *figure);
    bool hasCollisionsWhenMovingRight(Figure *figure);
    bool hasCollisionsWhenMovingUp(Figure *figure);
    bool hasCollisionsWhenMovingDown(Figure *figure);
    bool hasCollisionsWhenRotation(Figure *figure);
    bool isPossibleMovementDown(Figure *figure);

    void rotateClockWise(Figure *figure);
    void rotateCounterClockWise(Figure *figure);
};

#endif // CONTROLLER_H

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <icallbacklistener.h>
#include <map.h>

class Controller : public ICallbackListener
{
public:
    Controller();
    ~Controller();

    virtual void movementLeft(Figure *figure);
    virtual void movementRight(Figure *figure);
    virtual void movementDown(Figure *figure);
    virtual void rotation(Figure *figure);

    bool hasCollisions(Figure *figure);
    bool hasCollisionsWhenMovingLeft(Figure *figure);
    bool hasCollisionsWhenMovingRight(Figure *figure);
    bool hasCollisionsWhenMovingUp(Figure *figure);
    bool hasCollisionsWhenMovingDown(Figure *figure);
    bool hasCollisionsWhenRotation(Figure *figure);

    void rotateClockWise(Figure *figure);
    void rotateCounterClockWise(Figure *figure);
};

#endif // CONTROLLER_H

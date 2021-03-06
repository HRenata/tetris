#ifndef ICALLBACKLISTENER_H
#define ICALLBACKLISTENER_H

#include <Figure.h>
class ICallbackFigureWatcher
{
public:
    ICallbackFigureWatcher() = default;
    virtual ~ICallbackFigureWatcher() = default;

    virtual bool movementLeft(Figure *figure) = 0;
    virtual bool movementRight(Figure *figure) = 0;
    virtual bool movementDown(Figure *figure) = 0;
    virtual bool rotation(Figure *figure) = 0;
    virtual bool hasCollisions(Figure *figure) = 0;
    virtual void deleteFilledRows() = 0;
    virtual void lockFigure(Figure *figure) = 0;
};

#endif // ICALLBACKLISTENER_H

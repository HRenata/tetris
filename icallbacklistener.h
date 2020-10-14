#ifndef ICALLBACKLISTENER_H
#define ICALLBACKLISTENER_H

#include <figure.h>
class ICallbackListener
{
public:
    ICallbackListener() = default;
    virtual ~ICallbackListener() = default;

    virtual bool movementLeft(Figure *figure) = 0;
    virtual bool movementRight(Figure *figure) = 0;
    virtual bool movementDown(Figure *figure) = 0;
    virtual bool rotation(Figure *figure) = 0;
    virtual bool hasCollisions(Figure *figure) = 0;
    virtual void lockFigure(Figure *figure) = 0;
};

#endif // ICALLBACKLISTENER_H

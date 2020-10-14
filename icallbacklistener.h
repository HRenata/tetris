#ifndef ICALLBACKLISTENER_H
#define ICALLBACKLISTENER_H

#include <figure.h>
class ICallbackListener
{
public:
    ICallbackListener() = default;
    virtual ~ICallbackListener() = default;

    virtual void movementLeft(Figure *figure) = 0;
    virtual void movementRight(Figure *figure) = 0;
    virtual void movementDown(Figure *figure) = 0;
    virtual void rotation(Figure *figure) = 0;
};

#endif // ICALLBACKLISTENER_H

#ifndef ICALLBACKGAMESTATEWATCHER_H
#define ICALLBACKGAMESTATEWATCHER_H

#include <Figure.h>
class ICallbackGameStateWatcher
{
public:
    ICallbackGameStateWatcher() = default;
    virtual ~ICallbackGameStateWatcher() = default;

    virtual void startGame() = 0;
    virtual void resetGame() = 0;
    virtual void endGame() = 0;
};

#endif // ICALLBACKGAMESTATEWATCHER_H

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <ICallbackFigureWatcher.h>
#include <ICallbackGameStateWatcher.h>
#include <../helper_class/helper_class.h>
#include <Game.h>

class Controller :
        public ICallbackFigureWatcher,
        public ICallbackGameStateWatcher
{
public:
    Controller();
    ~Controller();

    bool movementLeft(Figure *figure) override;
    bool movementRight(Figure *figure) override;
    bool movementDown(Figure *figure) override;
    bool rotation(Figure *figure) override;
    bool hasCollisions(Figure *figure) override;
    void deleteFilledRows() override;
    void lockFigure(Figure *figure) override;

    bool hasCollisionsWhenMovingLeft(Figure *figure);
    bool hasCollisionsWhenMovingRight(Figure *figure);
    bool hasCollisionsWhenMovingUp(Figure *figure);
    bool hasCollisionsWhenMovingDown(Figure *figure);
    bool hasCollisionsWhenRotation(Figure *figure);
    bool isPossibleMovementDown(Figure *figure);

    void rotateClockWise(Figure *figure);
    void rotateCounterClockWise(Figure *figure);


    void startGame() override;
    void resetGame() override;
    void endGame() override;
    void pauseGame() override;
    void continueGame() override;
};

#endif // CONTROLLER_H

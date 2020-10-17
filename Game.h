#ifndef GAME_H
#define GAME_H


class Game
{
public:
    static bool mGameIsActive;
    static bool mGameIsPaused;
    static int mScore;
    static int mNumberFigures;

    Game (Game &) = delete;
    void operator= (const Game &) = delete;
    Game (Game &&) = delete;
    Game() = delete;
};

#endif // GAME_H

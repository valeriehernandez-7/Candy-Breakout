#ifndef SERVER_GAME_H
#define SERVER_GAME_H

#include"game/state.h"

#include<SFML/Graphics.hpp>
#include<array>

using namespace std;
using namespace sf;

class Game {
public:
    Game();

    ~Game();

    RenderWindow &getWindow();

    Font &getFont();

    void run();

    void setState(State::states state);

    void resetGame();

    void newGame();

    void loadGame();

    int getScore() const;

    int getLevel() const;

    friend class PlayState;

    friend class OverState;

private:
    RenderWindow window;
    Font font;
    array<State*, 4> states;
    State* currentState;
    bool gameClosed;
    bool gamePaused;
    bool gameWon;
    int totalScore;
    int levelNumber;
};

#endif //SERVER_GAME_H
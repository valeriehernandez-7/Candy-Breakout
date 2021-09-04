#ifndef SERVER_STATE_H
#define SERVER_STATE_H

#include "game/objects.cpp"

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

using namespace std;
using namespace sf;

class Game;

////////// State //////////

class State {
public:
    State(Game *game);

    virtual ~State();

    Game &getGameState();

    enum states {
        initial,
        play,
        load,
        final
    };

    RenderWindow &window;
    Sprite backgroundImage;

    Texture textures[12];
    enum {
        tBall, tBallPlus, 
        tPlayer, tPlayerPlus, tPlayerJunior, tPlayerHealth,
        tCandySingle, tCandyDouble, tCandyTriple, tCandyDeep, tBonus,
        tGameScreen
    };

    virtual void update(Clock &clock);

    virtual void display(RenderWindow &window);

    virtual void handler(Event &event);

private:
    Game *gameState;
};


////////// InitialState //////////

class InitialState : public State {
public:
    InitialState(Game *game);

    ~InitialState();

    void display(RenderWindow &window) override;

private:
    Text message;
    Text txtKey;
};


////////// PlayState //////////

class PlayState : public State {
public:
    PlayState(Game *game);

    ~PlayState();

    void update(Clock &clock) override;

    void display(RenderWindow &window) override;

    void handler(Event &e) override;

    friend struct Bonus;

private:
    ////////// UI //////////
    int timer;
    Text bonusInfo;
    Text txtPlayerScore;
    Sprite playerHealthMeter;

    bool onCollision(Sprite &spriteA, Sprite &spriteB);

    ////////// Level //////////
    Image levelImage;

    void levelGenerator();

    ////////// Ball //////////
    vector<Ball> balls;
    bool ballStatus;

    void ballGenerator();

    void ballRelease();

    ////////// Player //////////
    int playerHealth;
    int playerScore;
    int playerMotion;
    int playerSpeed;
    Sprite player;

    ////////// Candy //////////
    vector<Candy> candies;
    Bonus bonus;

    void bonusGenerator();

    void candyManager(vector<Candy> &candies, unsigned &id);
};


////////// LoadState ////////

class LoadState : public State {
public:
    LoadState(Game *game);

    ~LoadState();

    void update(Clock &clock) override;

    void display(RenderWindow &window) override;

private:
    int timer;
    Text txtLevel;
    Text txtTimer;
    Text txtKey;
};


////////// OverState //////////

class OverState : public State {
public:
    OverState(Game *game);

    ~OverState();

    void update(Clock &clock) override;

    void display(RenderWindow &window) override;

private:
    Text message;
    Text txtKey;
    Text txtFinalScore;
};

#endif //SERVER_STATE_H
#ifndef SERVER_STATE_H
#define SERVER_STATE_H

/*!
 * \include communication/socketserver.h
 * \include game/objects.cpp
 * \include SFML/Graphics.hpp
 * \include cstdlib
 * \include string
 * \include vector
 * \include iostream
 */
#include "communication/socketserver.h"
#include "game/objects.cpp"
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>

/*!
 * \namespace std General-purpose polymorphic function wrapper.
 * \namespace sf Simple and Fast Multimedia Library provides a simple interface to the various components of your PC.
 */
using namespace std;
using namespace sf;

class Game;

// --- State ---

/*! \class State
 *  \brief Baseline dataset for game states (screens).
 */
class State {
public:
    /*! \brief State constructor.
     *  \param game pointer to Game Class.
     */
    State(Game *game);

    virtual ~State(); /*!< State destructor. */

    /*! \fn getGameState()
     *  \brief Gets the current game state.
     *  \return gameState as a pointer to the Game State
     */
    Game &getGameState();

    /*! \enum states
     *  \brief Game screens (states).
     */
    enum states {
        initial, /*!< Refers to the InitialState, this is the welcome screen where the connection with the player is expected to access the loading screen. */
        play, /*!< Refers to the PlayState, this is the game screen where the player must lose all lives to access the game over screen or must win all 10 levels to access the victory screen. */
        load, /*!< Refers to the LoadState, this is the loading screen where the counter is expected to finish to access the game screen or to the next game level screen. */
        final /*!< Refers to the OverState, this is the last screen where the game score and result are shown to the player, the player decides whether to finish the game or play again. */
    };

    RenderWindow &window; /*!< Reference to the application window where the game is being executed. */
    Sprite backgroundImage; /*!< Game background sprite (tGameScreen), changes according to the current game state. */

    Texture textures[12]; /*!< Contains all possible textures of the game objects. */

    /*! \enum textures
     *  \brief Game textures (sprites).
     */
    enum {
        tBall, /*!< Ball common state texture. \file i_ball.png */
        tBallPlus, /*!< Ball Bonus (super_ball) state texture. \file i_ball-plus.png */
        tPlayer, /*!< Player common state texture. \file i_player.png */
        tPlayerPlus, /*!< Player Bonus (player+Width) state texture. \file i_player-plus.png */
        tPlayerJunior, /*!< Player Bonus (player-Width) state texture. \file i_player-junior.png */
        tPlayerHealth, /*!< Player's health meter texture. \file i_life-3.png */
        tCandySingle, /*!< Candy (resistance = 1, points = 10) texture. \file i_candy-single.png */
        tCandyDouble, /*!< Candy (resistance = 2, points = 15) texture. \file i_candy-double.png */
        tCandyTriple, /*!< Candy (resistance = 3, points = 20) texture. \file i_candy-triple.png */
        tCandyDeep, /*!< Candy (resistance = 4, points = 25) texture. \file i_candy-deep.png */
        tBonus, /*!< Candy Bonus texture. \file i_candy-bonus.png */
        tGameScreen /*!< Game background texture. \file i_s-start.png */
    };

    /*! \fn virtual void update(Clock &clock)
     *  \brief Specifies the behavior of all objects in the current game screen.
     *  \param clock as time elapsed
     */
    virtual void update(Clock &clock);

    /*! \fn virtual void display(RenderWindow &window);
     *  \brief Updates and displays all objects on the current game screen.
     *  \param window as game window
     */
    virtual void display(RenderWindow &window);

    /*! \fn virtual void handler(Event &event, string &key)
     *  \brief Handles the events through the data (key) sent by the client on the current game screen.
     *  \param event as system event
     *  \param key as data received from the client
     */
    virtual void handler(Event &event, string &key);

private:
    Game *gameState; /*!< Pointer to the Game State. */
};


// --- InitialState ---

/*! \class InitialState
 *  \brief Welcome screen feature set.
 */
class InitialState : public State {
public:
    InitialState(Game *game);

    ~InitialState();

    void display(RenderWindow &window) override;

private:
    Text message; /*!< "WELCOME!" screen message. */
    Text txtKey; /*!< Displays the key codes message. */
};


// --- PlayState ---

/*! \class PlayState
 *  \brief Game screen feature set.
 */
class PlayState : public State {
public:
    PlayState(Game *game);

    ~PlayState();

    void update(Clock &clock) override;

    void display(RenderWindow &window) override;

    void handler(Event &event, string &key) override;

    friend struct Bonus;

private:
    // --- UI ---
    int timer; /*!< Displays the countdown to go to the next game screen. */
    Text bonusInfo; /*!< Displays the identifier of the earned Bonus. */
    Text txtPlayerScore; /*!< Displays the Player 's current score. */
    Sprite playerHealthMeter; /*!< Sprite of the Player 's health meter. */

    /*! \fn bool onCollision(Sprite &spriteA, Sprite &spriteB)
     *  \brief Checks if spriteA intersects spriteB.
     *  If there is an intersection between objects A-B it returns true, otherwise it returns false.
     *  \param spriteA sprite of object A
     *  \param spriteB sprite of object B
     *  \return boolean
     */
    bool onCollision(Sprite &spriteA, Sprite &spriteB);

    // --- Level ---
    Image levelImage; /*!< Level image figure reference. \file l-1.png */

    /*! \fn void levelGenerator()
     *  \brief Generates a pattern using Candy type game objects (using an image as a reference), these candies will be chosen randomly.
     */
    void levelGenerator();

    // --- Ball ---
    vector<Ball> balls; /*!< Ball type game objects vector. */
    bool ballStatus; /*!< Check if the ball has been released by the player or is in rest state. */

    /*! \fn void ballGenerator()
     *  \brief Places the ball in its common state, i.e. it will place the ball just above the player so that the ball can be released.
     */
    void ballGenerator();

    /*! \fn void ballRelease()
     *  \brief Releases the Ball in a random direction.
     */
    void ballRelease();

    // --- Player ---
    int playerHealth; /*!< An integer representing the Player 's amount of lives. */
    int playerScore; /*!< An integer representing the Player 's current score. */
    int playerMotion; /*!< An integer defining several states of player movements (slide_left = 1, slide_right = 2, rotate_right = 3, rotate_left = 4, default = 5 , no_motion = 0). */
    int playerSpeed; /*!< An integer representing the Player 's speed, the amount of pixels at which the player moves from left to right. */
    Sprite player; /*!< Sprite of the Player. */

    // --- Candy ---
    vector<Candy> candies; /*!< Candy type game objects vector. */
    Bonus bonus; /*!< Bonus instance. */

    /*! \fn void bonusGenerator()
     *  \brief Selects in a random way and quantity the Candy objects that will be Bonus type.
     */
    void bonusGenerator();

    /*! \fn void candyManager(vector<Candy> &candies, unsigned &id)
     *  \param candies as the reference to the vector containing the Candy objects.
     *  \param id as the reference to the identifier of the position (index) where the Candy is located in the vector of candies.
     */
    void candyManager(vector<Candy> &candies, unsigned &id);
};


// --- LoadState ---

/*! \class LoadState
 *  \brief Loading screen feature set.
 */
class LoadState : public State {
public:
    LoadState(Game *game);

    ~LoadState();

    void update(Clock &clock) override;

    void display(RenderWindow &window) override;

private:
    int timer; /*!< Counter number (default = 3). */
    Text txtLevel; /*!< Displays the game level number. */
    Text txtTimer; /*!< Displays the counter current number. */
    Text txtKey; /*!< Displays the key codes message. */
};

// --- OverState ---

/*! \class OverState
 *  \brief Last screen feature set.
 */
class OverState : public State {
public:
    OverState(Game *game);

    ~OverState();

    void update(Clock &clock) override;

    void display(RenderWindow &window) override;

private:
    Text message; /*!< "GAME OVER" || "VICTORY" screen message. */
    Text txtKey; /*!< Displays the key codes message. */
    Text txtFinalScore; /*!< Displays the Player's total score. */
};

#endif //SERVER_STATE_H
#ifndef SERVER_GAME_H
#define SERVER_GAME_H

/*!
 * \include communication/socketserver.h
 * \include game/state.cpp
 * \include SFML/Graphics.hpp
 * \include array
 */
#include "communication/socketserver.h"
#include "game/state.h"
#include <SFML/Graphics.hpp>
#include <array>

/*!
 * \namespace std General-purpose polymorphic function wrapper.
 * \namespace sf Simple and Fast Multimedia Library provides a simple interface to the various components of your PC.
 */
using namespace std;
using namespace sf;

/*! \class Game
 *  \brief Game feature set.
 */
class Game {
public:
    Game(); /*!< Game constructor. */

    ~Game(); /*!< Game destructor. */

    /*! \fn RenderWindow &getWindow()
     *  \brief References the game window.
     *  \return window
     */
    RenderWindow &getWindow();

    /*! \fn Font &getFont()
     *  \brief References the game font.
     *  \return font
     */
    Font &getFont();

    /*! \fn void run(string key)
     *  \brief Defines the events that will happen according to the current game screen and the client sent data.
     *  \param key as data received from the client
     */
    void run(string key);

    /*! \fn void setState(State::states state)
     *  \brief Sets the requested State as the current gameState.
     *  \param state as requested state
     */
    void setState(State::states state);

    /*! \fn void resetGame()
     *  \brief Removes the PlayState references.
     */
    void resetGame();

    /*! \fn void newGame()
     *  \brief Set the game level as 1 and the player total score as 0.
     */
    void newGame();

    /*! \fn void loadGame()
     *  \brief Creates the PlayState reference.
     */
    void loadGame();

    /*! \fn int getScore() const
     *  \brief Gets the Player 's total score.
     *  \return totalScore
     */
    int getScore() const;

    /*! \fn int getLevel() const
     *  \brief Gets the current Game level.
     *  \return levelNumber
     */
    int getLevel() const;

    friend class PlayState;

    friend class OverState;

private:
    RenderWindow window; /*!< Game window instance. */
    Font font; /*!< Game font instance. */
    array<State*, 4> states; /*!< Game states descriptor. */
    State* currentState; /*!< Current game state (screen). */
    bool gamePaused; /*!< Defines whether the game is paused or not. */
    bool gameWon; /*!< Defines if the player has won all the levels (Levels 1-10). */
    int totalScore; /*!< Player 's total score. */
    int levelNumber; /*!< Current game level. */
};

#endif //SERVER_GAME_H
/*!
 * \headerfile game.h "game/game.h"
 */
#include "game/game.h"

Game::Game() : window{VideoMode(550, 450), "CANDY BREAKOUT", Style::Titlebar | Style::Close},
               gamePaused{false}, gameWon{false}, totalScore{0}, levelNumber{1} {

    window.setFramerateLimit(60);
    font.loadFromFile("resources/font/CanvasBags.otf");

    states[State::initial] = new InitialState(this);
    states[State::play] = nullptr;
    states[State::load] = new LoadState(this);
    states[State::final] = new OverState(this);

    setState(State::initial);
}

Game::~Game() {
    for (State *state: states) {
        delete state;
    }
}

RenderWindow &Game::getWindow() {
    return window;
}

Font &Game::getFont() {
    return font;
}

void Game::run(string key) {
    Clock frames;
    Event event;
    while (window.pollEvent(event)) {
        if (event.type == Event::Closed) {
            window.close();
        }
        if (event.type == Event::KeyPressed) {
            if (key == "<Escape>") {
                if (currentState != states[State::initial]) {
                    newGame();
                    setState(State::initial);
                }
            }
            if (key == "<Space>") {
                if (currentState == states[State::initial]) {
                    setState(State::load);
                }
            }
            if (key == "<Enter>") {
                if (currentState == states[State::final]) {
                    newGame();
                    setState(State::initial);
                }
            }
            if (key == "<P>") {
                if (currentState == states[State::play]) {
                    gamePaused = !gamePaused;
                }
            }
        }
        currentState->handler(event, key);
    }
    if (!gamePaused) {
        currentState->update(frames);
    }
    window.clear();
    currentState->display(window);
    window.display();
}

void Game::setState(State::states state) {
    currentState = states[state];
}

void Game::resetGame() {
    delete states[State::play];
    states[State::play] = nullptr;
}

void Game::newGame() {
    levelNumber = 1;
    totalScore = 0;
}

void Game::loadGame() {
    states[State::play] = new PlayState(this);
}

int Game::getScore() const {
    return totalScore;
}

int Game::getLevel() const {
    return levelNumber;
}
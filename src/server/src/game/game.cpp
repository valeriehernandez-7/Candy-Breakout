#include "game/game.h"
#include "game/state.h"

using namespace std;
using namespace sf;

Game::Game() : window{VideoMode(550, 450), "CANDY BREAKOUT"}, gameClosed{false},
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

void Game::run() {
    Clock frames;
    Event event;
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Space) {
                    if (currentState == states[State::initial])
                        setState(State::load);
                }
                if (event.key.code == Keyboard::Enter) {
                    if (currentState == states[State::final]) {
                        newGame();
                        setState(State::initial);
                    }
                }
                if (event.key.code == Keyboard::Escape) {
                    if (currentState == states[State::final] || currentState == states[State::initial])
                        window.close();
                }
                if (event.key.code == Keyboard::P) {
                    if (currentState == states[State::play])
                        gamePaused = !gamePaused;
                }
            }
            currentState->handler(event);
        }
        if (!gamePaused) {
            currentState->update(frames);
        }
        window.clear();
        currentState->display(window);
        window.display();
    }
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
    gameClosed = false;
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
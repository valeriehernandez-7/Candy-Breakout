#include "game/game.h"
#include "game/state.h"

#include <iostream>
#include <cstdlib>

using namespace std;
using namespace sf;

/////// State ///////

State::State(Game *game) : window{game->getWindow()}, gameState{game} {}

State::~State() {}

Game &State::getGameState() {
    return *gameState;
}

void State::update(Clock &clock) {}

void State::display(RenderWindow &window) {}

void State::handler(Event &event) {}


/////// InitialState ///////

InitialState::InitialState(Game *game) : State{game} {
    textures[tGameScreen].loadFromFile("resources/screen/i_s-start.png");
    backgroundImage.setTexture(textures[tGameScreen]);

    message.setFont(getGameState().getFont());
    message.setCharacterSize(35);
    message.setString("WELCOME!");
    message.setPosition(380, 255);

    txtKey.setFont(getGameState().getFont());
    txtKey.setCharacterSize(17);
    txtKey.setString("[SPACE] - Play\n  [ESC] - Quit");
    txtKey.setPosition(395, 300);

}

InitialState::~InitialState() {}

void InitialState::display(RenderWindow &window) {
    window.draw(backgroundImage);
    window.draw(message);
    window.draw(txtKey);
}


/////// PlayState ///////

PlayState::PlayState(Game *game) : State(game), ballStatus{false}, playerScore{0}, playerMotion{3}, playerHealth{3},
                                   playerSpeed{8}, timer{0} {

}

PlayState::~PlayState() {}

void PlayState::update(Clock &clock) {

}

void PlayState::display(RenderWindow &window) {

}

void PlayState::handler(Event &e) {

}

bool PlayState::onCollision(Sprite &spriteA, Sprite &spriteB) {

}

void PlayState::levelGenerator() {

}

void PlayState::ballGenerator() {

}

void PlayState::ballRelease() {

}

void PlayState::bonusGenerator() {

}

void PlayState::candyManager(vector<Candy> &candies, unsigned &id) {
    
}

/////// LoadState ///////

LoadState::LoadState(Game *game) : State{game}, timer{3} {
    textures[tGameScreen].loadFromFile("resources/screen/i_s-game.png");
    backgroundImage.setTexture(textures[tGameScreen]);

    txtLevel.setFont(getGameState().getFont());
    txtLevel.setCharacterSize(30);
    txtLevel.setPosition(405, 245);

    txtTimer.setFont(getGameState().getFont());
    txtTimer.setCharacterSize(120);
    txtTimer.setOutlineThickness(3);
    txtTimer.setOutlineColor(Color(89,42,30));
    txtTimer.setPosition(140, 50);

    txtKey.setFont(getGameState().getFont());
    txtKey.setCharacterSize(15);
    txtKey.setString("         [P] - Pause\n    [UP] - Ball Release\n[LEFT] & [RIGHT] - Move\n         [ESC] - Quit");
    txtKey.setPosition(376, 285);
}

LoadState::~LoadState() {}

void LoadState::update(Clock &clock) {
    if (clock.getElapsedTime().asSeconds() > 1) {
        txtLevel.setString("Level " + to_string(getGameState().getLevel()));
        txtTimer.setString(to_string(timer));
        timer--;
        clock.restart();
        if (timer < 0) {
            timer = 3;
            if (getGameState().getLevel() > 1) {
                getGameState().resetGame();
            }
            getGameState().loadGame();
            getGameState().setState(State::play);
        }
    }
}

void LoadState::display(RenderWindow &window) {
    window.draw(backgroundImage);
    window.draw(txtLevel);
    window.draw(txtTimer);
    window.draw(txtKey);
}

/////// OverState ///////

OverState::OverState(Game *game) : State{game} {
    textures[tGameScreen].loadFromFile("resources/screen/i_s-over.png");
    backgroundImage.setTexture(textures[tGameScreen]);

    message.setFont(getGameState().getFont());
    message.setCharacterSize(35);
    message.setPosition(374, 255);

    txtKey.setFont(getGameState().getFont());
    txtKey.setCharacterSize(17);
    txtKey.setString("[ENTER] - Play Again\n       [ESC] - Quit");
    txtKey.setPosition(376, 300);

    txtFinalScore.setFont(getGameState().getFont());
    txtFinalScore.setCharacterSize(60);
    txtFinalScore.setOutlineThickness(2);
    txtFinalScore.setOutlineColor(Color(27,20,100));
    txtFinalScore.setPosition(85, 30);

}

OverState::~OverState() {}

void OverState::update(Clock &clock) {
    if (getGameState().gameClosed) {
        message.setString("GAME OVER");
    } else if (!getGameState().gameWon) {
        message.setString("GAME OVER");
    } else if (getGameState().gameWon) {
        message.setString("  VICTORY");
    }
    txtFinalScore.setString(" SCORE\n" + to_string(getGameState().getScore()));
}

void OverState::display(RenderWindow &window) {
    window.draw(backgroundImage);
    window.draw(message);
    window.draw(txtKey);
    window.draw(txtFinalScore);
}

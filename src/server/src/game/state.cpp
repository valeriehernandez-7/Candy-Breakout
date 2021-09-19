#include "game/game.h"
#include "game/state.h"

#include <cstdlib>
#include <iostream>

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

PlayState::PlayState(Game *game) : State(game), ballStatus{false}, playerScore{0}, playerMotion{5}, playerHealth{3}, playerSpeed{8}, timer{0} {
    levelImage.loadFromFile("resources/level/l-" + to_string(getGameState().getLevel()) + ".png");
    textures[tBall].loadFromFile("resources/assets/i_ball.png");
    textures[tBallPlus].loadFromFile("resources/assets/i_ball-plus.png");
    textures[tPlayer].loadFromFile("resources/assets/i_player.png");
    textures[tPlayerPlus].loadFromFile("resources/assets/i_player-plus.png");
    textures[tPlayerJunior].loadFromFile("resources/assets/i_player-junior.png");
    textures[tCandySingle].loadFromFile("resources/assets/i_candy-single.png");
    textures[tCandyDouble].loadFromFile("resources/assets/i_candy-double.png");
    textures[tCandyTriple].loadFromFile("resources/assets/i_candy-triple.png");
    textures[tCandyDeep].loadFromFile("resources/assets/i_candy-deep.png");
    textures[tBonus].loadFromFile("resources/assets/i_candy-bonus.png");
    textures[tGameScreen].loadFromFile("resources/screen/i_s-game.png");

    backgroundImage.setTexture(textures[tGameScreen]);

    balls.push_back(textures[tBall]);

    player.setTexture(textures[tPlayer]);
    player.setOrigin(player.getTexture()->getSize().x / 2, player.getTexture()->getSize().y / 2);
    player.setPosition(165, 360);

    playerHealthMeter.setPosition(25, 385);

    txtPlayerScore.setFont(getGameState().getFont());
    txtPlayerScore.setCharacterSize(17);
    txtPlayerScore.setPosition(85, 392);

    bonusInfo.setFont(getGameState().getFont());
    bonusInfo.setCharacterSize(30);
    bonusInfo.setPosition(395, 280);

    levelGenerator();
    bonusGenerator();
    ballGenerator();
}

PlayState::~PlayState() {}

void PlayState::update(Clock &clock) {
    // player info
    textures[tPlayerHealth].loadFromFile("resources/assets/i_life-" + to_string(playerHealth) + ".png");
    playerHealthMeter.setTexture(textures[tPlayerHealth]);
    txtPlayerScore.setString(to_string(playerScore));

    // player motion zone
    if (playerMotion == 1) {
        if (player.getPosition().x > 15 + player.getGlobalBounds().width / 2) {
            player.move(-playerSpeed, 0);
            if (!ballStatus) {
                for (unsigned b = 0; b < balls.size(); b++) {
                    balls[b].move(-playerSpeed, 0);
                }
            }
        }
    }
    if (playerMotion == 2) {
        if (player.getPosition().x < 315 - player.getGlobalBounds().width / 2) {
            player.move(playerSpeed, 0);
            if (!ballStatus) {
                for (unsigned b = 0; b < balls.size(); b++) {
                    balls[b].move(playerSpeed, 0);
                }
            }
        }
    }
    if (playerMotion == 3 && ballStatus) {
        player.rotate(-10);
    }
    if (playerMotion == 4 && ballStatus) {
        player.rotate(10);
    }

    // ball (Y) motion
    for (unsigned b = 0; b < balls.size(); b++) {
        balls[b].move(0, balls[b].offset.y);
    }

    // ball - candy (Y) collision
    for (unsigned i = 0; i < candies.size(); i++) {
        for (unsigned b = 0; b < balls.size(); b++) {
            if (onCollision(candies[i].sprite, balls[b])) {
                if (candies[i].resistance > 0) {
                    balls[b].offset.y = -balls[b].offset.y;
                }
                balls[b].move(0, balls[b].offset.y);
                candyManager(candies, i);
            }
        }
    }

    // ball (X) motion
    for (unsigned b = 0; b < balls.size(); b++) {
        balls[b].move(balls[b].offset.x, 0);
    }

    // ball - candy (X) collision
    for (unsigned i = 0; i < candies.size(); i++) {
        for (unsigned b = 0; b < balls.size(); b++) {
            if (onCollision(candies[i].sprite, balls[b])) {
                if (candies[i].resistance > 0) {
                    balls[b].offset.x = -balls[b].offset.x;
                }
                balls[b].move(balls[b].offset.x, 0);
                candyManager(candies, i);
            }
        }
    }

    // ball position
    for (unsigned b = 0; b < balls.size(); b++) {
        balls[b].position = balls[b].getPosition();
    }

    // ball - wall collision
    for (unsigned b = 0; b < balls.size(); b++) {
        if (balls[b].position.x < 15 || balls[b].position.x > 315 - balls[b].getGlobalBounds().width) {
            balls[b].offset.x = -balls[b].offset.x;
        }
        if (balls[b].position.y < 15) {
            balls[b].offset.y = -balls[b].offset.y;
        }
    }

    // ball - player collision
    for (unsigned b = 0; b < balls.size(); b++) {
        if (onCollision(player, balls[b])) {
            balls[b].offset.y = -balls[b].offset.y;
            balls[b].move(0, -balls[b].getGlobalBounds().height);
            if (balls[b].offset.x > 0) {
                balls[b].offset.x = rand() % 4 + 3;
            }
            if (balls[b].offset.x < 0) {
                balls[b].offset.x = (rand() % 4 + 3) * -1;
            }
        }
    }

    // ball lost
    for (unsigned b = 0; b < balls.size(); b++) {
        if (balls[b].position.y > window.getSize().y) {
            if (balls.size() > 1)
                balls.erase(balls.begin() + b);
            else if (playerHealth == 0) {
                getGameState().totalScore += playerScore;
                getGameState().setState(State::final);
            } else if (playerHealth-- > 0) {
                bonus.bonusDismiss(player, playerSpeed, candies, textures); // dismiss bonus
                ballGenerator();
            }
        }
    }

    // bonus release
    for (unsigned i = 0; i < candies.size(); i++) {
        if (candies[i].resistance <= 0 && candies[i].bonus) {
            candies[i].sprite.setTexture(textures[tBonus]);
            candies[i].sprite.move(0, 3);
            // bonus dismiss
            if (candies[i].sprite.getPosition().y > window.getSize().y) {
                candies.erase(candies.begin() + i);
            }
            // bonus reward
            else if (onCollision(candies[i].sprite, player)) {
                candies.erase(candies.begin() + i);
                bonus.bonusSelector();
                bonus.bonusManager(player, playerSpeed, playerHealth, balls, candies, textures);
                if (bonus.bonusType == "health") {
                    bonusInfo.setString("    1UP  ");
                } else if (bonus.bonusType == "balls") {
                    bonusInfo.setString("+ BALLS");
                } else if (bonus.bonusType == "super_ball") {
                    bonusInfo.setString("* BALL *");
                } else if (bonus.bonusType == "ball+Speed") {
                    bonusInfo.setString("+B SPEED");
                } else if (bonus.bonusType == "ball-Speed") {
                    bonusInfo.setString("-B SPEED");
                } else if (bonus.bonusType == "player+Width") {
                    bonusInfo.setString("+P WIDTH");
                } else if (bonus.bonusType == "player-Width") {
                    bonusInfo.setString("-P WIDTH");
                } else if (bonus.bonusType == "player+Speed") {
                    bonusInfo.setString("+P SPEED");
                }
                timer = 3;
            }
        }
    }

    // screen manager based on candy state
    if (candies.empty()) {
        getGameState().totalScore += playerScore;
        getGameState().levelNumber++;
        if (getGameState().levelNumber == 11) {
            getGameState().gameWon = true;
            getGameState().setState(State::final);
        } else {
            getGameState().setState(State::load);
        }
    }

    // counter loading screen
    if (clock.getElapsedTime().asSeconds() > 1 && timer > 0) {
        timer--;
        clock.restart();
        if (timer <= 0) {
            bonusInfo.setString("");
        }
    }
}

void PlayState::display(RenderWindow &window) {
    window.draw(backgroundImage);
    for (auto c: candies) {
        window.draw(c.sprite);
    }
    window.draw(bonusInfo);
    window.draw(player);
    for (auto b: balls) {
        window.draw(b);
    }
    window.draw(playerHealthMeter);
    window.draw(txtPlayerScore);
}

void PlayState::handler(Event &e) {
    if (e.type == Event::KeyPressed) {
        if (!ballStatus) {
            if (e.key.code == Keyboard::Up) {
                ballRelease();
            }
        }
        if (e.key.code == Keyboard::Left) {
            playerMotion = 1;
        }
        if (e.key.code == Keyboard::Right) {
            playerMotion = 2;
        }
        if (e.key.code == Keyboard::A) {
            playerMotion = 3;
        }
        if (e.key.code == Keyboard::D) {
            playerMotion = 4;
        }
        if (e.key.code == Keyboard::Escape) {
            getGameState().gameClosed = true;
            getGameState().totalScore += playerScore;
            getGameState().setState(State::final);
        }
    }
    if (e.type == Event::KeyReleased) {
        if (e.key.code == Keyboard::Left || e.key.code == Keyboard::Right) {
            playerMotion = 0;
        }
        if (e.key.code == Keyboard::A || e.key.code == Keyboard::D) {
            player.setRotation(0);
            playerMotion = 0;
        }
    }
}

bool PlayState::onCollision(Sprite &spriteA, Sprite &spriteB) {
    return spriteA.getGlobalBounds().intersects(spriteB.getGlobalBounds());
}

void PlayState::levelGenerator() {
    for (unsigned y = 0; y < levelImage.getSize().y; y++) {
        for (unsigned x = 0; x < levelImage.getSize().x; x++) {
            Color cellColor = levelImage.getPixel(x, y);
            if (cellColor != Color::White) {
                int randCandy = (rand() % (9 + 1 - 6)) + 6;
                Candy candy;
                if (cellColor == Color(0, 0, 0)) {
                    if (randCandy == 6) {
                        candy = Candy(textures[6], 1, 10);
                    }
                    if (randCandy == 7) {
                        candy = Candy(textures[7], 2, 15);
                    }
                    if (randCandy == 8) {
                        candy = Candy(textures[8], 3, 20);
                    }
                    if (randCandy == 9) {
                        candy = Candy(textures[9], 4, 25);
                    }
                }
                candy.sprite.setPosition(x * 30, y * 30);
                candies.push_back(candy);
            }
        }
    }
}

void PlayState::ballGenerator() {
    balls[0].offset.x = 0;
    balls[0].offset.y = 0;
    balls[0].setTexture(textures[tBall]);
    balls[0].setPosition(player.getPosition().x - balls[0].getTexture()->getSize().x / 2, player.getPosition().y - balls[0].getTexture()->getSize().y / 2);
    ballStatus = false;
}

void PlayState::ballRelease() {
    for (unsigned b = 0; b < balls.size(); b++) {
        balls[b].offset.x = -3;
        balls[b].offset.y = -5;
    }
    ballStatus = true;
}

void PlayState::bonusGenerator() {
    int bonusTotal = rand() % bonus.bonusCount;
    for (int i = 0; i < bonusTotal; i++) {
        int randIndex = rand() % candies.size();
        if (candies[randIndex].bonus) {
            randIndex = rand() % candies.size();
        }
        candies[randIndex].bonus = true;
    }
}

void PlayState::candyManager(vector<Candy> &candies, unsigned &id) {
    candies[id].resistance--;
    if (candies[id].resistance == 0) {
        playerScore += candies[id].points;
        if (!candies[id].bonus) {
            candies.erase(candies.begin() + id);
        }
    }
    if (candies[id].special && candies.size() > 0) {
        candies[id].resistance = 0;
        playerScore += 30;
        if (!candies[id].bonus) {
            candies.erase(candies.begin() + id);
        }
    }
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
    txtTimer.setOutlineColor(Color(89, 42, 30));
    txtTimer.setPosition(140, 50);

    txtKey.setFont(getGameState().getFont());
    txtKey.setCharacterSize(14);
    txtKey.setString("          [P] - Pause\n     [UP] - Ball Release\n[LEFT] & [RIGHT] - Move\n     [A] & [D] - Rotate\n          [ESC] - Quit");
    txtKey.setPosition(385, 280);
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
    txtFinalScore.setOutlineColor(Color(27, 20, 100));
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
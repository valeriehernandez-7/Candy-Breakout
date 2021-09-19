#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

using namespace std;
using namespace sf;

class PlayState;

struct Ball : public Sprite {
public:
    Ball(const Texture &texture) : Sprite(texture) {}

    Vector2f position;
    Vector2f offset;
};

struct Candy {
public:
    Candy() {}

    Candy(Texture &texture, int resistanceCandy, int pointsCandy, bool bonusCandy = false, bool specialCandy = false) {
        sprite.setTexture(texture);
        resistance = resistanceCandy;
        points = pointsCandy;
        bonus = bonusCandy;
        special = specialCandy;
    }

    Sprite sprite;
    int resistance, points;
    bool bonus, special;
};

struct Bonus {
public:
    Bonus() {
        bonusList.emplace_back("health");
        bonusList.emplace_back("balls");
        bonusList.emplace_back("super_ball");
        bonusList.emplace_back("ball+Speed");
        bonusList.emplace_back("ball-Speed");
        bonusList.emplace_back("player+Width");
        bonusList.emplace_back("player-Width");
        bonusList.emplace_back("player+Speed");
        bonusCount = bonusList.size();
    }

    int bonusCount;
    string bonusType;

    string bonusSelector() {
        int randIndex = rand() % bonusList.size();
        string bonusName = bonusList[randIndex];
        bonusType = bonusList[randIndex];
        bonusList.erase(bonusList.begin() + randIndex);
        return bonusName;
    }

    void bonusManager(Sprite &player, int &speed, int &lives, vector<Ball> &balls, vector<Candy> &candies, Texture textures[12]) {
        if (bonusType == "health") {
            if (lives < 3) {
                lives++;
            }
        }
        if (bonusType == "balls") {
            for (unsigned n = 1; n < 3; n++) {
                Ball ball(textures[0]);
                ball.setPosition(balls[n - 1].getPosition());
                ball.offset = balls[n - 1].offset;
                balls.push_back(ball);
            }
        }
        if (bonusType == "super_ball") {
            ballPowerChanged = true;
            for (unsigned n = 0; n < balls.size(); n++) {
                balls[n].setTexture(textures[1]);
            }
            for (unsigned n = 0; n < candies.size(); n++) {
                candies[n].special = true;
            }
        }
        if (bonusType == "ball+Speed") {
            for (unsigned n = 0; n < balls.size(); n++) {
                balls[n].offset.x = balls[n].offset.x * 2;
                balls[n].offset.y = balls[n].offset.y * 2;
            }
        }
        if (bonusType == "ball-Speed") {
            for (unsigned n = 0; n < balls.size(); n++) {
                balls[n].offset.x = balls[n].offset.x / 2;
                balls[n].offset.y = balls[n].offset.y / 2;
            }
        }
        if (bonusType == "player+Width") {
            playerWidthChanged = true;
            player.setOrigin(textures[3].getSize().x / 2, textures[3].getSize().y / 2);
            player.setTextureRect(IntRect(player.getTextureRect().left, player.getTextureRect().top, textures[3].getSize().x, textures[3].getSize().y));
            player.setTexture(textures[3]);
        }
        if (bonusType == "player-Width") {
            playerWidthChanged = true;
            player.setOrigin(textures[4].getSize().x / 2, textures[4].getSize().y / 2);
            player.setTextureRect(IntRect(player.getTextureRect().left, player.getTextureRect().top, textures[4].getSize().x, textures[4].getSize().y));
            player.setTexture(textures[4]);
        }
        if (bonusType == "player+Speed") {
            playerSpeedChanged = true;
            speed = 12;
        }
    }

    void bonusDismiss(Sprite &player, int &speed, vector<Candy> &candies, Texture textures[12]) {
        if (playerSpeedChanged) {
            playerSpeedChanged = false;
            speed = 8;
        }
        if (playerWidthChanged) {
            playerWidthChanged = false;
            player.setOrigin(textures[2].getSize().x / 2, textures[2].getSize().y / 2);
            player.setTextureRect(IntRect(player.getTextureRect().left, player.getTextureRect().top, textures[2].getSize().x, textures[2].getSize().y));
            player.setTexture(textures[2]);
        }
        if (ballPowerChanged) {
            ballPowerChanged = false;
            for (unsigned n = 0; n < candies.size(); n++) {
                candies[n].special = false;
            }
        }
    }

private:
    vector<string> bonusList;
    bool playerSpeedChanged;
    bool playerWidthChanged;
    bool ballPowerChanged;
};
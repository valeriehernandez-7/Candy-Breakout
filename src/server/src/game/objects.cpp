/*!
 * \include SFML/Graphics.hpp
 * \include string
 * \include vector
 */
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

/*!
 * \namespace std General-purpose polymorphic function wrapper.
 * \namespace sf Simple and Fast Multimedia Library provides a simple interface to the various components of your PC.
 */
using namespace std;
using namespace sf;

class PlayState;

/*! \struct Ball
 *  \brief Contains the features of the Ball as a game object.
 */
struct Ball : public Sprite {
public:
    /*!
     *  \brief Ball constructor.
     *  \param texture a reference to the texture of the game object Ball (tBall, tBallPlus).
     */
    Ball(const Texture &texture) : Sprite(texture) {}

    Vector2f position; /*!< Defines the Ball position as a vector with two coordinates (x , y). */
    Vector2f offset; /*!< Defines the Ball motion as a vector with two coordinates (x , y). */
};

/*! \struct Candy
 *  \brief Contains the features of the Candy as a game object.
 */
struct Candy {
public:
    Candy() {} /*!< Candy empty constructor. */

    /*!
     *  \brief Candy constructor.
     *  \param texture a reference to the texture of the game object Candy (tCandySingle, tCandyDouble, tCandyTriple, tCandyDeep, tBonus).
     *  \param resistanceCandy an integer defining the number of hits the Candy should receive from the Ball.
     *  \param pointsCandy an integer representing the number of points the Player gets when the Candy is destroyed.
     *  \param bonusCandy a boolean defining if the Candy when destroyed will release a Bonus.
     *  \param specialCandy a boolean defining whether the Candy was destroyed by a super Ball.
     */
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

/*! \struct Bonus
 *  \brief Contains the features of the Bonus Candy as a game object.
 */
struct Bonus {
public:
    /*! \brief Bonus constructor.
     *  It adds to the bonusList vector the possible types of Bonus Candy that the player can get during the play.
     *  Finally it gets the number of Bonus according to the size of the vector.
     */
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

    int bonusCount; /*!< Amount of Bonus types. */
    string bonusType; /*!< Bonus description. */

    /*! \fn string bonusSelector()
     *  \brief Selects the Bonus type to be released.
     *  It randomly chooses the Bonus type that will be released.
     *  Then remove from bonusList the chosen one to avoid repeating the same type on the same level during the game.
     *  \return string as Bonus name
     */
    string bonusSelector() {
        int randIndex = rand() % bonusList.size();
        string bonusName = bonusList[randIndex]; /*!< Bonus name. */
        bonusType = bonusList[randIndex];
        bonusList.erase(bonusList.begin() + randIndex);
        return bonusName;
    }

    /*! \fn void bonusManager(Sprite &player, int &speed, int &lives, vector<Ball> &balls, vector<Candy> &candies, Texture textures[12])
     *  \brief Defines the game events that will happen based on the Bonus obtained by the player.
     *  \param player a reference to the sprite of the game object Player (tPlayer, tPlayerPlus, tPlayerJunior).
     *  \param speed a reference to the integer representing the Player 's speed.
     *  \param lives a reference to the integer representing the Player 's amount of lives.
     *  \param balls a reference to the vector containing the Ball objects on the game screen.
     *  \param candies a reference to the vector containing the Candy objects on the game screen.
     *  \param textures a pointer to the textures of the game objects.
     */
    void bonusManager(Sprite &player, int &speed, int &lives, vector<Ball> &balls, vector<Candy> &candies, Texture textures[12]) {
        // increases the player's amount of lives
        if (bonusType == "health") {
            if (lives < 3) {
                lives++;
            }
        }
        // increases the amount of balls on the game screen
        if (bonusType == "balls") {
            for (unsigned n = 1; n < 3; n++) {
                Ball ball(textures[0]);
                ball.setPosition(balls[n - 1].getPosition());
                ball.offset = balls[n - 1].offset;
                balls.push_back(ball);
            }
        }
        // changes the texture of the balls on the game screen,
        // also sets all candies to be smashed with a single hit  and increase 30 score points
        if (bonusType == "super_ball") {
            ballPowerChanged = true;
            for (unsigned n = 0; n < balls.size(); n++) {
                balls[n].setTexture(textures[1]);
            }
            for (unsigned n = 0; n < candies.size(); n++) {
                candies[n].special = true;
            }
        }
        // increases the speed of the balls on the game screen
        if (bonusType == "ball+Speed") {
            for (unsigned n = 0; n < balls.size(); n++) {
                balls[n].offset.x = balls[n].offset.x * 2;
                balls[n].offset.y = balls[n].offset.y * 2;
            }
        }
        // decreases the speed of the balls on the game screen
        if (bonusType == "ball-Speed") {
            for (unsigned n = 0; n < balls.size(); n++) {
                balls[n].offset.x = balls[n].offset.x / 2;
                balls[n].offset.y = balls[n].offset.y / 2;
            }
        }
        // increases the player's width
        if (bonusType == "player+Width") {
            playerWidthChanged = true;
            player.setOrigin(textures[3].getSize().x / 2, textures[3].getSize().y / 2);
            player.setTextureRect(IntRect(player.getTextureRect().left, player.getTextureRect().top, textures[3].getSize().x, textures[3].getSize().y));
            player.setTexture(textures[3]);
        }
        // decreases the player's width
        if (bonusType == "player-Width") {
            playerWidthChanged = true;
            player.setOrigin(textures[4].getSize().x / 2, textures[4].getSize().y / 2);
            player.setTextureRect(IntRect(player.getTextureRect().left, player.getTextureRect().top, textures[4].getSize().x, textures[4].getSize().y));
            player.setTexture(textures[4]);
        }
        // increases the player's speed
        if (bonusType == "player+Speed") {
            playerSpeedChanged = true;
            speed = 12;
        }
    }

    /*! \fn void bonusDismiss(Sprite &player, int &speed, vector<Candy> &candies, Texture textures[12])
     *  \brief Defines the game events that will happen after losing all balls in game screen based on the status of the game objects affected by the Bonus.
     *  \param player a reference to the sprite of the game object Player (tPlayer, tPlayerPlus, tPlayerJunior).
     *  \param speed a reference to the integer representing the Player 's speed.
     *  \param candies a reference to the vector containing the Candy objects on the game screen.
     *  \param textures a pointer to the textures of the game objects.
     */
    void bonusDismiss(Sprite &player, int &speed, vector<Candy> &candies, Texture textures[12]) {
        // resets the player's speed
        if (playerSpeedChanged) {
            playerSpeedChanged = false;
            speed = 8;
        }
        // resets the player's width
        if (playerWidthChanged) {
            playerWidthChanged = false;
            player.setOrigin(textures[2].getSize().x / 2, textures[2].getSize().y / 2);
            player.setTextureRect(IntRect(player.getTextureRect().left, player.getTextureRect().top, textures[2].getSize().x, textures[2].getSize().y));
            player.setTexture(textures[2]);
        }
        // resets the ball status and the status of candies on game screen
        if (ballPowerChanged) {
            ballPowerChanged = false;
            for (unsigned n = 0; n < candies.size(); n++) {
                candies[n].special = false;
            }
        }
    }

private:
    vector<string> bonusList; /*!< Contains the Bonus type descriptions. */
    bool playerSpeedChanged; /*!< Determines if the Player 's speed has been modified by a Bonus (increased or decreased). */
    bool playerWidthChanged; /*!< Determines if the Player size has been modified by a Bonus (increased or decreased). */
    bool ballPowerChanged; /*!< Determines if the Ball has been modified by a Bonus. */
};
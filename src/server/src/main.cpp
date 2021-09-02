#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "CANDY BREAKOUT");
    sf::RectangleShape player;
    player.setSize(sf::Vector2f(100, 10));
    player.setFillColor(sf::Color::Blue);
    player.setPosition(350, 550);
    sf::CircleShape ball(10.f);
    ball.setFillColor(sf::Color::Red);
    ball.setPosition(400, 500);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        window.draw(player);
        window.draw(ball);
        window.display();
    }
    return 0;
}
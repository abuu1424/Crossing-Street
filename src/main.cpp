#include<SFML/Graphics.hpp>
#include "Utils.h"

int main ()
{
    const int W = static_cast<int>(sf::VideoMode::getDesktopMode().width);
    const int H = static_cast<int>(sf::VideoMode::getDesktopMode().height);

    sf::RenderWindow window(
        sf::VideoMode::getDesktopMode(),
        "Crossing Street",
        sf::Style::Default
    );
    window.setFramerateLimit(60);

    sf::Clock clock;

    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();
        //Nút bấm cơ bản
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
        }
        // Update

        //Render
        window.clear(sf::Color(20, 22, 48));
        window.display();
    }
    return 0;
}
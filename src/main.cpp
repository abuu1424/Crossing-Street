#include <SFML/Graphics.hpp>
#include "Utils.h"
#include "CPEOPLE.h"

int main() {
    sf::RenderWindow window(
        sf::VideoMode(Win_W, Win_H),
        "Crossing Street",
        sf::Style::Default
    );
    window.setFramerateLimit(60);

    sf::Clock clock;
    CPEOPLE player;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
        }


        //Update
        player.Move(dt);
        player.update(dt);

        window.clear(sf::Color(20, 22, 48));
        player.Draw(window);
        window.display();
    }
    return 0;
}
#include <SFML/Graphics.hpp>
#include "Utils.h"
#include "CGAME.h"

int main() {
    sf::RenderWindow window(
        sf::VideoMode(Win_W, Win_H),
        "Crossing Street",
        sf::Style::Default
    );
    window.setFramerateLimit(FPS);

    CGAME game(window);
    game.run();

    return 0;
}
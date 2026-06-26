#include <SFML/Graphics.hpp>
#include "Utils.h"
#include "CGAME.h"
#include <filesystem>
#include <cstdio>

int main() {
    printf("Running from: %s\n",
        std::filesystem::current_path().string().c_str());
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
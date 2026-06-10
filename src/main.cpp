#include <SFML/Graphics.hpp>
#include "Utils.h"
#include "CPEOPLE.h"
#include "CDINOSOUR.h"

int main() {
    sf::RenderWindow window(
        sf::VideoMode(Win_W, Win_H),
        "Crossing Street",
        sf::Style::Default
    );
    window.setFramerateLimit(60);

    sf::Clock clock;
    //Player
    CPEOPLE player;
    //Dino
    CDINOSOUR dino1(150.f,  1.f);
    CDINOSOUR dino2(200.f, -1.f);
    CDINOSOUR dino3(120.f,  1.f);

    dino1.loadSprite("assets/sprites/lv1_sp/dino/dino1.png",   0, 200);
    dino2.loadSprite("assets/sprites/lv1_sp/dino/dino2.png", 600, 300);
    dino3.loadSprite("assets/sprites/lv1_sp/dino/dino3.png", 300, 400);

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
        //Player
        player.Move(dt);
        player.update(dt);
        //Dino
        dino1.Move(dt); dino1.update(dt);
        dino2.Move(dt); dino2.update(dt);
        dino3.Move(dt); dino3.update(dt);
        //window
        window.clear(sf::Color(20, 22, 48));
        //Dino
        dino1.Draw(window);
        dino2.Draw(window);
        dino3.Draw(window);
        //Player
        player.Draw(window);
        window.display();
    }
    return 0;
}
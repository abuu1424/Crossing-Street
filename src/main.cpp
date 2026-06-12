#include <SFML/Graphics.hpp>
#include <vector>
#include "Utils.h"
#include "CPEOPLE.h"
#include "CDINOSOUR.h"
#include "CBIRD.h"
#include "CTRAFFIC_LV1.h"

int main() {
    sf::RenderWindow window(
        sf::VideoMode(Win_W, Win_H),
        "Crossing Street",
        sf::Style::Default
    );
    window.setFramerateLimit(FPS);

    sf::Clock clock;

    // ── Player
    CPEOPLE player;

    // ── Vật cản
    CDINOSOUR dino1(150.f,  1.f);
    CDINOSOUR dino2(200.f, -1.f);
    CDINOSOUR dino3(120.f,  1.f);

    // ── Động vật bay
    CBIRD bird1(150.f, 1.f);

    // ── Load sprite
    dino1.loadSprite("assets/sprites/lv1_sp/dino/dino1.png",   0, 200);
    dino2.loadSprite("assets/sprites/lv1_sp/dino/dino2.png", 600, 300);
    dino3.loadSprite("assets/sprites/lv1_sp/dino/dino3.png", 300, 400);
    bird1.loadSprite("assets/sprites/lv1_sp/bird/bay.png",   900,   0);

    std::vector<CVEHICLE*> obstacles = { &dino1, &dino2, &dino3 };
    std::vector<CANIMAL*>  animals   = { &bird1 };

    // ── Traffic light
    CTRAFFIC_LV1 torch(obstacles);
    torch.loadSprite("assets/sprites/lv1_sp/traffic/torch.png", 10, 180);

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        // ── Events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
                if (event.key.code == sf::Keyboard::R) {
                    player.setDead(false);
                    player.setPosition(
                        Win_W / 2.f - Player_W / 2.f,
                        Win_H / 2.f - Player_H / 2.f
                    );
                }
            }
        }

        // ── Update
        if (!player.isDead()) {
            player.Move(dt);
            player.update(dt);
        }

        for (auto* obs : obstacles) { obs->Move(dt); obs->update(dt); }
        for (auto* ani : animals)   { ani->Move(dt); ani->update(dt); }
        torch.update(dt);

        // ── Collision detection
        if (!player.isDead()) {
            sf::FloatRect pb = player.getBounds();
            for (auto* obs : obstacles) {
                if (pb.intersects(obs->getBounds())) {
                    player.setDead(true);
                    printf("DEAD!\n");
                    break;
                }
            }
            for (auto* ani : animals) {
                if (pb.intersects(ani->getBounds())) {
                    player.setDead(true);
                    printf("DEAD!\n");
                    break;
                }
            }
        }

        // ── Render
        window.clear(sf::Color(20, 22, 48));
        for (auto* obs : obstacles) obs->Draw(window);
        for (auto* ani : animals)   ani->Draw(window);
        torch.Draw(window);
        player.Draw(window);
        window.display();
    }

    return 0;
}
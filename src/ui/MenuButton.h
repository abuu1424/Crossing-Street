#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "TextureManager.h"

struct MenuButton {
    sf::Sprite sprite;
    const sf::Texture* pTexture = nullptr;
    const sf::Texture* pHoverTexture = nullptr;
    sf::Text label;
    float baseScale  = 1.f;   // scale bình thường
    float hoverScale = 1.15f; // scale khi hover
    float curScale   = 1.f;   // scale hiện tại
    bool  hovered    = false;
    bool  hasHoverTex = false;

    void setup(const std::string &texPath, const std::string &textLabel,
               sf::Font &font, float x, float y,
               const std::string &hoverTexPath = "", unsigned int charSize = 26) {
        pTexture = &TextureManager::getInstance().getTexture(texPath);

        hasHoverTex = false;
        if (!hoverTexPath.empty()) {
            pHoverTexture = &TextureManager::getInstance().getTexture(hoverTexPath);
            hasHoverTex = true;
        }

        sprite.setTexture(*pTexture);
        float w = pTexture->getSize().x;
        float h = pTexture->getSize().y;
        sprite.setOrigin(w / 2.f, h / 2.f);
        sprite.setPosition(x, y);
        sprite.setScale(baseScale, baseScale);

        label.setFont(font);
        label.setString(textLabel);
        label.setCharacterSize(charSize);
        label.setFillColor(sf::Color::White);
        label.setOutlineColor(sf::Color::Black);
        label.setOutlineThickness(2.f);

        sf::FloatRect lb = label.getLocalBounds();
        label.setOrigin(lb.left + lb.width / 2.f, lb.top + lb.height / 2.f);
        label.setPosition(x, y);
    }

    void update(sf::Vector2f mousePos, float dt) {
        sf::FloatRect bounds = sprite.getGlobalBounds();
        hovered = bounds.contains(mousePos);

        if (hasHoverTex && pHoverTexture && pTexture) {
            sprite.setTexture(hovered ? *pHoverTexture : *pTexture);
        }

        float target = hovered ? hoverScale : baseScale;
        curScale += (target - curScale) * 12.f * dt;

        sprite.setScale(curScale, curScale);
        label.setScale(curScale, curScale);
    }

    void draw(sf::RenderWindow &w) const {
        w.draw(sprite);
        w.draw(label);
    }

    bool contains(sf::Vector2f pos) const {
        return sprite.getGlobalBounds().contains(pos);
    }
};

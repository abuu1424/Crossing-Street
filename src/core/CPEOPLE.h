#ifndef CROSSINGGAME_CPEOPLE_H
#define CROSSINGGAME_CPEOPLE_H

#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include"Animation.h"
#include "Utils.h"

class CPEOPLE
{
    sf::Sprite mSprite;
    sf::Texture mTexture;
    Animation* mAnim = nullptr;

    sf::Vector2f mPosition;

    float mSpeed;
    bool mIsDead;
    bool mIsFinish;

public:
    CPEOPLE();

    bool loadSprite(const std::string& texturePath);

    void Move(float dt);
    void update(float dt);

    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;

    sf::FloatRect getBounds() const;

    bool isDead() const;
    bool isFinish() const;

    void setDead(bool dead);
    void setFinish(bool finish);

    void Draw(sf::RenderWindow& window);

};

#endif // CROSSINGGAME_CPEOPLE_H
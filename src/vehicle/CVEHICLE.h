
#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include "Animation.h"
#include "Utils.h"

class CVEHICLE {
protected:
    sf::Texture                mTexture;
    sf::Sprite                 mSprite;
    std::unique_ptr<Animation> mAnim;
    float                      mSpeed;
    float                      mDirection;  // 1 = phải, -1 = trái
    bool                       mIsStopped; // đèn đỏ → dừng lại

public:
    CVEHICLE(float speed = 150.f, float direction = 1.f);
    virtual ~CVEHICLE();

    virtual bool loadSprite(const std::string& path, float x, float y);
    virtual void Move(float dt);
    virtual void update(float dt);
    virtual void Draw(sf::RenderWindow& w);

    sf::FloatRect getBounds() const;

    void stop()   { mIsStopped = true;  }  // đèn đỏ
    void resume() { mIsStopped = false; }  // đèn xanh
    bool isStopped() const { return mIsStopped; }

    float getSpeed()     const { return mSpeed;     }
    float getDirection() const { return mDirection; }
    sf::Vector2f getPosition() const { return mSprite.getPosition(); }
    void setPosition(float x, float y) { mSprite.setPosition(x, y); }
};

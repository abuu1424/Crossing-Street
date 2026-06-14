#include "CANIMAL.h"

CANIMAL::CANIMAL(float speed, float direction)
    : mSpeed(speed), mDirection(direction) {
}

void CANIMAL::Move(float dt) {
    mSprite.move(mSpeed * mDirection * dt, 0.f);

    sf::FloatRect bounds = mSprite.getGlobalBounds();

    if (mDirection > 0.f && bounds.left > Win_W) {
        mSprite.setPosition(-ANIMAL_W, mSprite.getPosition().y);
    }
    else if (mDirection < 0.f && bounds.left + bounds.width < 0.f) {
        mSprite.setPosition(Win_W, mSprite.getPosition().y);
    }
}

void CANIMAL::Draw(sf::RenderWindow& w) {
    w.draw(mSprite);
}

bool CANIMAL::loadSprite(const std::string& texturePath, float x, float y) {
    if (!mTexture.loadFromFile(texturePath)) {
        printf("FAILED: %s\n", texturePath.c_str());
        return false;
    }
    mSprite.setTexture(mTexture);
    mSprite.setTextureRect(sf::IntRect(0, 0,
        mTexture.getSize().x,
        mTexture.getSize().y));
    mSprite.setScale(
        ANIMAL_W / static_cast<float>(mTexture.getSize().x),
        ANIMAL_H / static_cast<float>(mTexture.getSize().y)
    );
    mSprite.setPosition(x, y);
    return true;
}

void CANIMAL::update(float dt) {
    // Subclass override de goi mAnim.update(dt)
}
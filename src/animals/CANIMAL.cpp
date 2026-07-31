#include "CANIMAL.h"
#include "TextureManager.h"

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
    const auto& texture = TextureManager::getInstance().getTexture(texturePath);
    mSprite.setTexture(texture);
    mSprite.setTextureRect(sf::IntRect(0, 0,
        texture.getSize().x,
        texture.getSize().y));
    float scale = std::min(ANIMAL_W / static_cast<float>(texture.getSize().x),
                           ANIMAL_H / static_cast<float>(texture.getSize().y));
    if (mDirection > 0.f) {
        mSprite.setScale(-scale, scale);
        mSprite.setOrigin(static_cast<float>(texture.getSize().x), 0.f);
    } else {
        mSprite.setScale(scale, scale);
        mSprite.setOrigin(0.f, 0.f);
    }
    mSprite.setPosition(x, y);
    return true;
}

void CANIMAL::update(float dt) {
    if (mAnim) mAnim->update(dt);
}
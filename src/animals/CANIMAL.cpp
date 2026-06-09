#include "CANIMAL.h"

CANIMAL::CANIMAL(float speed, float direction)
    : mSpeed(speed), mDirection(direction) {
}

void CANIMAL::Move(float dt) {
    // SFML 2.x: move() nhận 2 float riêng, không cần Vector2f
    mSprite.move(mSpeed * mDirection * dt, 0.f);

    sf::FloatRect bounds = mSprite.getGlobalBounds();

    // SFML 2.x: dùng .left và .width thay vì .position.x và .size.x
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

void CANIMAL::update(float dt) {
    // Subclass override de goi mAnim.update(dt)
}
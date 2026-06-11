#include "CTRUCK.h"

CTRUCK::CTRUCK(float direction)
    : CVEHICLE(100.f, direction),
    mTruckAnim(mSprite,
        "assets/sprites/obstacles/modern/truck.png", 96, 48, 4, 1, FRAME_TIME) {
    if (direction < 0.f)
        mSprite.setScale(-1.f, 1.f);
}

void CTRUCK::update(float dt) {
    mTruckAnim.update(dt);
    Move(dt);
}

void CTRUCK::Draw(sf::RenderWindow& w) {
    w.draw(mSprite);
}
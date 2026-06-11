#include "../../vehicle/CCAR.h"

CCAR::CCAR(float direction)
    : CVEHICLE(150.f, direction),
    mCarAnim(mSprite,
        "assets/sprites/obstacles/modern/car.png", 64, 32, 4, 1, FRAME_TIME){
    if (direction < 0.f)
        mSprite.setScale(-1.f, 1.f);
}

void CCAR::update(float dt) {
    mCarAnim.update(dt);
    Move(dt);
}

void CCAR::Draw(sf::RenderWindow& w){
    w.draw(mSprite);
}
#include "CBIRD.h"
CBIRD::CBIRD(float direction): CANIMAL(BASE_SPEED, direction),mAnim(mSprite,"assets/sprites/lv1_sp/animal/bay.png",48, 48,  2, 2, FRAME_TIME) 
{
    if (direction < 0.f)
        mSprite.setScale(-1.f, 1.f);
}

void CBIRD::update(float dt) {
    mAnim.update(dt);
    Move(dt); 
}

void CBIRD::Draw(sf::RenderWindow& w) {
    w.draw(mSprite);
}

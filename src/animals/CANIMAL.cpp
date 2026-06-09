#include "CANIMAL.h"
#include "../Utils.h"
CANIMAL::CANIMAL(float speed, float direction)
    : mSpeed(speed), mDirection(direction),
     mSprite(mTexture) {}
 
void CANIMAL::Move(float dt) {
    mSprite.move(sf::Vector2f(mSpeed * mDirection * dt, 0.f));
    sf::FloatRect bounds = mSprite.getGlobalBounds();
 
    // Wrap around: bay ra ngoai man hinh thi xuat hien lai phia doi dien
    if (mDirection > 0.f &&  bounds.position.x > WIN_W) {
        // Bay sang phai, ra khoi man hinh ben phai → xuat hien ben trai
       mSprite.setPosition(sf::Vector2f(-ANIMAL_W, mSprite.getPosition().y));
    }
    else if (mDirection < 0.f && bounds.position.x + bounds.size.x < 0.f) {
        mSprite.setPosition(sf::Vector2f(WIN_W, mSprite.getPosition().y));
    }
}
 
void CANIMAL::Draw(sf::RenderWindow& w) {
    w.draw(mSprite);
}
 
void CANIMAL::update(float dt) {
    // Subclass override de goi mAnim.update(dt)
}
 
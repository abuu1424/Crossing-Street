#include "CPEOPLE.h"
#include <cmath>

CPEOPLE::CPEOPLE()
{
    mSpeed = Player_Speed;
    mIsDead = false;
    mIsFinish = false;

    mPosition = sf::Vector2f(
        Win_W / 2.f - Player_W / 2.f,
        Win_H - Safe_Zone_H + (Safe_Zone_H - Player_H) / 2.f
    );

    mSprite.setPosition(mPosition);

    loadSprite("assets/sprites/lv1_sp/player/player.png");
}

bool CPEOPLE::loadSprite(const std::string& texturePath) {
    delete mAnim;
    mAnim = new Animation(
        mSprite,
        texturePath,
        64, 64,   // frameW, frameH
        4, 4,     // 4 cột, 4 hàng = 16 frame
        Frame_Time
    );

    mSprite.setScale(
        Player_W / 64.f,
        Player_H / 64.f
    );

    mSprite.setPosition(mPosition);
    return true;
}

void CPEOPLE::Move(float dt)
{
    if (mIsDead || mIsFinish)
    {
        return;
    }

    sf::Vector2f direction(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        direction.y -= 1.f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        direction.y += 1.f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        direction.x -= 1.f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        direction.x += 1.f;
    }

    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length != 0.f)
    {
        direction.x /= length;
        direction.y /= length;
    }

    mPosition.x += direction.x * mSpeed * dt;
    mPosition.y += direction.y * mSpeed * dt;

    if (mPosition.x < 0.f)
    {
        mPosition.x = 0.f;
    }

    if (mPosition.x > Win_W - Player_W)
    {
        mPosition.x = Win_W - Player_W;
    }

    if (mPosition.y < 0.f)
    {
        mPosition.y = 0.f;
    }

    if (mPosition.y > Win_H - Player_H)
    {
        mPosition.y = Win_H - Player_H;
    }

    mSprite.setPosition(mPosition);
}

void CPEOPLE::update(float dt) {
    if (mAnim) mAnim->update(dt);
}

void CPEOPLE::setPosition(float x, float y)
{
    mPosition = sf::Vector2f(x, y);
    mSprite.setPosition(mPosition);
}

sf::Vector2f CPEOPLE::getPosition() const
{
    return mPosition;
}

sf::FloatRect CPEOPLE::getBounds() const
{
    return mSprite.getGlobalBounds();
}

bool CPEOPLE::isDead() const
{
    return mIsDead;
}

bool CPEOPLE::isFinish() const
{
    return mIsFinish;
}

void CPEOPLE::setDead(bool dead)
{
    mIsDead = dead;
}

void CPEOPLE::setFinish(bool finish)
{
    mIsFinish = finish;
}

void CPEOPLE::Draw(sf::RenderWindow& window)
{
    window.draw(mSprite);
}
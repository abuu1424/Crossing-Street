#include "CPEOPLE.h"
#include <cmath>

CPEOPLE::CPEOPLE()
{
    mSpeed = Player_Speed;
    mIsDead = false;
    mIsFinish = false;

    mPosition = sf::Vector2f(
        Win_W / 2.f - Player_W / 2.f,
        Win_H / 2.f - Player_H / 2.f
    );

    mSprite.setPosition(mPosition);

    loadSprite("assets/sprites/lv1_sp/player/player.png");
}

CPEOPLE::~CPEOPLE()
{
    delete mAnim;
}

//Load Sprite
bool CPEOPLE::loadSprite(const std::string& texturePath) {
    if (!mTexture.loadFromFile(texturePath)) {
        printf("FAILED: %s\n", texturePath.c_str());
        return false;
    }
    delete mAnim;
    mAnim = new Animation(
        mSprite, mTexture,
        64, 64,   // frameW, frameH
        4, 1,     // 4 cột, 1 hàng
        Frame_Time
    );
    mSprite.setScale(Player_W / 64.f, Player_H / 64.f);
    mSprite.setPosition(mPosition);
    return true;
}

void CPEOPLE::reloadSprite(const std::string& texturePath) {
    if (!mTexture.loadFromFile(texturePath)) {
        printf("FAILED reload sprite: %s\n", texturePath.c_str());
        return;
    }
    delete mAnim;
    mAnim = new Animation(mSprite, mTexture,
        64, 64, 4, 1, Frame_Time);
    mSprite.setScale(Player_W / 64.f, Player_H / 64.f);
    //KO load vị trí mới
}

void CPEOPLE::update(float dt) {
    if (!mAnim) return;

    if (mIsMoving) {
        mAnim->update(dt);
        // Đổi sang đúng hàng
        sf::IntRect rect = mSprite.getTextureRect();
        rect.top = mRow * 64;
        mSprite.setTextureRect(rect);
    } else {
        // Đứng yên — hiện fram đầu tiên
        mSprite.setTextureRect(sf::IntRect(0, mRow * 64, 64, 64));
    }
}

// Move
void CPEOPLE::Move(float dt)
{
    if (mIsDead || mIsFinish) return;

    sf::Vector2f dir(0.f, 0.f);
    mIsMoving = false;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        dir.y -= 1.f; mRow = 3; mIsMoving = true;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        dir.y += 1.f; mRow = 0; mIsMoving = true;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        dir.x -= 1.f; mRow = 1; mIsMoving = true;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        dir.x += 1.f; mRow = 2; mIsMoving = true;
    }

    mPosition.x += dir.x * mSpeed * dt;
    mPosition.y += dir.y * mSpeed * dt;
    mPosition.x = std::max(0.f, std::min(mPosition.x, (float)Win_W - Player_W));
    mPosition.y = std::max(0.f, std::min(mPosition.y, (float)Win_H - Player_H));
    mSprite.setPosition(mPosition);
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
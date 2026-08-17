#include "CVEHICLE.h"
#include "TextureManager.h"
#include <cstdio>
#include <memory>
#include <algorithm>


CVEHICLE::CVEHICLE(float speed, float direction)
    : mSpeed(speed), mDirection(direction), mIsStopped(false) {}

CVEHICLE::~CVEHICLE() = default;

bool CVEHICLE::loadSprite(const std::string& path, float x, float y) {
    const auto& texture = TextureManager::getInstance().getTexture(path);

    // Tự tính frameW — giả sử spritesheet 4 cột 1 hàng
    // Subclass override nếu layout khác
    int frameW = texture.getSize().x / 4;
    int frameH = texture.getSize().y;

    mAnim = std::make_unique<Animation>(mSprite, texture,
        frameW, frameH,
        4, 1,
        Frame_Time);

    float scale = std::min(96.f / static_cast<float>(frameW),
                           56.f / static_cast<float>(frameH));
    if (mDirection > 0.f) {
        mSprite.setScale(-scale, scale);
        mSprite.setOrigin(static_cast<float>(frameW), 0.f);
    } else {
        mSprite.setScale(scale, scale);
        mSprite.setOrigin(0.f, 0.f);
    }
    mSprite.setPosition(x, y);
    return true;
}

void CVEHICLE::Move(float dt) {
    if (mIsStopped) return;  // đèn đỏ → không di chuyển

    mSprite.move(mSpeed * mDirection * dt, 0.f);
    sf::FloatRect bounds = mSprite.getGlobalBounds();

    // Wrap around
    if (mDirection > 0.f && bounds.left > Win_W)
        mSprite.setPosition(-bounds.width, mSprite.getPosition().y);
    else if (mDirection < 0.f && bounds.left + bounds.width < 0.f)
        mSprite.setPosition(Win_W, mSprite.getPosition().y);
}

void CVEHICLE::update(float dt) {
    if (mAnim && !mIsStopped)
        mAnim->update(dt);
}

void CVEHICLE::Draw(sf::RenderWindow& w) {
    w.draw(mSprite);
}

sf::FloatRect CVEHICLE::getBounds() const {
    return mSprite.getGlobalBounds();
}

sf::FloatRect CVEHICLE::getHitbox() const {
    sf::FloatRect r = mSprite.getGlobalBounds();
    float dx = r.width * 0.22f;
    float dy = r.height * 0.28f;
    return sf::FloatRect(r.left + dx, r.top + dy, r.width - dx * 2.f, r.height - dy * 2.f);
}
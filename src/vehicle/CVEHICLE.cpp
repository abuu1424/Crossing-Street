#include "CVEHICLE.h"
#include <cstdio>

CVEHICLE::CVEHICLE(float speed, float direction)
    : mSpeed(speed), mDirection(direction), mIsStopped(false) {}

CVEHICLE::~CVEHICLE() {
    delete mAnim;
}

bool CVEHICLE::loadSprite(const std::string& path, float x, float y) {
    if (!mTexture.loadFromFile(path)) {
        printf("FAILED: %s\n", path.c_str());
        return false;
    }

    // Tự tính frameW — giả sử spritesheet 4 cột 1 hàng
    // Subclass override nếu layout khác
    int frameW = mTexture.getSize().x / 4;
    int frameH = mTexture.getSize().y;

    delete mAnim;
    mAnim = new Animation(mSprite, mTexture,
        frameW, frameH,
        4, 1,
        Frame_Time);

    mSprite.setScale(
        96.f / static_cast<float>(frameW),
        56.f / static_cast<float>(frameH)
    );
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

//Kiểm tra có đụng xe hoặc chạm người, là hàm tính toán khoảng cách xung quanh vật
sf::FloatRect CVEHICLE::getBounds() const {
    return mSprite.getGlobalBounds();
}
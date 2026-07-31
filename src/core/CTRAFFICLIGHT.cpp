#include "CTRAFFICLIGHT.h"
#include "CVEHICLE.h"
#include "TextureManager.h"

CTRAFFICLIGHT::CTRAFFICLIGHT(std::vector<std::unique_ptr<CVEHICLE>>& vehicles)
    : mVehicles(vehicles) {}

CTRAFFICLIGHT::~CTRAFFICLIGHT() = default;

bool CTRAFFICLIGHT::loadSprite(const std::string& path, float x, float y) {
    const auto& texture = TextureManager::getInstance().getTexture(path);
    // Subclass override để chỉnh frameW/frameH
    int frameW = texture.getSize().x / 4;
    int frameH = texture.getSize().y;
    mAnim = std::make_unique<Animation>(mSprite, texture,
        frameW, frameH, 4, 2, 0.2f);
    mSprite.setScale(3.f, 3.f);
    mSprite.setPosition(x, y);
    return true;
}

void CTRAFFICLIGHT::update(float dt) {
    if (mAnim) mAnim->update(dt);

    if (!mIsRed) {
        mGreenTimer += dt;
        if (mGreenTimer >= Green_Time) {
            mIsRed = true;
            mGreenTimer = 0.f;
            for (auto& v : mVehicles) v->stop();
        }
    } else {
        mRedTimer += dt;
        if (mRedTimer >= Red_Time) {
            mIsRed = false;
            mRedTimer = 0.f;
            for (auto& v : mVehicles) v->resume();
        }
    }
}

void CTRAFFICLIGHT::Draw(sf::RenderWindow& w) {
    w.draw(mSprite);
}
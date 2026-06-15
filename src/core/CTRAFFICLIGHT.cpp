#include "CTRAFFICLIGHT.h"
#include "CVEHICLE.h"

CTRAFFICLIGHT::CTRAFFICLIGHT(std::vector<CVEHICLE*>& vehicles)
    : mVehicles(vehicles) {}

CTRAFFICLIGHT::~CTRAFFICLIGHT() {
    delete mAnim;
}

bool CTRAFFICLIGHT::loadSprite(const std::string& path, float x, float y) {
    if (!mTexture.loadFromFile(path)) {
        printf("FAILED: %s\n", path.c_str());
        return false;
    }
    // Subclass override để chỉnh frameW/frameH
    int frameW = mTexture.getSize().x / 4;
    int frameH = mTexture.getSize().y;
    delete mAnim;
    mAnim = new Animation(mSprite, mTexture,
        frameW, frameH, 4, 1, 0.2f);
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
            for (auto* v : mVehicles) v->stop();
        }
    } else {
        mRedTimer += dt;
        if (mRedTimer >= Red_Time) {
            mIsRed = false;
            mRedTimer = 0.f;
            for (auto* v : mVehicles) v->resume();
        }
    }
}

void CTRAFFICLIGHT::Draw(sf::RenderWindow& w) {
    w.draw(mSprite);
}
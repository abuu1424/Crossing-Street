#include "CTRUCK.h"

CTRUCK::CTRUCK(float speed, float direction)
    : CVEHICLE(speed, direction) {}

bool CTRUCK::loadSprite(const std::string& path, float x, float y) {
    if (!mTexture.loadFromFile(path)) {
        printf("FAILED: %s\n", path.c_str());
        return false;
    }

    delete mAnim;
    mAnim = new Animation(mSprite, mTexture,
        32, 32,   // frameW, frameH
        2, 2,
        Frame_Time
    );
    mSprite.setScale(4.f, 3.f);
    mSprite.setPosition(x, y);
    return true;
}

void CTRUCK::update(float dt) {
    if (mAnim && !mIsStopped)
        mAnim->update(dt);
}
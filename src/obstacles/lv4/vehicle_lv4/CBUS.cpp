#include "CBUS.h"

CBUS::CBUS(float speed, float direction)
    : CVEHICLE(speed, direction) {}

bool CBUS::loadSprite(const std::string& path, float x, float y) {
    if (!mTexture.loadFromFile(path)) {
        printf("FAILED: %s\n", path.c_str());
        return false;
    }

    delete mAnim;
    mAnim = new Animation(mSprite, mTexture,
        64, 64,   // frameW, frameH
        2, 2,
        Frame_Time
    );
    mSprite.setScale(4.f, 3.f);
    mSprite.setPosition(x, y);
    return true;
}

void CBUS::update(float dt) {
    if (mAnim && !mIsStopped)
        mAnim->update(dt);
}
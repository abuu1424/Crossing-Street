#include "CMAMMOTH.h"

#include "CMAMMOTH.h"

CMAMMOTH::CMAMMOTH(float speed, float direction)
    : CVEHICLE(speed, direction) {}

bool CMAMMOTH::loadSprite(const std::string& path, float x, float y) {
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
    mSprite.setScale(1.5f, 1.5f);
    mSprite.setPosition(x, y);
    return true;
}

void CMAMMOTH::update(float dt) {
    if (mAnim && !mIsStopped)
        mAnim->update(dt);
}
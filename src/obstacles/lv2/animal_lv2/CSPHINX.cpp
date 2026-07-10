#include "CSPHINX.h"

CSPHINX::CSPHINX(float speed, float direction)
    : CVEHICLE(speed, direction) {
}

bool CSPHINX::loadSprite(const std::string& path, float x, float y) {
    if (!mTexture.loadFromFile(path)) {
        printf("FAILED: %s\n", path.c_str());
        return false;
    }

    delete mAnim;
    mAnim = new Animation(mSprite, mTexture,
        64, 64,   // frameW, frameH
        4, 2,
        Frame_Time
    );
    mSprite.setScale(2.5f, 2.5f);
    mSprite.setPosition(x, y);
    return true;
}

void CSPHINX::update(float dt) {
    if (mAnim && !mIsStopped)
        mAnim->update(dt);
}
#include "CDINOSOUR.h"

CDINOSOUR::CDINOSOUR(float speed, float direction)
    : CVEHICLE(speed, direction) {}

bool CDINOSOUR::loadSprite(const std::string& path, float x, float y) {
    if (!mTexture.loadFromFile(path)) {
        printf("FAILED: %s\n", path.c_str());
        return false;
    }

    // 2 cột x 2 hàng, mỗi frame 16x16
    delete mAnim;
    mAnim = new Animation(mSprite, mTexture,
        32, 32,   // frameW, frameH
        2, 2,
        Frame_Time
    );
    // Scale lên 96x64 trên màn hình
    mSprite.setScale(3.f, 3.f);
    mSprite.setPosition(x, y);
    return true;
}

void CDINOSOUR::update(float dt) {
    if (mAnim && !mIsStopped)
        mAnim->update(dt);
}
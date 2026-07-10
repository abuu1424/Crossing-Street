#include "CHORUS.h"
CHORUS::CHORUS(float speed, float direction) : CANIMAL(speed, direction) {}
CHORUS::~CHORUS() {
    delete mAnim;
}
bool CHORUS::loadSprite(const std::string& path, float x, float y) {
    if (!mTexture.loadFromFile(path)) {
        printf("FAILED : %s\n", path.c_str());
        return false;
    }
    delete mAnim;
    mAnim = new Animation(mSprite, mTexture, 64, 64, 2, 2, Frame_Time);
    mSprite.setScale(1.f, 1.f);
    mSprite.setPosition(x, y);
    return true;
}
void CHORUS::update(float dt) {
    if (mAnim) mAnim->update(dt);
}
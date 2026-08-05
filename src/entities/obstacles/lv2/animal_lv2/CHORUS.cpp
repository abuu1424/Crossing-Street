#include "CHORUS.h"
#include <memory>
CHORUS::CHORUS(float speed, float direction) : CANIMAL(speed, direction) {}
bool CHORUS::loadSprite(const std::string& path, float x, float y) {
    if (!mTexture.loadFromFile(path)) {
        printf("FAILED : %s\n", path.c_str());
        return false;
    }
    mAnim = std::make_unique<Animation>(mSprite, mTexture, 64, 64, 2, 2, Frame_Time);
    mSprite.setScale(1.f, 1.f);
    mSprite.setPosition(x, y);
    return true;
}
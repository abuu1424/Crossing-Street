#include "CTRAFFIC_LV1.h"
CTRAFFIC_LV1::CTRAFFIC_LV1(std::vector<CVEHICLE*>& vehicles)
    : CTRAFFICLIGHT(vehicles) {}

bool CTRAFFIC_LV1::loadSprite(const std::string& path, float x, float y) {
    if (!mTexture.loadFromFile(path)) {
        printf("FAILED: %s\n", path.c_str());
        return false;
    }
    delete mAnim;
    mAnim = new Animation(mSprite, mTexture,
        64, 64,
        4, 2,
        0.1f
    );
    mSprite.setScale(1.5f, 1.5f);
    mSprite.setPosition(x, y);
    return true;
}
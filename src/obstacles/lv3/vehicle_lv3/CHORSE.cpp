#include "CHORSE.h"
#include <memory>

CHORSE::CHORSE(float speed, float direction) : CVEHICLE(speed, direction) {}

bool CHORSE::loadSprite(const std::string &path, float x, float y) {
  if (!mTexture.loadFromFile(path)) {
    printf("FAILED: %s\n", path.c_str());
    return false;
  }

  mAnim = std::make_unique<Animation>(mSprite, mTexture, 64, 64, // frameW, frameH
                        2, 2, Frame_Time);
  mSprite.setScale(2.f, 2.f);
  mSprite.setPosition(x, y);
  return true;
}

void CHORSE::update(float dt) {
  if (mAnim && !mIsStopped)
    mAnim->update(dt);
}
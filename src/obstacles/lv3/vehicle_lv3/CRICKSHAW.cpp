#include "CRICKSHAW.h"
#include <memory>

CRICKSHAW::CRICKSHAW(float speed, float direction)
    : CVEHICLE(speed, direction) {}

bool CRICKSHAW::loadSprite(const std::string &path, float x, float y) {
  if (!mTexture.loadFromFile(path)) {
    printf("FAILED: %s\n", path.c_str());
    return false;
  }

  mAnim = std::make_unique<Animation>(mSprite, mTexture, 64, 64, // frameW, frameH
                        4, 2, Frame_Time);
  mSprite.setScale(1.5f, 1.5f);
  mSprite.setPosition(x, y);
  return true;
}

void CRICKSHAW::update(float dt) {
  if (mAnim && !mIsStopped)
    mAnim->update(dt);
}
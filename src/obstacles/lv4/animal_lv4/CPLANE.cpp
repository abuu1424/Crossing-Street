#include "CPLANE.h"
CPLANE::CPLANE(float speed, float direction) : CANIMAL(speed, direction) {}
CPLANE::~CPLANE() { delete mAnim; }

bool CPLANE::loadSprite(const std::string &path, float x, float y) {
  if (!mTexture.loadFromFile(path)) {
    printf("FAILED : %s\n", path.c_str());
    return false;
  }
  delete mAnim;
  mAnim = new Animation(mSprite, mTexture, 64, 64, 2, 2, Frame_Time);
  bool hasDirectionSuffix = (path.find("_phai") != std::string::npos ||
                             path.find("_trai") != std::string::npos);
  if (hasDirectionSuffix) {
    mSprite.setScale(1.5f, 1.5f);
    mSprite.setOrigin(0.f, 0.f);
  } else if (mDirection > 0.f) {
    mSprite.setScale(-1.5f, 1.5f);
    mSprite.setOrigin(64.f, 0.f);
  } else {
    mSprite.setScale(1.5f, 1.5f);
    mSprite.setOrigin(0.f, 0.f);
  }
  mSprite.setPosition(x, y);
  return true;
}

void CPLANE::update(float dt) {
  if (mAnim)
    mAnim->update(dt);
}
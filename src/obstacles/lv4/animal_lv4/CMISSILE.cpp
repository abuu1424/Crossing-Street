#include "CMISSILE.h"
CMISSILE::CMISSILE(float speed, float direction) : CANIMAL(speed, direction) {}
CMISSILE::~CMISSILE() { delete mAnim; }

bool CMISSILE::loadSprite(const std::string &path, float x, float y) {
  if (!mTexture.loadFromFile(path)) {
    printf("FAILED : %s\n", path.c_str());
    return false;
  }
  delete mAnim;
  mAnim = new Animation(mSprite, mTexture, 64, 64, 2, 2, Frame_Time);
  mSprite.setScale(1.5f, 1.5f);
  mSprite.setPosition(x, y);
  return true;
}

void CMISSILE::update(float dt) {
  if (mAnim)
    mAnim->update(dt);
}
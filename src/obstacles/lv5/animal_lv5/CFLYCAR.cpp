#include "CFLYCAR.h"
CFLYCAR::CFLYCAR(float speed, float direction) : CANIMAL(speed, direction) {}
CFLYCAR::~CFLYCAR() { delete mAnim; }

bool CFLYCAR::loadSprite(const std::string &path, float x, float y) {
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

void CFLYCAR::update(float dt) {
  if (mAnim)
    mAnim->update(dt);
}
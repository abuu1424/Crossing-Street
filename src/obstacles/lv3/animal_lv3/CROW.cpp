#include "CROW.h"
CROW::CROW(float speed, float direction) : CANIMAL(speed, direction) {}
CROW::~CROW() { delete mAnim; }

bool CROW::loadSprite(const std::string &path, float x, float y) {
  if (!mTexture.loadFromFile(path)) {
    printf("FAILED : %s\n", path.c_str());
    return false;
  }
  delete mAnim;
  mAnim = new Animation(mSprite, mTexture, 64, 64, 4, 2, Frame_Time);
  mSprite.setScale(1.f, 1.f);
  mSprite.setPosition(x, y);
  return true;
}
void CROW::update(float dt) {
  if (mAnim)
    mAnim->update(dt);
}
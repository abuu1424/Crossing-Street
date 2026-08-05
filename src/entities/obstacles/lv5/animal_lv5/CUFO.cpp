#include "CUFO.h"
#include <memory>
CUFO::CUFO(float speed, float direction) : CANIMAL(speed, direction) {}

bool CUFO::loadSprite(const std::string &path, float x, float y) {
  if (!mTexture.loadFromFile(path)) {
    printf("FAILED : %s\n", path.c_str());
    return false;
  }
  mAnim = std::make_unique<Animation>(mSprite, mTexture, 64, 64, 2, 2, Frame_Time);
  mSprite.setScale(1.5f, 1.5f);
  mSprite.setPosition(x, y);
  return true;
}
#include "CTRAFFIC_LV3.h"
CTRAFFIC_LV3::CTRAFFIC_LV3(std::vector<CVEHICLE *> &vehicles)
    : CTRAFFICLIGHT(vehicles) {}

bool CTRAFFIC_LV3::loadSprite(const std::string &redPath,
                              const std::string &greenPath, float x, float y) {
  if (!mTextureRed.loadFromFile(redPath)) {
    printf("FAILED red: %s\n", redPath.c_str());
    return false;
  }
  if (!mTextureGreen.loadFromFile(greenPath)) {
    printf("FAILED green: %s\n", greenPath.c_str());
    return false;
  }

  // Mặc định bắt đầu bằng đèn xanh
  delete mAnim;
  mAnim = new Animation(mSprite, mTextureGreen, 128, 128, 4, 2, 0.1f);

  mSprite.setScale(1.f, 1.f);
  mSprite.setPosition(x, y);
  return true;
}
void CTRAFFIC_LV3::Draw(sf::RenderWindow &w) {
  // Đổi texture theo trạng thái
  if (mIsRed) {
    mSprite.setTexture(mTextureRed);
  } else {
    mSprite.setTexture(mTextureGreen);
  }
  w.draw(mSprite);
}
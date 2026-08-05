#include "CTRAFFIC_LV3.h"
#include "TextureManager.h"

CTRAFFIC_LV3::CTRAFFIC_LV3(std::vector<std::unique_ptr<CVEHICLE>> &vehicles)
    : CTRAFFICLIGHT(vehicles) {}

bool CTRAFFIC_LV3::loadSprite(const std::string &redPath,
                              const std::string &greenPath, float x, float y) {
  mTextureRed = &TextureManager::getInstance().getTexture(redPath);
  mTextureGreen = &TextureManager::getInstance().getTexture(greenPath);

  // Mặc định bắt đầu bằng đèn xanh
  mAnim = std::make_unique<Animation>(mSprite, *mTextureGreen, 128, 128, 4, 2, 0.1f);

  mSprite.setScale(1.f, 1.f);
  mSprite.setPosition(x, y);
  return true;
}
void CTRAFFIC_LV3::Draw(sf::RenderWindow &w) {
  // Đổi texture theo trạng thái
  if (mIsRed && mTextureRed) {
    mSprite.setTexture(*mTextureRed);
  } else if (mTextureGreen) {
    mSprite.setTexture(*mTextureGreen);
  }
  w.draw(mSprite);
}
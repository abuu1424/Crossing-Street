#include "CTRAFFIC_LV4.h"
#include "TextureManager.h"

CTRAFFIC_LV4::CTRAFFIC_LV4(std::vector<std::unique_ptr<CVEHICLE>> &vehicles)
    : CTRAFFICLIGHT(vehicles) {}

bool CTRAFFIC_LV4::loadSprite(const std::string &redPath,
                              const std::string &greenPath, float x, float y) {
    mTextureRed = &TextureManager::getInstance().getTexture(redPath);
    mTextureGreen = &TextureManager::getInstance().getTexture(greenPath);

    // Mặc định bắt đầu bằng đèn xanh
    mAnim = std::make_unique<Animation>(mSprite, *mTextureGreen, 128, 128, 4, 2, 0.1f);

    mSprite.setScale(0.8f, 0.8f);
    mSprite.setPosition(x, y);
    return true;
}
void CTRAFFIC_LV4::Draw(sf::RenderWindow &w) {
    // Đổi texture theo trạng thái
    if (mIsRed && mTextureRed) {
        mSprite.setTexture(*mTextureRed);
    } else if (mTextureGreen) {
        mSprite.setTexture(*mTextureGreen);
    }
    w.draw(mSprite);
}
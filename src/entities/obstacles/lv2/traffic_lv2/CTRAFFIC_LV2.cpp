#include "CTRAFFIC_LV2.h"
#include "TextureManager.h"

CTRAFFIC_LV2::CTRAFFIC_LV2(std::vector<std::unique_ptr<CVEHICLE>>& vehicles)
    : CTRAFFICLIGHT(vehicles) {}

bool CTRAFFIC_LV2::loadSprite(const std::string& redPath, const std::string& greenPath, float x, float y) {
    mTextureRed = &TextureManager::getInstance().getTexture(redPath);
    mTextureGreen = &TextureManager::getInstance().getTexture(greenPath);

    // Mặc định bắt đầu bằng đèn xanh
    mAnim = std::make_unique<Animation>(mSprite, *mTextureGreen,
        64, 64, 4, 2, 0.1f);

    mSprite.setScale(1.5f, 1.5f);
    mSprite.setPosition(x, y);
    return true;
}
void CTRAFFIC_LV2::Draw(sf::RenderWindow& w) {
    // Đổi texture theo trạng thái
    if (mIsRed && mTextureRed) {
        mSprite.setTexture(*mTextureRed);
    } else if (mTextureGreen) {
        mSprite.setTexture(*mTextureGreen);
    }
    w.draw(mSprite);
}
#include "CTRAFFIC_LV4.h"
CTRAFFIC_LV4::CTRAFFIC_LV4(std::vector<CVEHICLE *> &vehicles)
    : CTRAFFICLIGHT(vehicles) {}

bool CTRAFFIC_LV4::loadSprite(const std::string &redPath,
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

    mSprite.setScale(0.8f, 0.8f);
    mSprite.setPosition(x, y);
    return true;
}
void CTRAFFIC_LV4::Draw(sf::RenderWindow &w) {
    // Đổi texture theo trạng thái
    if (mIsRed) {
        mSprite.setTexture(mTextureRed);
    } else {
        mSprite.setTexture(mTextureGreen);
    }
    w.draw(mSprite);
}
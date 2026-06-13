#include "CTRAFFIC_LV1.h"
CTRAFFIC_LV1::CTRAFFIC_LV1(std::vector<CVEHICLE*>& vehicles)
    : CTRAFFICLIGHT(vehicles) {}

bool CTRAFFIC_LV1::loadSprite(const std::string& redPath, const std::string& greenPath, float x, float y) {
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
    mAnim = new Animation(mSprite, mTextureGreen,
        64, 64, 4, 2, 0.1f);

    mSprite.setScale(1.5f, 1.5f);
    mSprite.setPosition(x, y);
    return true;
}
void CTRAFFIC_LV1::Draw(sf::RenderWindow& w) {
    // Đổi texture theo trạng thái
    if (mIsRed) {
        mSprite.setTexture(mTextureRed);
    } else {
        mSprite.setTexture(mTextureGreen);
    }
    w.draw(mSprite);
}
#pragma once
#include "CTRAFFICLIGHT.h"

class CTRAFFIC_LV4 : public CTRAFFICLIGHT {
    sf::Texture mTextureRed;
    sf::Texture mTextureGreen;

public:
    CTRAFFIC_LV4(std::vector<CVEHICLE *> &vehicles);
    bool loadSprite(const std::string &redPath, const std::string &greenPath,
                    float x, float y) override;
    void Draw(sf::RenderWindow &w) override;
};
#pragma once
#include "CTRAFFICLIGHT.h"

class CTRAFFIC_LV5 : public CTRAFFICLIGHT {
    const sf::Texture* mTextureRed = nullptr;
    const sf::Texture* mTextureGreen = nullptr;

public:
    CTRAFFIC_LV5(std::vector<std::unique_ptr<CVEHICLE>> &vehicles);
    bool loadSprite(const std::string &redPath, const std::string &greenPath,
                    float x, float y) override;
    void Draw(sf::RenderWindow &w) override;
};
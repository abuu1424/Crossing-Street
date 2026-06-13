#pragma once
#include "CTRAFFICLIGHT.h"

class CTRAFFIC_LV1 : public CTRAFFICLIGHT {
    sf::Texture mTextureRed;
    sf::Texture mTextureGreen;
public:
    CTRAFFIC_LV1(std::vector<CVEHICLE*>& vehicles);
    bool loadSprite(const std::string& redPath, const std::string& greenPath, float x, float y);
    void Draw(sf::RenderWindow& w) override;
};
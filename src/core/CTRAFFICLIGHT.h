#pragma once
#include <SFML/Graphics.hpp>
#include "Animation.h"
#include "Utils.h"
#include <vector>

class CVEHICLE;

class CTRAFFICLIGHT {
protected:
    sf::Texture  mTexture;
    sf::Sprite   mSprite;
    Animation*   mAnim       = nullptr;
    float        mGreenTimer = 0.f;
    float        mRedTimer   = 0.f;
    bool         mIsRed      = false;

    std::vector<CVEHICLE*>& mVehicles;

public:
    CTRAFFICLIGHT(std::vector<CVEHICLE*>& vehicles);
    virtual ~CTRAFFICLIGHT();

    virtual bool loadSprite(const std::string& path, float x, float y);
    virtual void update(float dt);
    virtual void Draw(sf::RenderWindow& w);

    bool isRed() const { return mIsRed; }
};
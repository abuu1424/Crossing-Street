#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "Animation.h"
#include "Utils.h"

class CVEHICLE;

class CTRAFFICLIGHT {
protected:
    sf::Texture                   mTexture;
    sf::Sprite                    mSprite;
    std::unique_ptr<Animation>    mAnim;
    float                         mGreenTimer = 0.f;
    float                         mRedTimer   = 0.f;
    bool                          mIsRed      = false;

    std::vector<std::unique_ptr<CVEHICLE>>& mVehicles;

public:
    CTRAFFICLIGHT(std::vector<std::unique_ptr<CVEHICLE>>& vehicles);
    virtual ~CTRAFFICLIGHT();

    virtual bool loadSprite(const std::string& path, float x, float y);

    virtual bool loadSprite(const std::string& redPath, const std::string& greenPath,
                             float x, float y) {
        return false;
    }

    virtual void update(float dt);
    virtual void Draw(sf::RenderWindow& w);

    bool isRed() const { return mIsRed; }
};
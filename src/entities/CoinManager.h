#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Utils.h"

class SoundManager;

struct Coin {
    sf::Vector2f position;
    bool collected = false;
    float animTimer = 0.f;
    float bobbingTimer = 0.f;
    int currentFrame = 0;
};

struct FloatingCoinText {
    std::string text;
    sf::Vector2f position;
    float alpha = 255.f;
    float lifetime = 0.8f; // seconds
};

class CoinManager {
public:
    CoinManager();

    void spawnForLevel(int level);
    void update(float dt, const sf::FloatRect& playerHitbox, SoundManager* sound = nullptr);
    void draw(sf::RenderWindow& window);

private:
    sf::Texture mCoinTexture;
    sf::Sprite mCoinSprite;
    sf::CircleShape mShadowShape;
    sf::Font mFont;
    bool mTextureLoaded = false;
    bool mFontLoaded = false;

    std::vector<Coin> mCoins;
    std::vector<FloatingCoinText> mFloatingTexts;
};

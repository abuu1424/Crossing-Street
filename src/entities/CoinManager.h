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
    sf::Color color = sf::Color(255, 215, 0);
    float alpha = 255.f;
    float lifetime = 0.85f; // seconds
};

class CoinManager {
public:
    CoinManager();

    void spawnForLevel(int level);
    void update(float dt, const sf::FloatRect& p1Hitbox, SoundManager* sound = nullptr,
                bool p1Magnet = false, const sf::Vector2f& p1Pos = sf::Vector2f(0.f, 0.f),
                int p1ScoreMultiplier = 1, int p1Slot = 0,
                bool is2P = false, const sf::FloatRect& p2Hitbox = sf::FloatRect(),
                bool p2Magnet = false, const sf::Vector2f& p2Pos = sf::Vector2f(0.f, 0.f),
                int p2ScoreMultiplier = 1, int p2Slot = 0);
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

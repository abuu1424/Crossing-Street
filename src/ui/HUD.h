#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class HUD {
private:
    sf::Font    mFont;
    sf::Texture mHudTexture;
    sf::Sprite  mHudSprite;

    sf::Text mLevelText;
    sf::Text mScoreText;
    sf::Text mTimeText;

    sf::Vector2f mLevelCenter;
    sf::Vector2f mScoreCenter;
    sf::Vector2f mTimeCenter;

    bool mLoaded;
    bool mHudBarLoaded = false;
private:
    void setupText(sf::Text& text, unsigned int size, float xRatio, float yRatio, sf::Vector2f& centerOut);
    std::string formatTime(float seconds) const;

public:
    HUD();

    bool isLoaded() const;

    void update(int level, int score, float timeSeconds);
    void draw(sf::RenderWindow& window);
    void reloadHudBar(const std::string& hudPath);
    sf::FloatRect getPauseIconBounds() const;
};
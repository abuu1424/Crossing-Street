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

    bool mLoaded;

private:
    void setupText(sf::Text& text, unsigned int size, float xRatio, float yRatio);
    std::string formatTime(float seconds) const;

public:
    HUD();

    bool isLoaded() const;

    void update(int level, int score, float timeSeconds);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getPauseIconBounds() const;
};
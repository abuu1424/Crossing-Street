#include "HUD.h"
#include "Utils.h"

#include <iostream>
#include <sstream>
#include <iomanip>

HUD::HUD()
    : mLoaded(false)
{
    mLoaded = mFont.loadFromFile(Font_Path);

    if (!mLoaded) {
        std::cerr << "Cannot load HUD font from: " << Font_Path << std::endl;
        return;
    }

    mBar.setSize(sf::Vector2f(static_cast<float>(Win_W), 52.f));
    mBar.setPosition(0.f, 0.f);
    mBar.setFillColor(sf::Color(0, 0, 0, 160));

    setupText(mLevelText, 24, 24.f, 12.f);
    setupText(mScoreText, 24, 260.f, 12.f);
    setupText(mTimeText, 24, 520.f, 12.f);

    update(1, 0, 0.f);
}

void HUD::setupText(sf::Text& text, unsigned int size, float x, float y)
{
    text.setFont(mFont);
    text.setCharacterSize(size);
    text.setFillColor(sf::Color::White);
    text.setPosition(x, y);
}

std::string HUD::formatTime(float seconds) const
{
    if (seconds < 0.f) {
        seconds = 0.f;
    }

    int totalSeconds = static_cast<int>(seconds);
    int minutes = totalSeconds / 60;
    int remainSeconds = totalSeconds % 60;

    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << minutes
        << ":"
        << std::setw(2) << std::setfill('0') << remainSeconds;

    return oss.str();
}

bool HUD::isLoaded() const
{
    return mLoaded;
}

void HUD::update(int level, int score, float timeSeconds)
{
    if (!mLoaded) {
        return;
    }

    mLevelText.setString("Level: " + std::to_string(level));
    mScoreText.setString("Score: " + std::to_string(score));
    mTimeText.setString("Time: " + formatTime(timeSeconds));
}

void HUD::draw(sf::RenderWindow& window) {
    // Luôn vẽ thanh nền để biết HUD có được gọi draw hay không
    window.draw(mBar);
   

    if (!mLoaded) {
        return;
    }

    window.draw(mLevelText);
    window.draw(mScoreText);
    window.draw(mTimeText);
}
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

    if (!mBarTexture.loadFromFile("assets/ui/hud/hud_bg.png")) {
        std::cerr << "Cannot load HUD background!" << std::endl;
    }
    mBarSprite.setTexture(mBarTexture);
    
    // Cắt bỏ viền trong suốt, lấy đúng thanh bar
    mBarSprite.setTextureRect(sf::IntRect(0, 412, 1024, 200));
    mBarSprite.setPosition(0.f, 0.f);
    
    // Chỉnh lại kích thước hiển thị
    float targetHeight = 80.f; // Độ dày mới của HUD
    float scaleX = static_cast<float>(Win_W) / 1024.f;
    float scaleY = targetHeight / 200.f;
    mBarSprite.setScale(scaleX, scaleY);

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
    if (!mLoaded) return;
    float remaining = Level_Time_Limit - timeSeconds;
    if (remaining < 0.f) remaining = 0.f;
    mLevelText.setString("Level: " + std::to_string(level));
    mScoreText.setString("Score: " + std::to_string(score));
    mTimeText.setString("Time: " + formatTime(remaining));
    if (remaining <= 10.f)
        mTimeText.setFillColor(sf::Color::Red);
    else
        mTimeText.setFillColor(sf::Color::White);
}

void HUD::draw(sf::RenderWindow& window) {
    // Luôn vẽ thanh nền để biết HUD có được gọi draw hay không
    window.draw(mBarSprite);


    if (!mLoaded) {
        return;
    }

    window.draw(mLevelText);
    window.draw(mScoreText);
    window.draw(mTimeText);
}
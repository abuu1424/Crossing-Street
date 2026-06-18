#include "HUD.h"
#include "Utils.h"
#include <iostream>
#include <sstream>
#include <iomanip>

HUD::HUD() : mLoaded(false) {
    mLoaded = mFont.loadFromFile(Font_Path);
    if (!mLoaded) {
        std::cerr << "Cannot load HUD font\n";
        return;
    }

    if (!mHudTexture.loadFromFile("assets/ui/hud/hud_bar.png")) {
        std::cerr << "Cannot load HUD image\n";
    }
    mHudSprite.setTexture(mHudTexture);
    float hudHeight = 300.f;
    float scale = hudHeight / mHudTexture.getSize().y;
    mHudSprite.setScale(scale, scale);

    float scaledWidth = mHudTexture.getSize().x * scale;
    mHudSprite.setPosition(Win_W / 2.f - scaledWidth / 2.f, -110.f);

    setupText(mLevelText, 26, 275.f/1536.f, 495.f/1024.f, mLevelCenter);
    setupText(mScoreText, 26, 800.f/1536.f, 495.f/1024.f, mScoreCenter);
    setupText(mTimeText,  28, 1200.f/1536.f, 495.f/1024.f, mTimeCenter);

    update(1, 0, 0.f);
}

void HUD::setupText(sf::Text& text, unsigned int size, float xRatio, float yRatio, sf::Vector2f& centerOut) {
    text.setFont(mFont);
    text.setCharacterSize(size);
    text.setFillColor(sf::Color(255, 240, 200));

    sf::FloatRect spriteBounds = mHudSprite.getGlobalBounds();
    float x = spriteBounds.left + spriteBounds.width  * xRatio;
    float y = spriteBounds.top  + spriteBounds.height * yRatio;

    centerOut = sf::Vector2f(x, y);   // lưu tâm cố định
    text.setPosition(x, y);
}

std::string HUD::formatTime(float seconds) const {
    if (seconds < 0.f) seconds = 0.f;
    int total = static_cast<int>(seconds);
    int m = total / 60, s = total % 60;
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << m << ":"
        << std::setw(2) << std::setfill('0') << s;
    return oss.str();
}

bool HUD::isLoaded() const { return mLoaded; }

void HUD::update(int level, int score, float timeSeconds) {
    if (!mLoaded) return;

    mLevelText.setString("Level " + std::to_string(level));
    mScoreText.setString("Score " + std::to_string(score));

    float remaining = Level_Time_Limit - timeSeconds;
    if (remaining < 0.f) remaining = 0.f;
    mTimeText.setString(formatTime(remaining));
    mTimeText.setFillColor(remaining <= 10.f ? sf::Color::Red : sf::Color(255, 240, 200));

    auto centerOn = [](sf::Text& t, sf::Vector2f center) {
        sf::FloatRect b = t.getLocalBounds();
        t.setOrigin(b.left + b.width/2.f, b.top + b.height/2.f);
        t.setPosition(center);
    };

    centerOn(mLevelText, mLevelCenter);
    centerOn(mScoreText, mScoreCenter);
    centerOn(mTimeText,  mTimeCenter);
}

void HUD::draw(sf::RenderWindow& window) {
    window.draw(mHudSprite);

    if (!mLoaded) return;

    window.draw(mLevelText);
    window.draw(mScoreText);
    window.draw(mTimeText);
}

sf::FloatRect HUD::getPauseIconBounds() const {
    return sf::FloatRect(
        780.f,  // x
        20.f,   // y
        90.f,   // width
        80.f    // height
    );
}
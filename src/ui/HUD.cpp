#include "HUD.h"
#include "Utils.h"
#include "ShopData.h"
#include <iostream>
#include <sstream>
#include <iomanip>

HUD::HUD() : mLoaded(false), mHudBarLoaded(false) {
    mLoaded = mFont.loadFromFile(Font_Path);
    if (!mLoaded) {
        std::cerr << "Cannot load HUD font\n";
        return;
    }
    setupItemPanel();
    update(1, 0, 0.f);
}

void HUD::setupText(sf::Text& text, unsigned int size, float xRatio, float yRatio, sf::Vector2f& centerOut) {
    text.setFont(mFont);
    text.setCharacterSize(size);
    text.setFillColor(sf::Color(255, 240, 200));
    text.setOutlineColor(sf::Color(20, 15, 10, 230));
    text.setOutlineThickness(1.5f);

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

void HUD::setupItemPanel() {
    std::string iconPaths[4] = {
        "assets/shop/item_shield.png",
        "assets/shop/item_speed.png",
        "assets/shop/item_time.png",
        "assets/shop/item_radar.png"
    };

    float startX = 12.f;  // Sát góc trên bên trái
    float startY = 8.f;
    float gapX   = 70.f;  // Rộng rãi dễ nhìn

    for (int i = 0; i < 4; i++) {
        if (mItemTextures[i].loadFromFile(iconPaths[i])) {
            mItemSprites[i].setTexture(mItemTextures[i]);
            mItemSprites[i].setScale(0.68f, 0.68f); // Icon to rõ ~44x44px
            mItemSprites[i].setPosition(startX + i * gapX, startY);
        }

        mItemCountTexts[i].setFont(mFont);
        mItemCountTexts[i].setCharacterSize(16);
        mItemCountTexts[i].setFillColor(sf::Color::White);
        mItemCountTexts[i].setOutlineColor(sf::Color::Black);
        mItemCountTexts[i].setOutlineThickness(2.0f);
        mItemCountTexts[i].setPosition(startX + i * gapX + 42.f, startY + 12.f);
    }
    mItemPanelLoaded = true;
}

void HUD::drawItemPanel(sf::RenderWindow& window) {
    if (!mItemPanelLoaded) {
        setupItemPanel();
    }

    std::string itemIds[4] = { "shield", "speed", "time", "radar" };

    for (int i = 0; i < 4; i++) {
        bool owned = ShopData::isItemPurchased(itemIds[i]);

        if (i == 0) {
            // Khiên: Sáng rõ khi có, Tối mờ khi không có / đã dùng
            if (owned) {
                mItemSprites[0].setColor(sf::Color(255, 255, 255, 255)); // Sáng rực
            } else {
                mItemSprites[0].setColor(sf::Color(80, 80, 90, 90));     // Tối mờ
            }
            window.draw(mItemSprites[0]);
        } else {
            // Các item khác: Hiện số lượng (x1 khi đã mua, x0 khi chưa mua)
            if (owned) {
                mItemSprites[i].setColor(sf::Color(255, 255, 255, 255));
                mItemCountTexts[i].setString("x1");
                mItemCountTexts[i].setFillColor(sf::Color(120, 255, 120));
            } else {
                mItemSprites[i].setColor(sf::Color(140, 140, 150, 120));
                mItemCountTexts[i].setString("x0");
                mItemCountTexts[i].setFillColor(sf::Color(220, 220, 220));
            }
            window.draw(mItemSprites[i]);
            window.draw(mItemCountTexts[i]);
        }
    }
}

void HUD::draw(sf::RenderWindow& window) {
    drawItemPanel(window);

    if (!mHudBarLoaded) return;
    window.draw(mHudSprite);

    if (!mLoaded) return;

    window.draw(mLevelText);
    window.draw(mScoreText);
    window.draw(mTimeText);
}

void HUD::drawStats(sf::RenderWindow& window, const PlayerStats& stats) {
    // 1. HP Hearts (Top-Right)
    float startX = Win_W - 240.f;
    float startY = 14.f;
    float heartSize = 12.f;
    float gap = 28.f;

    for (int i = 0; i < stats.maxHp; ++i) {
        float cx = startX + i * gap;
        float cy = startY + 6.f;

        bool hasHp = (i < stats.currentHp);

        sf::Color fillCol = hasHp ? sf::Color(245, 45, 75, 240) : sf::Color(55, 35, 45, 140);
        sf::Color outlineCol = hasHp ? sf::Color(255, 210, 225, 255) : sf::Color(90, 75, 85, 160);

        sf::CircleShape leftLobe(heartSize * 0.55f);
        leftLobe.setOrigin(heartSize * 0.55f, heartSize * 0.55f);
        leftLobe.setPosition(cx - heartSize * 0.4f, cy - heartSize * 0.2f);
        leftLobe.setFillColor(fillCol);
        leftLobe.setOutlineColor(outlineCol);
        leftLobe.setOutlineThickness(1.f);
        window.draw(leftLobe);

        sf::CircleShape rightLobe(heartSize * 0.55f);
        rightLobe.setOrigin(heartSize * 0.55f, heartSize * 0.55f);
        rightLobe.setPosition(cx + heartSize * 0.4f, cy - heartSize * 0.2f);
        rightLobe.setFillColor(fillCol);
        rightLobe.setOutlineColor(outlineCol);
        rightLobe.setOutlineThickness(1.f);
        window.draw(rightLobe);

        sf::ConvexShape bottomPoint;
        bottomPoint.setPointCount(3);
        bottomPoint.setPoint(0, sf::Vector2f(cx - heartSize * 0.9f, cy - heartSize * 0.1f));
        bottomPoint.setPoint(1, sf::Vector2f(cx + heartSize * 0.9f, cy - heartSize * 0.1f));
        bottomPoint.setPoint(2, sf::Vector2f(cx, cy + heartSize * 0.9f));
        bottomPoint.setFillColor(fillCol);
        bottomPoint.setOutlineColor(outlineCol);
        bottomPoint.setOutlineThickness(1.f);
        window.draw(bottomPoint);
    }

    // 2. Energy Bar (Top-Right under Hearts)
    float barX = startX - 8.f;
    float barY = startY + 24.f;
    float barW = 145.f;
    float barH = 10.f;

    sf::RectangleShape bgEnergy(sf::Vector2f(barW, barH));
    bgEnergy.setPosition(barX, barY);
    bgEnergy.setFillColor(sf::Color(20, 25, 35, 210));
    bgEnergy.setOutlineColor(sf::Color(0, 180, 220, 200));
    bgEnergy.setOutlineThickness(1.2f);
    window.draw(bgEnergy);

    float energyRatio = std::clamp(stats.energy / stats.maxEnergy, 0.f, 1.f);
    sf::RectangleShape fillEnergy(sf::Vector2f((barW - 2.f) * energyRatio, barH - 2.f));
    fillEnergy.setPosition(barX + 1.f, barY + 1.f);
    fillEnergy.setFillColor(energyRatio < 0.3f ? sf::Color(255, 100, 50, 240) : sf::Color(0, 220, 240, 240));
    window.draw(fillEnergy);

    if (mLoaded) {
        sf::Text nrgLabel;
        nrgLabel.setFont(mFont);
        nrgLabel.setString("ENERGY");
        nrgLabel.setCharacterSize(11);
        nrgLabel.setFillColor(sf::Color(200, 245, 255));
        nrgLabel.setPosition(barX, barY - 12.f);
        window.draw(nrgLabel);

        // 3. Realtime Speed Stat Display (px/s)
        sf::Text spdText;
        spdText.setFont(mFont);
        int spdVal = static_cast<int>(stats.currentCalculatedSpeed);
        spdText.setString("SPEED: " + std::to_string(spdVal) + " px/s");
        spdText.setCharacterSize(14);
        spdText.setPosition(barX, barY + 13.f);
        spdText.setOutlineColor(sf::Color::Black);
        spdText.setOutlineThickness(1.5f);

        if (stats.skillActive) {
            spdText.setFillColor(sf::Color(255, 230, 50)); // Bright Yellow on Skill Surge
        } else if (energyRatio < 0.4f) {
            spdText.setFillColor(sf::Color(255, 140, 60)); // Amber when Low Energy
        } else {
            spdText.setFillColor(sf::Color(120, 255, 160)); // Green normal
        }
        window.draw(spdText);
    }

    // 4. Active Skill 'E' Icon Widget (Far Right)
    float skillX = Win_W - 80.f;
    float skillY = 12.f;
    float skillW = 68.f;
    float skillH = 50.f;

    bool hasSkill = ShopData::isItemPurchased("speed_skill") || stats.hasSpeedSkill;

    sf::RectangleShape skillBox(sf::Vector2f(skillW, skillH));
    skillBox.setPosition(skillX, skillY);
    skillBox.setFillColor(sf::Color(15, 20, 30, 220));

    if (stats.skillActive) {
        skillBox.setOutlineColor(sf::Color(255, 230, 50, 240)); // Flashing Gold
        skillBox.setOutlineThickness(2.0f);
    } else if (hasSkill && stats.skillCooldownTimer <= 0.f) {
        skillBox.setOutlineColor(sf::Color(50, 240, 140, 240)); // Ready Green
        skillBox.setOutlineThickness(1.8f);
    } else {
        skillBox.setOutlineColor(sf::Color(90, 100, 110, 180)); // Cooldown / Locked
        skillBox.setOutlineThickness(1.2f);
    }
    window.draw(skillBox);

    if (mLoaded) {
        sf::Text keyText;
        keyText.setFont(mFont);
        keyText.setString("[E] SKILL");
        keyText.setCharacterSize(11);
        keyText.setFillColor(sf::Color(220, 240, 255));
        keyText.setPosition(skillX + 6.f, skillY + 4.f);
        window.draw(keyText);

        sf::Text statusText;
        statusText.setFont(mFont);
        statusText.setCharacterSize(12);

        if (stats.skillActive) {
            int sec = static_cast<int>(stats.skillTimer) + 1;
            statusText.setString(std::to_string(sec) + "s FAST");
            statusText.setFillColor(sf::Color(255, 230, 50));
        } else if (stats.skillCooldownTimer > 0.f) {
            int cd = static_cast<int>(stats.skillCooldownTimer) + 1;
            statusText.setString(std::to_string(cd) + "s CD");
            statusText.setFillColor(sf::Color(180, 180, 190));
        } else if (hasSkill) {
            statusText.setString("READY");
            statusText.setFillColor(sf::Color(50, 240, 140));
        } else {
            statusText.setString("SHOP");
            statusText.setFillColor(sf::Color(140, 140, 150));
        }

        statusText.setPosition(skillX + 6.f, skillY + 24.f);
        window.draw(statusText);
    }
}



void HUD::reloadHudBar(const std::string& hudPath) {
    if (!mHudTexture.loadFromFile(hudPath)) {
        printf("FAILED reload HUD: %s\n", hudPath.c_str());
        return;
    }
    mHudBarLoaded = true;
    mHudSprite.setTexture(mHudTexture);

    float hudHeight = 300.f;
    float scale = hudHeight / mHudTexture.getSize().y;
    mHudSprite.setScale(scale, scale);

    float scaledWidth = mHudTexture.getSize().x * scale;
    mHudSprite.setPosition(Win_W / 2.f - scaledWidth / 2.f, -110.f);

    setupText(mLevelText, 22, 275.f/1536.f, 495.f/1024.f, mLevelCenter);
    setupText(mScoreText, 22, 800.f/1536.f, 495.f/1024.f, mScoreCenter);
    setupText(mTimeText,  24, 1200.f/1536.f, 495.f/1024.f, mTimeCenter);
}

sf::FloatRect HUD::getPauseIconBounds() const {
    return sf::FloatRect(
        780.f,  // x
        20.f,   // y
        90.f,   // width
        80.f    // height
    );
}
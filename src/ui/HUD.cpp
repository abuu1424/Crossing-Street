#include "HUD.h"
#include "Utils.h"
#include "ShopData.h"
#include <iostream>
#include <sstream>
#include <iomanip>

HUD::HUD() : mLoaded(false), mHudBarLoaded(false), mHeartLoaded(false) {
    mLoaded = mFont.loadFromFile(Font_Path);
    if (!mLoaded) {
        std::cerr << "Cannot load HUD font\n";
    }

    // Load heart texture assets
    if (mHeartTexture.loadFromFile("assets/ui/hud/heart.png")) {
        mHeartSprite.setTexture(mHeartTexture);
        mHeartLoaded = true;
    }
    if (mHeartHalfTexture.loadFromFile("assets/ui/hud/heart_half.png")) {
        mHeartHalfSprite.setTexture(mHeartHalfTexture);
        mHeartHalfLoaded = true;
    }
    if (mHeartEmptyTexture.loadFromFile("assets/ui/hud/heart_empty.png")) {
        mHeartEmptySprite.setTexture(mHeartEmptyTexture);
    } else if (mHeartLoaded) {
        mHeartEmptySprite = mHeartSprite;
        mHeartEmptySprite.setColor(sf::Color(80, 80, 90, 150));
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
    char buf[16];
    snprintf(buf, sizeof(buf), "%02d:%02d", m, s);
    return std::string(buf);
}

bool HUD::isLoaded() const { return mLoaded; }

void HUD::update(int level, int score, float timeSeconds) {
    if (!mLoaded) return;

    if (level != mLastLevel) {
        mLastLevel = level;
        mLevelText.setString("Level " + std::to_string(level));
        sf::FloatRect b = mLevelText.getLocalBounds();
        mLevelText.setOrigin(b.left + b.width / 2.f, b.top + b.height / 2.f);
        mLevelText.setPosition(mLevelCenter);
    }

    if (score != mLastScore) {
        mLastScore = score;
        mScoreText.setString("Score " + std::to_string(score));
        sf::FloatRect b = mScoreText.getLocalBounds();
        mScoreText.setOrigin(b.left + b.width / 2.f, b.top + b.height / 2.f);
        mScoreText.setPosition(mScoreCenter);
    }

    float remaining = std::max(0.f, Level_Time_Limit - timeSeconds);
    int remainingSec = static_cast<int>(remaining);
    if (remainingSec != mLastRemainingSec) {
        mLastRemainingSec = remainingSec;
        mTimeText.setString(formatTime(remaining));
        mTimeText.setFillColor(remaining <= 10.f ? sf::Color::Red : sf::Color(255, 240, 200));
        sf::FloatRect b = mTimeText.getLocalBounds();
        mTimeText.setOrigin(b.left + b.width / 2.f, b.top + b.height / 2.f);
        mTimeText.setPosition(mTimeCenter);
    }
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
    float gapX   = 72.f;  // Rộng rãi dễ nhìn
    float targetSize = 40.f; // Bounding box chuẩn 40x40px cho tất cả icon

    for (int i = 0; i < 4; i++) {
        if (mItemTextures[i].loadFromFile(iconPaths[i])) {
            mItemSprites[i].setTexture(mItemTextures[i]);
            
            sf::Vector2u texSize = mItemTextures[i].getSize();
            if (texSize.x > 0 && texSize.y > 0) {
                float scaleX = targetSize / static_cast<float>(texSize.x);
                float scaleY = targetSize / static_cast<float>(texSize.y);
                float scale = std::min(scaleX, scaleY);
                mItemSprites[i].setScale(scale, scale);

                // Căn giữa icon trong hộp 40x40px
                float renderW = texSize.x * scale;
                float renderH = texSize.y * scale;
                float offsetX = (targetSize - renderW) / 2.f;
                float offsetY = (targetSize - renderH) / 2.f;
                mItemSprites[i].setPosition(startX + i * gapX + offsetX, startY + offsetY);
            } else {
                mItemSprites[i].setPosition(startX + i * gapX, startY);
            }
        }

        mItemCountTexts[i].setFont(mFont);
        mItemCountTexts[i].setCharacterSize(15);
        mItemCountTexts[i].setFillColor(sf::Color::White);
        mItemCountTexts[i].setOutlineColor(sf::Color::Black);
        mItemCountTexts[i].setOutlineThickness(2.0f);
        mItemCountTexts[i].setPosition(startX + i * gapX + 44.f, startY + 11.f);
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
    float topY = 10.f;
    float hpSectionX = 900.f; // Safely right of central HUD bar (Pause frame ends ~870px)

    // 1. HP Hearts (Top-Right, aligned after central HUD bar)
    if (mLoaded) {
        sf::Text hpLabel;
        hpLabel.setFont(mFont);
        hpLabel.setString("HEALTH");
        hpLabel.setCharacterSize(11);
        hpLabel.setFillColor(sf::Color(255, 200, 210));
        hpLabel.setPosition(hpSectionX, topY);
        window.draw(hpLabel);
    }

    float heartStartX = hpSectionX;
    float heartStartY = topY + 15.f;
    float heartGap = 26.f;

    for (int i = 0; i < stats.maxHp; ++i) {
        float cx = heartStartX + i * heartGap;
        float cy = heartStartY;
        bool hasHp = (i < stats.currentHp);

        if (mHeartLoaded) {
            sf::Sprite* sprPtr = &mHeartEmptySprite;
            if (hasHp) {
                sprPtr = &mHeartSprite;
            }
            sf::Sprite& spr = *sprPtr;
            sf::Vector2u texSize = spr.getTexture() ? spr.getTexture()->getSize() : sf::Vector2u(52, 40);
            spr.setScale(24.f / texSize.x, 18.5f / texSize.y);
            spr.setPosition(cx, cy);
            window.draw(spr);
        } else {
            // Procedural shape fallback
            float heartSize = 12.f;
            float centerOffset = cx + 12.f;
            float cyOffset = cy + 10.f;
            sf::Color fillCol = hasHp ? sf::Color(245, 45, 75, 240) : sf::Color(55, 35, 45, 140);
            sf::Color outlineCol = hasHp ? sf::Color(255, 210, 225, 255) : sf::Color(90, 75, 85, 160);

            sf::CircleShape leftLobe(heartSize * 0.55f);
            leftLobe.setOrigin(heartSize * 0.55f, heartSize * 0.55f);
            leftLobe.setPosition(centerOffset - heartSize * 0.4f, cyOffset - heartSize * 0.2f);
            leftLobe.setFillColor(fillCol);
            leftLobe.setOutlineColor(outlineCol);
            leftLobe.setOutlineThickness(1.f);
            window.draw(leftLobe);

            sf::CircleShape rightLobe(heartSize * 0.55f);
            rightLobe.setOrigin(heartSize * 0.55f, heartSize * 0.55f);
            rightLobe.setPosition(centerOffset + heartSize * 0.4f, cyOffset - heartSize * 0.2f);
            rightLobe.setFillColor(fillCol);
            rightLobe.setOutlineColor(outlineCol);
            rightLobe.setOutlineThickness(1.f);
            window.draw(rightLobe);

            sf::ConvexShape bottomPoint;
            bottomPoint.setPointCount(3);
            bottomPoint.setPoint(0, sf::Vector2f(centerOffset - heartSize * 0.9f, cyOffset - heartSize * 0.1f));
            bottomPoint.setPoint(1, sf::Vector2f(centerOffset + heartSize * 0.9f, cyOffset - heartSize * 0.1f));
            bottomPoint.setPoint(2, sf::Vector2f(centerOffset, cyOffset + heartSize * 0.9f));
            bottomPoint.setFillColor(fillCol);
            bottomPoint.setOutlineColor(outlineCol);
            bottomPoint.setOutlineThickness(1.f);
            window.draw(bottomPoint);
        }
    }

    // 2. Energy Bar (Side-by-side / ngang hàng với Health Bar)
    float hpWidth = stats.maxHp * heartGap;
    float barX = hpSectionX + hpWidth + 16.f;
    float barY = topY + 17.f;
    float barW = 135.f;
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
        nrgLabel.setPosition(barX, topY);
        window.draw(nrgLabel);

        // 3. Realtime Speed Stat Display (px/s)
        sf::Text spdText;
        spdText.setFont(mFont);
        int spdVal = static_cast<int>(stats.currentCalculatedSpeed);
        spdText.setString("SPEED: " + std::to_string(spdVal) + " px/s");
        spdText.setCharacterSize(13);
        spdText.setPosition(barX, barY + 13.f);
        spdText.setOutlineColor(sf::Color::Black);
        spdText.setOutlineThickness(1.5f);

        if (stats.skillActive) {
            spdText.setFillColor(sf::Color(255, 230, 50));
        } else if (energyRatio < 0.4f) {
            spdText.setFillColor(sf::Color(255, 140, 60));
        } else {
            spdText.setFillColor(sf::Color(120, 255, 160));
        }
        window.draw(spdText);
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
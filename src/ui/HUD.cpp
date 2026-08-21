#include "HUD.h"
#include "Utils.h"
#include "ShopData.h"
#include "LevelConfig.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>


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
    sf::FloatRect b = text.getLocalBounds();
    text.setOrigin(b.left + b.width / 2.f, b.top + b.height / 2.f);
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

    if (mIsEndless) {
        mLevelText.setString("Wave " + std::to_string(mEndlessWave));
        sf::FloatRect b = mLevelText.getLocalBounds();
        mLevelText.setOrigin(b.left + b.width / 2.f, b.top + b.height / 2.f);
        mLevelText.setPosition(mLevelCenter);
    } else if (level != mLastLevel) {
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

    float baseLevelTime = getLevel(level).timeLimit;
    float effectiveTimeLimit = baseLevelTime + 8.f * ShopData::getItemCount("time");
    float remaining = std::max(0.f, effectiveTimeLimit - timeSeconds);
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
        int count = ShopData::getItemCount(itemIds[i]);

        if (count > 0) {
            mItemSprites[i].setColor(sf::Color(255, 255, 255, 255));
            mItemCountTexts[i].setString("x" + std::to_string(count));
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

void HUD::draw(sf::RenderWindow& window) {
    if (!mIsTwoPlayer) {
        drawItemPanel(window);
    }

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

    float xLevel = 275.f;
    float xScore = 778.f;
    float xTime  = 1200.f;
    float yCenter = 495.f;

    if (hudPath.find("tiensu") != std::string::npos) {
        xLevel = 263.f; xScore = 780.f; xTime = 1218.f; yCenter = 460.f;
    } else if (hudPath.find("codai") != std::string::npos) {
        xLevel = 277.f; xScore = 780.f; xTime = 1198.f; yCenter = 490.f;
    } else if (hudPath.find("trungco") != std::string::npos) {
        xLevel = 275.f; xScore = 778.f; xTime = 1200.f; yCenter = 495.f;
    } else if (hudPath.find("hiendai") != std::string::npos) {
        xLevel = 272.f; xScore = 778.f; xTime = 1205.f; yCenter = 521.f;
    } else if (hudPath.find("tuonglai") != std::string::npos) {
        xLevel = 263.f; xScore = 774.f; xTime = 1213.f; yCenter = 511.f;
    }

    setupText(mLevelText, 22, xLevel / 1536.f, yCenter / 1024.f, mLevelCenter);
    setupText(mScoreText, 22, xScore / 1536.f, yCenter / 1024.f, mScoreCenter);
    setupText(mTimeText,  24, xTime  / 1536.f, yCenter / 1024.f, mTimeCenter);

    // Re-center text immediately using current text strings
    sf::FloatRect lb = mLevelText.getLocalBounds();
    mLevelText.setOrigin(lb.left + lb.width / 2.f, lb.top + lb.height / 2.f);
    mLevelText.setPosition(mLevelCenter);

    sf::FloatRect sb = mScoreText.getLocalBounds();
    mScoreText.setOrigin(sb.left + sb.width / 2.f, sb.top + sb.height / 2.f);
    mScoreText.setPosition(mScoreCenter);

    sf::FloatRect tb = mTimeText.getLocalBounds();
    mTimeText.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
    mTimeText.setPosition(mTimeCenter);

    mLastLevel = -1;
    mLastScore = -1;
    mLastRemainingSec = -1;
}

sf::FloatRect HUD::getPauseIconBounds() const {
    sf::FloatRect hudBounds = mHudSprite.getGlobalBounds();
    float x = hudBounds.left + hudBounds.width * (1390.f / 1536.f);
    float y = hudBounds.top + hudBounds.height * (420.f / 1024.f);
    float w = hudBounds.width * (90.f / 1536.f);
    float h = hudBounds.height * (90.f / 1024.f);
    return sf::FloatRect(x, y, w, h);
}

void HUD::drawPowerUpBuffs(sf::RenderWindow& window, float magnetRem, float timeStopRem,
                          float speedRem, float scoreX2Rem, bool hasShield) {
    if (!mLoaded) return;

    struct ActiveBuff {
        std::string label;
        float remaining;
        float maxDuration;
        sf::Color color;
    };

    std::vector<ActiveBuff> buffs;
    if (magnetRem > 0.f) buffs.push_back({ "COIN MAGNET", magnetRem, 8.0f, sf::Color(0, 240, 255) });
    if (timeStopRem > 0.f) buffs.push_back({ "TIME STOP", timeStopRem, 4.0f, sf::Color(140, 180, 255) });
    if (speedRem > 0.f) buffs.push_back({ "SPEED SURGE", speedRem, 6.0f, sf::Color(255, 110, 40) });
    if (scoreX2Rem > 0.f) buffs.push_back({ "COIN 2X", scoreX2Rem, 10.0f, sf::Color(255, 215, 0) });
    int shieldCount = ShopData::getItemCount("shield");
    if (hasShield || shieldCount > 0) {
        std::string sLabel = shieldCount > 1 ? ("SHIELD x" + std::to_string(shieldCount)) : "SHIELD ACTIVE";
        buffs.push_back({ sLabel, 1.0f, 1.0f, sf::Color(255, 230, 80) });
    }

    if (buffs.empty()) return;

    float startX = 18.f;
    float startY = 120.f;
    float cardW = 125.f;
    float cardH = 24.f;
    float gap = 6.f;

    for (size_t i = 0; i < buffs.size(); ++i) {
        const auto& b = buffs[i];
        float y = startY + i * (cardH + gap);

        // Container Pill
        sf::RectangleShape box(sf::Vector2f(cardW, cardH));
        box.setPosition(startX, y);
        box.setFillColor(sf::Color(15, 18, 28, 220));
        box.setOutlineColor(b.color);
        box.setOutlineThickness(1.2f);
        window.draw(box);

        // Progress Bar
        float ratio = std::clamp(b.remaining / b.maxDuration, 0.f, 1.f);
        sf::RectangleShape progress(sf::Vector2f((cardW - 4.f) * ratio, cardH - 4.f));
        progress.setPosition(startX + 2.f, y + 2.f);
        progress.setFillColor(sf::Color(b.color.r, b.color.g, b.color.b, 65));
        window.draw(progress);

        // Text
        sf::Text txt;
        txt.setFont(mFont);
        txt.setCharacterSize(12);
        txt.setStyle(sf::Text::Bold);
        txt.setFillColor(b.color);

        if (b.label == "SHIELD") {
            txt.setString("SHIELD ACTIVE");
        } else {
            char buf[32];
            snprintf(buf, sizeof(buf), "%s %.1fs", b.label.c_str(), b.remaining);
            txt.setString(buf);
        }

        txt.setPosition(startX + 8.f, y + 3.f);
        window.draw(txt);
    }
}

void HUD::drawTwoPlayerStats(sf::RenderWindow& window, const PlayerStats& p1Stats, const PlayerStats& p2Stats, int p1Score, int p2Score) {
    if (!mLoaded) return;

    float cardW = 310.f;
    float cardH = 84.f;
    float topY  = 8.f;
    float heartGap = 24.f;
    float barW = 125.f;
    float barH = 9.f;
    std::string itemIds[4] = { "shield", "speed", "time", "radar" };

    // ==========================================
    // 1. PLAYER 1 PANEL (Top-Left, Cyan Theme)
    // ==========================================
    float p1X = 12.f;
    sf::RectangleShape p1Bg(sf::Vector2f(cardW, cardH));
    p1Bg.setPosition(p1X, topY);
    p1Bg.setFillColor(sf::Color(10, 16, 28, 225));
    p1Bg.setOutlineColor(sf::Color(0, 200, 255, 210));
    p1Bg.setOutlineThickness(1.5f);
    window.draw(p1Bg);

    // Row 1: Title & Speed
    sf::Text p1Title;
    p1Title.setFont(mFont);
    p1Title.setString("PLAYER 1 (WASD)");
    p1Title.setCharacterSize(13);
    p1Title.setFillColor(sf::Color(70, 220, 255));
    p1Title.setOutlineColor(sf::Color::Black);
    p1Title.setOutlineThickness(1.5f);
    p1Title.setPosition(p1X + 8.f, topY + 5.f);
    window.draw(p1Title);

    sf::Text p1Spd;
    p1Spd.setFont(mFont);
    p1Spd.setString("SPD: " + std::to_string(static_cast<int>(p1Stats.currentCalculatedSpeed)) + " px/s");
    p1Spd.setCharacterSize(11);
    p1Spd.setFillColor(sf::Color(255, 235, 120));
    p1Spd.setOutlineColor(sf::Color::Black);
    p1Spd.setOutlineThickness(1.2f);
    p1Spd.setPosition(p1X + cardW - 105.f, topY + 6.f);
    window.draw(p1Spd);

    // Row 2: Hearts & Energy Bar
    for (int i = 0; i < p1Stats.maxHp; ++i) {
        float cx = p1X + 8.f + i * heartGap;
        float cy = topY + 26.f;
        bool hasHp = (i < p1Stats.currentHp);

        if (mHeartLoaded) {
            sf::Sprite& spr = hasHp ? mHeartSprite : mHeartEmptySprite;
            sf::Vector2u texSize = spr.getTexture() ? spr.getTexture()->getSize() : sf::Vector2u(52, 40);
            spr.setScale(20.f / texSize.x, 16.f / texSize.y);
            spr.setPosition(cx, cy);
            window.draw(spr);
        }
    }

    float p1BarX = p1X + p1Stats.maxHp * heartGap + 12.f;
    float p1BarY = topY + 29.f;

    sf::RectangleShape bgEnergy1(sf::Vector2f(barW, barH));
    bgEnergy1.setPosition(p1BarX, p1BarY);
    bgEnergy1.setFillColor(sf::Color(20, 25, 35, 210));
    bgEnergy1.setOutlineColor(sf::Color(0, 180, 220, 200));
    bgEnergy1.setOutlineThickness(1.f);
    window.draw(bgEnergy1);

    float nrg1 = std::clamp(p1Stats.energy / p1Stats.maxEnergy, 0.f, 1.f);
    sf::RectangleShape fillEnergy1(sf::Vector2f((barW - 2.f) * nrg1, barH - 2.f));
    fillEnergy1.setPosition(p1BarX + 1.f, p1BarY + 1.f);
    fillEnergy1.setFillColor(sf::Color(0, 230, 255, 240));
    window.draw(fillEnergy1);

    // Row 3: Gold & Mini Item Inventory Badges
    int p1Coins = ShopData::getCoins(ShopData::SLOT_P1_2P);
    sf::Text p1Gold;
    p1Gold.setFont(mFont);
    p1Gold.setString("GOLD: " + std::to_string(p1Coins) + "g");
    p1Gold.setCharacterSize(11);
    p1Gold.setFillColor(sf::Color(255, 225, 70));
    p1Gold.setOutlineColor(sf::Color::Black);
    p1Gold.setOutlineThickness(1.2f);
    p1Gold.setPosition(p1X + 8.f, topY + 56.f);
    window.draw(p1Gold);

    float itemStartX1 = p1X + 98.f;
    float itemGap = 51.f;
    for (int i = 0; i < 4; i++) {
        int count1 = ShopData::getItemCount(itemIds[i], ShopData::SLOT_P1_2P);
        float ix = itemStartX1 + i * itemGap;
        float iy = topY + 54.f;
        if (mItemTextures[i].getSize().x > 0) {
            sf::Sprite miniSpr(mItemTextures[i]);
            float sc = 18.f / std::max(mItemTextures[i].getSize().x, mItemTextures[i].getSize().y);
            miniSpr.setScale(sc, sc);
            miniSpr.setPosition(ix, iy);
            miniSpr.setColor(count1 > 0 ? sf::Color::White : sf::Color(140, 140, 150, 120));
            window.draw(miniSpr);
        }
        sf::Text countText;
        countText.setFont(mFont);
        countText.setString("x" + std::to_string(count1));
        countText.setCharacterSize(11);
        countText.setFillColor(count1 > 0 ? sf::Color(120, 255, 120) : sf::Color(180, 180, 180));
        countText.setOutlineColor(sf::Color::Black);
        countText.setOutlineThickness(1.f);
        countText.setPosition(ix + 20.f, iy);
        window.draw(countText);
    }

    // ==========================================
    // 2. PLAYER 2 PANEL (Top-Right, Magenta Theme)
    // ==========================================
    float p2X = Win_W - cardW - 12.f;
    sf::RectangleShape p2Bg(sf::Vector2f(cardW, cardH));
    p2Bg.setPosition(p2X, topY);
    p2Bg.setFillColor(sf::Color(28, 12, 28, 225));
    p2Bg.setOutlineColor(sf::Color(255, 90, 220, 210));
    p2Bg.setOutlineThickness(1.5f);
    window.draw(p2Bg);

    // Row 1: Title & Speed
    sf::Text p2Title;
    p2Title.setFont(mFont);
    p2Title.setString("PLAYER 2 (ARROWS)");
    p2Title.setCharacterSize(13);
    p2Title.setFillColor(sf::Color(255, 110, 255));
    p2Title.setOutlineColor(sf::Color::Black);
    p2Title.setOutlineThickness(1.5f);
    p2Title.setPosition(p2X + 8.f, topY + 5.f);
    window.draw(p2Title);

    sf::Text p2Spd;
    p2Spd.setFont(mFont);
    p2Spd.setString("SPD: " + std::to_string(static_cast<int>(p2Stats.currentCalculatedSpeed)) + " px/s");
    p2Spd.setCharacterSize(11);
    p2Spd.setFillColor(sf::Color(255, 235, 120));
    p2Spd.setOutlineColor(sf::Color::Black);
    p2Spd.setOutlineThickness(1.2f);
    p2Spd.setPosition(p2X + cardW - 105.f, topY + 6.f);
    window.draw(p2Spd);

    // Row 2: Hearts & Energy Bar
    for (int i = 0; i < p2Stats.maxHp; ++i) {
        float cx = p2X + 8.f + i * heartGap;
        float cy = topY + 26.f;
        bool hasHp = (i < p2Stats.currentHp);

        if (mHeartLoaded) {
            sf::Sprite& spr = hasHp ? mHeartSprite : mHeartEmptySprite;
            sf::Vector2u texSize = spr.getTexture() ? spr.getTexture()->getSize() : sf::Vector2u(52, 40);
            spr.setScale(20.f / texSize.x, 16.f / texSize.y);
            spr.setPosition(cx, cy);
            window.draw(spr);
        }
    }

    float p2BarX = p2X + p2Stats.maxHp * heartGap + 12.f;
    float p2BarY = topY + 29.f;

    sf::RectangleShape bgEnergy2(sf::Vector2f(barW, barH));
    bgEnergy2.setPosition(p2BarX, p2BarY);
    bgEnergy2.setFillColor(sf::Color(35, 20, 35, 210));
    bgEnergy2.setOutlineColor(sf::Color(220, 80, 220, 200));
    bgEnergy2.setOutlineThickness(1.f);
    window.draw(bgEnergy2);

    float nrg2 = std::clamp(p2Stats.energy / p2Stats.maxEnergy, 0.f, 1.f);
    sf::RectangleShape fillEnergy2(sf::Vector2f((barW - 2.f) * nrg2, barH - 2.f));
    fillEnergy2.setPosition(p2BarX + 1.f, p2BarY + 1.f);
    fillEnergy2.setFillColor(sf::Color(255, 90, 240, 240));
    window.draw(fillEnergy2);

    // Row 3: Gold & Mini Item Inventory Badges
    int p2Coins = ShopData::getCoins(ShopData::SLOT_P2_2P);
    sf::Text p2Gold;
    p2Gold.setFont(mFont);
    p2Gold.setString("GOLD: " + std::to_string(p2Coins) + "g");
    p2Gold.setCharacterSize(11);
    p2Gold.setFillColor(sf::Color(255, 200, 255));
    p2Gold.setOutlineColor(sf::Color::Black);
    p2Gold.setOutlineThickness(1.2f);
    p2Gold.setPosition(p2X + 8.f, topY + 56.f);
    window.draw(p2Gold);

    float itemStartX2 = p2X + 98.f;
    for (int i = 0; i < 4; i++) {
        int count2 = ShopData::getItemCount(itemIds[i], ShopData::SLOT_P2_2P);
        float ix = itemStartX2 + i * itemGap;
        float iy = topY + 54.f;
        if (mItemTextures[i].getSize().x > 0) {
            sf::Sprite miniSpr(mItemTextures[i]);
            float sc = 18.f / std::max(mItemTextures[i].getSize().x, mItemTextures[i].getSize().y);
            miniSpr.setScale(sc, sc);
            miniSpr.setPosition(ix, iy);
            miniSpr.setColor(count2 > 0 ? sf::Color::White : sf::Color(140, 140, 150, 120));
            window.draw(miniSpr);
        }
        sf::Text countText;
        countText.setFont(mFont);
        countText.setString("x" + std::to_string(count2));
        countText.setCharacterSize(11);
        countText.setFillColor(count2 > 0 ? sf::Color(120, 255, 120) : sf::Color(180, 180, 180));
        countText.setOutlineColor(sf::Color::Black);
        countText.setOutlineThickness(1.f);
        countText.setPosition(ix + 20.f, iy);
        window.draw(countText);
    }

    // ==========================================
    // 3. CENTER MATCH SCORE INDICATOR
    // ==========================================
    sf::RectangleShape matchScoreBox(sf::Vector2f(230.f, 26.f));
    matchScoreBox.setOrigin(115.f, 13.f);
    matchScoreBox.setPosition(Win_W / 2.f, 75.f);
    matchScoreBox.setFillColor(sf::Color(15, 18, 28, 220));
    matchScoreBox.setOutlineColor(sf::Color(255, 215, 0, 200));
    matchScoreBox.setOutlineThickness(1.2f);
    window.draw(matchScoreBox);

    sf::Text matchScoreText;
    matchScoreText.setFont(mFont);
    matchScoreText.setString("P1 [ " + std::to_string(mP1Wins) + " ]  VS  [ " + std::to_string(mP2Wins) + " ] P2");
    matchScoreText.setCharacterSize(14);
    matchScoreText.setFillColor(sf::Color(255, 225, 60));
    matchScoreText.setOutlineColor(sf::Color::Black);
    matchScoreText.setOutlineThickness(1.5f);
    sf::FloatRect msb = matchScoreText.getLocalBounds();
    matchScoreText.setOrigin(msb.left + msb.width / 2.f, msb.top + msb.height / 2.f);
    matchScoreText.setPosition(Win_W / 2.f, 75.f);
    window.draw(matchScoreText);
}
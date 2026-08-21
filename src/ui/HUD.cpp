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
    const char* hotkeys[4] = { "[1]", "[2]", "[3]", "[4]" };

    float startX = 18.f;
    float startY = 12.f;
    float gapX   = 68.f;
    float targetSize = 34.f;

    for (int i = 0; i < 4; i++) {
        if (mItemTextures[i].loadFromFile(iconPaths[i])) {
            mItemSprites[i].setTexture(mItemTextures[i]);
            
            sf::Vector2u texSize = mItemTextures[i].getSize();
            if (texSize.x > 0 && texSize.y > 0) {
                float scaleX = targetSize / static_cast<float>(texSize.x);
                float scaleY = targetSize / static_cast<float>(texSize.y);
                float scale = std::min(scaleX, scaleY);
                mItemSprites[i].setScale(scale, scale);

                float renderW = texSize.x * scale;
                float renderH = texSize.y * scale;
                float offsetX = (targetSize - renderW) / 2.f;
                float offsetY = (targetSize - renderH) / 2.f;
                mItemSprites[i].setPosition(startX + i * gapX + offsetX, startY + offsetY);
            } else {
                mItemSprites[i].setPosition(startX + i * gapX, startY);
            }
        }

        // Hotkey badge text (e.g. [1])
        mItemHotkeyTexts[i].setFont(mFont);
        mItemHotkeyTexts[i].setString(hotkeys[i]);
        mItemHotkeyTexts[i].setCharacterSize(10);
        mItemHotkeyTexts[i].setFillColor(sf::Color(255, 215, 80));
        mItemHotkeyTexts[i].setOutlineColor(sf::Color::Black);
        mItemHotkeyTexts[i].setOutlineThickness(1.2f);
        mItemHotkeyTexts[i].setPosition(startX + i * gapX + 36.f, startY - 2.f);

        // Count text (e.g. x2)
        mItemCountTexts[i].setFont(mFont);
        mItemCountTexts[i].setCharacterSize(13);
        mItemCountTexts[i].setFillColor(sf::Color::White);
        mItemCountTexts[i].setOutlineColor(sf::Color::Black);
        mItemCountTexts[i].setOutlineThickness(1.5f);
        mItemCountTexts[i].setPosition(startX + i * gapX + 36.f, startY + 14.f);
    }
    mItemPanelLoaded = true;
}

void HUD::drawItemPanel(sf::RenderWindow& window) {
    if (!mItemPanelLoaded) {
        setupItemPanel();
    }

    // Glass panel frame
    sf::RectangleShape panelBg(sf::Vector2f(280.f, 54.f));
    panelBg.setPosition(10.f, 6.f);
    panelBg.setFillColor(sf::Color(10, 16, 26, 215));
    panelBg.setOutlineColor(sf::Color(0, 190, 240, 190));
    panelBg.setOutlineThickness(1.2f);
    window.draw(panelBg);

    std::string itemIds[4] = { "shield", "speed", "time", "radar" };

    for (int i = 0; i < 4; i++) {
        int count = ShopData::getItemCount(itemIds[i]);

        if (count > 0) {
            mItemSprites[i].setColor(sf::Color(255, 255, 255, 255));
            mItemCountTexts[i].setString("x" + std::to_string(count));
            mItemCountTexts[i].setFillColor(sf::Color(120, 255, 120));
            mItemHotkeyTexts[i].setFillColor(sf::Color(255, 225, 80));
        } else {
            mItemSprites[i].setColor(sf::Color(140, 140, 150, 100));
            mItemCountTexts[i].setString("x0");
            mItemCountTexts[i].setFillColor(sf::Color(160, 160, 170, 160));
            mItemHotkeyTexts[i].setFillColor(sf::Color(160, 160, 170, 140));
        }
        window.draw(mItemSprites[i]);
        window.draw(mItemHotkeyTexts[i]);
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

void HUD::drawStats(sf::RenderWindow& window, const PlayerStats& stats, int currentCoins) {
    if (!mLoaded) return;

    float cardW = 320.f;
    float cardH = 84.f;
    float startX = Win_W - cardW - 12.f;
    float topY = 6.f;
    float heartGap = 24.f;
    float barW = 95.f;
    float barH = 8.f;

    // Outer Glass Container Card
    sf::RectangleShape cardBg(sf::Vector2f(cardW, cardH));
    cardBg.setPosition(startX, topY);
    cardBg.setFillColor(sf::Color(10, 16, 28, 220));
    cardBg.setOutlineColor(sf::Color(0, 200, 255, 190));
    cardBg.setOutlineThickness(1.5f);
    window.draw(cardBg);

    // ==========================================
    // Row 1: HEALTH (Hearts) & GOLD Balance
    // ==========================================
    sf::Text hpLabel;
    hpLabel.setFont(mFont);
    hpLabel.setString("HEALTH");
    hpLabel.setCharacterSize(11);
    hpLabel.setFillColor(sf::Color(255, 180, 200));
    hpLabel.setOutlineColor(sf::Color::Black);
    hpLabel.setOutlineThickness(1.f);
    hpLabel.setPosition(startX + 10.f, topY + 5.f);
    window.draw(hpLabel);

    float heartStartX = startX + 10.f;
    float heartStartY = topY + 19.f;

    for (int i = 0; i < stats.maxHp; ++i) {
        float cx = heartStartX + i * heartGap;
        float cy = heartStartY;
        bool hasHp = (i < stats.currentHp);

        if (mHeartLoaded) {
            sf::Sprite& spr = hasHp ? mHeartSprite : mHeartEmptySprite;
            sf::Vector2u texSize = spr.getTexture() ? spr.getTexture()->getSize() : sf::Vector2u(52, 40);
            spr.setScale(20.f / texSize.x, 16.f / texSize.y);
            spr.setPosition(cx, cy);
            window.draw(spr);
        }
    }

    // Gold readout (Top-Right of card)
    int goldVal = (currentCoins > 0) ? currentCoins : ShopData::getCoins();
    sf::Text goldText;
    goldText.setFont(mFont);
    goldText.setString("GOLD: " + std::to_string(goldVal) + "g");
    goldText.setCharacterSize(12);
    goldText.setFillColor(sf::Color(255, 225, 60));
    goldText.setOutlineColor(sf::Color::Black);
    goldText.setOutlineThickness(1.2f);
    goldText.setPosition(startX + cardW - 105.f, topY + 5.f);
    window.draw(goldText);

    // ==========================================
    // Row 2: ENERGY & STAMINA Bars
    // ==========================================
    float barsStartX = startX + stats.maxHp * heartGap + 18.f;

    // 1. Energy Bar (Cyan)
    sf::Text nrgLabel;
    nrgLabel.setFont(mFont);
    nrgLabel.setString("ENERGY");
    nrgLabel.setCharacterSize(10);
    nrgLabel.setFillColor(sf::Color(180, 235, 255));
    nrgLabel.setPosition(barsStartX, topY + 20.f);
    window.draw(nrgLabel);

    float bar1X = barsStartX + 52.f;
    float bar1Y = topY + 22.f;
    sf::RectangleShape bgNrg(sf::Vector2f(barW, barH));
    bgNrg.setPosition(bar1X, bar1Y);
    bgNrg.setFillColor(sf::Color(15, 22, 32, 210));
    bgNrg.setOutlineColor(sf::Color(0, 180, 220, 180));
    bgNrg.setOutlineThickness(1.f);
    window.draw(bgNrg);

    float nrgRatio = std::clamp(stats.energy / stats.maxEnergy, 0.f, 1.f);
    sf::RectangleShape fillNrg(sf::Vector2f((barW - 2.f) * nrgRatio, barH - 2.f));
    fillNrg.setPosition(bar1X + 1.f, bar1Y + 1.f);
    fillNrg.setFillColor(nrgRatio < 0.3f ? sf::Color(255, 90, 50) : sf::Color(0, 225, 245));
    window.draw(fillNrg);

    // 2. Stamina Bar (Lime Green)
    sf::Text stmLabel;
    stmLabel.setFont(mFont);
    stmLabel.setString("STAMINA");
    stmLabel.setCharacterSize(10);
    stmLabel.setFillColor(sf::Color(180, 255, 180));
    stmLabel.setPosition(barsStartX, topY + 36.f);
    window.draw(stmLabel);

    float bar2X = barsStartX + 52.f;
    float bar2Y = topY + 38.f;
    sf::RectangleShape bgStm(sf::Vector2f(barW, barH));
    bgStm.setPosition(bar2X, bar2Y);
    bgStm.setFillColor(sf::Color(15, 28, 20, 210));
    bgStm.setOutlineColor(sf::Color(60, 210, 100, 180));
    bgStm.setOutlineThickness(1.f);
    window.draw(bgStm);

    float stmRatio = std::clamp(stats.stamina / stats.maxStamina, 0.f, 1.f);
    sf::RectangleShape fillStm(sf::Vector2f((barW - 2.f) * stmRatio, barH - 2.f));
    fillStm.setPosition(bar2X + 1.f, bar2Y + 1.f);
    fillStm.setFillColor(stats.isSprinting ? sf::Color(255, 230, 40) : (stmRatio < 0.25f ? sf::Color(255, 120, 50) : sf::Color(80, 240, 120)));
    window.draw(fillStm);

    // ==========================================
    // Row 3: Realtime Speed & Skill Pill
    // ==========================================
    sf::Text spdText;
    spdText.setFont(mFont);
    int spdVal = static_cast<int>(stats.currentCalculatedSpeed);
    spdText.setString("SPEED: " + std::to_string(spdVal) + " px/s");
    spdText.setCharacterSize(11);
    spdText.setPosition(startX + 10.f, topY + 60.f);
    spdText.setOutlineColor(sf::Color::Black);
    spdText.setOutlineThickness(1.2f);
    spdText.setFillColor(stats.skillActive ? sf::Color(255, 235, 60) : (stats.isSprinting ? sf::Color(120, 255, 220) : sf::Color(140, 255, 160)));
    window.draw(spdText);

    // Skill Status Pill
    sf::Text skillText;
    skillText.setFont(mFont);
    skillText.setCharacterSize(11);
    skillText.setOutlineColor(sf::Color::Black);
    skillText.setOutlineThickness(1.2f);

    if (stats.skillActive) {
        char buf[32];
        snprintf(buf, sizeof(buf), "[SPACE] BOOST: %.1fs", stats.skillTimer);
        skillText.setString(buf);
        skillText.setFillColor(sf::Color(255, 220, 50));
    } else if (stats.skillCooldownTimer > 0.f) {
        char buf[32];
        snprintf(buf, sizeof(buf), "[SPACE] CD: %.1fs", stats.skillCooldownTimer);
        skillText.setString(buf);
        skillText.setFillColor(sf::Color(255, 140, 70));
    } else {
        skillText.setString("[SPACE] BOOST READY");
        skillText.setFillColor(sf::Color(80, 240, 160));
    }
    skillText.setPosition(startX + 150.f, topY + 60.f);
    window.draw(skillText);
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

void HUD::drawVsBotStats(sf::RenderWindow& window, const PlayerStats& playerStats, const PlayerStats& botStats, BotDifficulty diff, float playerY, float botY) {
    if (!mLoaded) return;

    float cardW = 310.f;
    float cardH = 84.f;
    float topY  = 8.f;
    float heartGap = 24.f;
    float barW = 120.f;
    float barH = 9.f;

    // ==========================================
    // 1. PLAYER 1 (YOU) PANEL (Top-Left, Cyan Theme)
    // ==========================================
    float p1X = 12.f;
    sf::RectangleShape p1Bg(sf::Vector2f(cardW, cardH));
    p1Bg.setPosition(p1X, topY);
    p1Bg.setFillColor(sf::Color(10, 18, 30, 225));
    p1Bg.setOutlineColor(sf::Color(0, 210, 255, 210));
    p1Bg.setOutlineThickness(1.5f);
    window.draw(p1Bg);

    // Row 1: Title & Speed
    sf::Text p1Title;
    p1Title.setFont(mFont);
    p1Title.setString("PLAYER 1 (YOU)");
    p1Title.setCharacterSize(13);
    p1Title.setFillColor(sf::Color(60, 230, 255));
    p1Title.setOutlineColor(sf::Color::Black);
    p1Title.setOutlineThickness(1.5f);
    p1Title.setPosition(p1X + 8.f, topY + 5.f);
    window.draw(p1Title);

    sf::Text p1Spd;
    p1Spd.setFont(mFont);
    p1Spd.setString("SPD: " + std::to_string(static_cast<int>(playerStats.currentCalculatedSpeed)) + " px/s");
    p1Spd.setCharacterSize(11);
    p1Spd.setFillColor(sf::Color(255, 235, 120));
    p1Spd.setOutlineColor(sf::Color::Black);
    p1Spd.setOutlineThickness(1.2f);
    p1Spd.setPosition(p1X + cardW - 105.f, topY + 6.f);
    window.draw(p1Spd);

    // Row 2: Hearts & Energy Bar
    for (int i = 0; i < playerStats.maxHp; ++i) {
        float cx = p1X + 8.f + i * heartGap;
        float cy = topY + 26.f;
        bool hasHp = (i < playerStats.currentHp);

        if (mHeartLoaded) {
            sf::Sprite& spr = hasHp ? mHeartSprite : mHeartEmptySprite;
            sf::Vector2u texSize = spr.getTexture() ? spr.getTexture()->getSize() : sf::Vector2u(52, 40);
            spr.setScale(20.f / texSize.x, 16.f / texSize.y);
            spr.setPosition(cx, cy);
            window.draw(spr);
        }
    }

    float p1BarX = p1X + playerStats.maxHp * heartGap + 12.f;
    float p1BarY = topY + 29.f;

    sf::RectangleShape bgEnergy1(sf::Vector2f(barW, barH));
    bgEnergy1.setPosition(p1BarX, p1BarY);
    bgEnergy1.setFillColor(sf::Color(20, 25, 35, 210));
    bgEnergy1.setOutlineColor(sf::Color(0, 180, 220, 200));
    bgEnergy1.setOutlineThickness(1.f);
    window.draw(bgEnergy1);

    float nrg1 = std::clamp(playerStats.energy / playerStats.maxEnergy, 0.f, 1.f);
    sf::RectangleShape fillEnergy1(sf::Vector2f((barW - 2.f) * nrg1, barH - 2.f));
    fillEnergy1.setPosition(p1BarX + 1.f, p1BarY + 1.f);
    fillEnergy1.setFillColor(sf::Color(0, 230, 255, 240));
    window.draw(fillEnergy1);

    // Row 3: Gold & Mini Item Inventory Badges
    int p1Coins = ShopData::getCoins();
    sf::Text p1Gold;
    p1Gold.setFont(mFont);
    p1Gold.setString("GOLD: " + std::to_string(p1Coins) + "g");
    p1Gold.setCharacterSize(11);
    p1Gold.setFillColor(sf::Color(255, 225, 70));
    p1Gold.setOutlineColor(sf::Color::Black);
    p1Gold.setOutlineThickness(1.2f);
    p1Gold.setPosition(p1X + 8.f, topY + 56.f);
    window.draw(p1Gold);

    std::string itemIds[4] = { "shield", "speed", "time", "radar" };
    float itemStartX1 = p1X + 98.f;
    float itemGap = 51.f;
    for (int i = 0; i < 4; i++) {
        int count1 = ShopData::getItemCount(itemIds[i]);
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
    // 2. OPPONENT BOT AI PANEL (Top-Right, Coral / Crimson Theme)
    // ==========================================
    float botX = Win_W - cardW - 12.f;
    sf::RectangleShape botBg(sf::Vector2f(cardW, cardH));
    botBg.setPosition(botX, topY);
    botBg.setFillColor(sf::Color(28, 14, 18, 225));
    botBg.setOutlineColor(sf::Color(255, 75, 90, 210));
    botBg.setOutlineThickness(1.5f);
    window.draw(botBg);

    // Row 1: Title & Difficulty Badge
    sf::Text botTitle;
    botTitle.setFont(mFont);
    botTitle.setString("OPPONENT BOT AI");
    botTitle.setCharacterSize(13);
    botTitle.setFillColor(sf::Color(255, 110, 120));
    botTitle.setOutlineColor(sf::Color::Black);
    botTitle.setOutlineThickness(1.5f);
    botTitle.setPosition(botX + 8.f, topY + 5.f);
    window.draw(botTitle);

    sf::Text diffBadge;
    diffBadge.setFont(mFont);
    if (diff == BotDifficulty::EASY) {
        diffBadge.setString("[EASY BOT]");
        diffBadge.setFillColor(sf::Color(120, 255, 140));
    } else if (diff == BotDifficulty::NORMAL) {
        diffBadge.setString("[NORMAL BOT]");
        diffBadge.setFillColor(sf::Color(255, 220, 80));
    } else {
        diffBadge.setString("[HARD BOT]");
        diffBadge.setFillColor(sf::Color(255, 70, 70));
    }
    diffBadge.setCharacterSize(11);
    diffBadge.setOutlineColor(sf::Color::Black);
    diffBadge.setOutlineThickness(1.2f);
    diffBadge.setPosition(botX + cardW - 110.f, topY + 6.f);
    window.draw(diffBadge);

    // Row 2: Bot HP (Hearts with reddish tint) & Bot Speed
    for (int i = 0; i < botStats.maxHp; ++i) {
        float cx = botX + 8.f + i * heartGap;
        float cy = topY + 26.f;
        bool hasHp = (i < botStats.currentHp);

        if (mHeartLoaded) {
            sf::Sprite spr = hasHp ? mHeartSprite : mHeartEmptySprite;
            sf::Vector2u texSize = spr.getTexture() ? spr.getTexture()->getSize() : sf::Vector2u(52, 40);
            spr.setScale(20.f / texSize.x, 16.f / texSize.y);
            spr.setPosition(cx, cy);
            if (hasHp) spr.setColor(sf::Color(255, 130, 130));
            window.draw(spr);
        }
    }

    sf::Text botSpd;
    botSpd.setFont(mFont);
    botSpd.setString("SPD: " + std::to_string(static_cast<int>(botStats.currentCalculatedSpeed)) + " px/s");
    botSpd.setCharacterSize(11);
    botSpd.setFillColor(sf::Color(255, 200, 160));
    botSpd.setOutlineColor(sf::Color::Black);
    botSpd.setOutlineThickness(1.2f);
    botSpd.setPosition(botX + botStats.maxHp * heartGap + 16.f, topY + 28.f);
    window.draw(botSpd);

    // Row 3: Bot Status Indicator Pill
    sf::Text botStatus;
    botStatus.setFont(mFont);
    botStatus.setCharacterSize(11);
    botStatus.setOutlineColor(sf::Color::Black);
    botStatus.setOutlineThickness(1.2f);

    if (botStats.currentHp <= 0) {
        botStatus.setString("STATUS: [ELIMINATED / CRASHED]");
        botStatus.setFillColor(sf::Color(255, 60, 60));
    } else if (botStats.timeFreezeActive) {
        botStatus.setString("STATUS: [FROZEN IN TIME]");
        botStatus.setFillColor(sf::Color(140, 210, 255));
    } else {
        botStatus.setString("STATUS: [RACING / SMART DODGE]");
        botStatus.setFillColor(sf::Color(80, 240, 160));
    }
    botStatus.setPosition(botX + 8.f, topY + 56.f);
    window.draw(botStatus);

    // ==========================================
    // 3. CENTER RACE PROGRESS TRACKER
    // ==========================================
    float trackW = 280.f;
    float trackH = 26.f;
    sf::RectangleShape raceTrackBox(sf::Vector2f(trackW, trackH));
    raceTrackBox.setOrigin(trackW / 2.f, trackH / 2.f);
    raceTrackBox.setPosition(Win_W / 2.f, 75.f);
    raceTrackBox.setFillColor(sf::Color(12, 16, 26, 225));
    raceTrackBox.setOutlineColor(sf::Color(255, 215, 0, 210));
    raceTrackBox.setOutlineThickness(1.2f);
    window.draw(raceTrackBox);

    // Track baseline
    float lineStartX = Win_W / 2.f - trackW / 2.f + 45.f;
    float lineEndX   = Win_W / 2.f + trackW / 2.f - 45.f;
    float lineY      = 75.f;

    sf::RectangleShape trackLine(sf::Vector2f(lineEndX - lineStartX, 3.f));
    trackLine.setPosition(lineStartX, lineY - 1.5f);
    trackLine.setFillColor(sf::Color(70, 85, 110, 200));
    window.draw(trackLine);

    // Start / Finish Text
    sf::Text startLabel;
    startLabel.setFont(mFont);
    startLabel.setString("START");
    startLabel.setCharacterSize(9);
    startLabel.setFillColor(sf::Color(180, 190, 210));
    startLabel.setPosition(lineStartX - 38.f, lineY - 6.f);
    window.draw(startLabel);

    sf::Text finLabel;
    finLabel.setFont(mFont);
    finLabel.setString("FINISH");
    finLabel.setCharacterSize(9);
    finLabel.setFillColor(sf::Color(255, 215, 80));
    finLabel.setPosition(lineEndX + 6.f, lineY - 6.f);
    window.draw(finLabel);

    // Normalized progress: from spawn (y = 560) to finish (y = 80)
    float pProg = std::clamp((560.f - playerY) / 480.f, 0.f, 1.f);
    float bProg = std::clamp((560.f - botY) / 480.f, 0.f, 1.f);

    float pDotX = lineStartX + pProg * (lineEndX - lineStartX);
    float bDotX = lineStartX + bProg * (lineEndX - lineStartX);

    // Bot Dot Marker (Red)
    sf::CircleShape botDot(5.f);
    botDot.setOrigin(5.f, 5.f);
    botDot.setPosition(bDotX, lineY);
    botDot.setFillColor(sf::Color(255, 70, 80));
    botDot.setOutlineColor(sf::Color::White);
    botDot.setOutlineThickness(1.f);
    window.draw(botDot);

    // Player Dot Marker (Cyan)
    sf::CircleShape playerDot(6.f);
    playerDot.setOrigin(6.f, 6.f);
    playerDot.setPosition(pDotX, lineY);
    playerDot.setFillColor(sf::Color(0, 240, 255));
    playerDot.setOutlineColor(sf::Color::White);
    playerDot.setOutlineThickness(1.2f);
    window.draw(playerDot);
}
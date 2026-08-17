#include "PowerUpManager.h"
#include "SoundManager.h"
#include "ShopData.h"
#include <cmath>
#include <iostream>
#include <algorithm>

PowerUpManager::PowerUpManager() {
    mTexMagnet.loadFromFile("assets/shop/item_radar.png");
    mTexTimeStop.loadFromFile("assets/shop/item_time.png");
    mTexSpeed.loadFromFile("assets/shop/item_speed.png");
    mTexShield.loadFromFile("assets/shop/item_shield.png");
    mTexScoreX2.loadFromFile("assets/shop/coin.png");

    mGlowShape.setRadius(24.f);
    mGlowShape.setOrigin(24.f, 24.f);

    mRingShape.setRadius(28.f);
    mRingShape.setOrigin(28.f, 28.f);
    mRingShape.setFillColor(sf::Color::Transparent);
    mRingShape.setOutlineThickness(2.0f);
}

void PowerUpManager::init(const sf::Font& font) {
    mFont = font;
    mFontLoaded = true;
}

void PowerUpManager::reset() {
    mItems.clear();
    mFloatingTexts.clear();
    mMagnetTimer = 0.f;
    mTimeStopTimer = 0.f;
    mSpeedBoostTimer = 0.f;
    mScoreX2Timer = 0.f;
    mHasShield = false;
}

void PowerUpManager::spawnForLevel(int level, bool isEndless) {
    mItems.clear();

    // Các vị trí làn an toàn để spawn Power-up
    std::vector<float> safeY = { 200.f, 310.f, 420.f, 530.f };
    int spawnCount = isEndless ? (1 + rand() % 3) : (1 + rand() % 2);

    std::vector<int> usedYIndices;
    for (int i = 0; i < spawnCount && i < static_cast<int>(safeY.size()); ++i) {
        int yIdx = rand() % safeY.size();
        while (std::find(usedYIndices.begin(), usedYIndices.end(), yIdx) != usedYIndices.end()) {
            yIdx = (yIdx + 1) % safeY.size();
        }
        usedYIndices.push_back(yIdx);

        PowerUpItem item;
        item.type = static_cast<PowerUpType>(rand() % 5);
        item.position.x = 180.f + static_cast<float>(rand() % 920);
        item.position.y = safeY[yIdx];
        item.bobbingTimer = static_cast<float>(rand() % 100) / 10.f;
        item.glowTimer = 0.f;
        item.collected = false;

        mItems.push_back(item);
    }
}

void PowerUpManager::update(float dt, const sf::FloatRect& playerHitbox, SoundManager* sound) {
    // 1. Update Buff Timers
    if (mMagnetTimer > 0.f) {
        mMagnetTimer -= dt;
        if (mMagnetTimer < 0.f) mMagnetTimer = 0.f;
    }
    if (mTimeStopTimer > 0.f) {
        mTimeStopTimer -= dt;
        if (mTimeStopTimer < 0.f) mTimeStopTimer = 0.f;
    }
    if (mSpeedBoostTimer > 0.f) {
        mSpeedBoostTimer -= dt;
        if (mSpeedBoostTimer < 0.f) mSpeedBoostTimer = 0.f;
    }
    if (mScoreX2Timer > 0.f) {
        mScoreX2Timer -= dt;
        if (mScoreX2Timer < 0.f) mScoreX2Timer = 0.f;
    }

    // 2. Check Item Collisions
    for (auto& item : mItems) {
        if (item.collected) continue;

        item.bobbingTimer += dt * 4.f;
        item.glowTimer += dt * 5.f;

        sf::FloatRect itemBox(item.position.x - 22.f, item.position.y - 22.f, 44.f, 44.f);
        if (playerHitbox.intersects(itemBox)) {
            item.collected = true;
            applyBuff(item.type, item.position, sound);
        }
    }

    // 3. Update Floating Texts
    for (auto it = mFloatingTexts.begin(); it != mFloatingTexts.end();) {
        it->lifetime -= dt;
        it->position.y -= dt * 40.f;
        it->alpha = std::clamp(it->lifetime / 1.0f * 255.f, 0.f, 255.f);

        if (it->lifetime <= 0.f) {
            it = mFloatingTexts.erase(it);
        } else {
            ++it;
        }
    }
}

void PowerUpManager::applyBuff(PowerUpType type, const sf::Vector2f& pos, SoundManager* sound) {
    FloatingPowerUpText ft;
    ft.position = pos;

    if (sound) {
        sound->playCoinSound();
    }

    switch (type) {
        case PowerUpType::MAGNET:
            mMagnetTimer = getMagnetDuration();
            ft.text = "+COIN MAGNET!";
            ft.color = sf::Color(0, 240, 255); // Cyan
            break;

        case PowerUpType::TIME_STOP:
            mTimeStopTimer = getTimeStopDuration();
            ft.text = "+TIME STOP!";
            ft.color = sf::Color(140, 180, 255); // Ice Blue
            break;

        case PowerUpType::SPEED_BOOST:
            mSpeedBoostTimer = getSpeedBoostDuration();
            ft.text = "+SPEED SURGE (+50%)!";
            ft.color = sf::Color(255, 100, 50); // Orange
            break;

        case PowerUpType::SHIELD:
            // Synchronize with Shop Shield count: Add +1 shield to active slot inventory
            ShopData::buyItem("shield", 0);
            ft.text = "+1 ENERGY SHIELD!";
            ft.color = sf::Color(255, 225, 60); // Gold
            break;

        case PowerUpType::SCORE_X2:
            mScoreX2Timer = getScoreX2Duration();
            ft.text = "+2X COINS & SCORE!";
            ft.color = sf::Color(255, 215, 0); // Golden Yellow
            break;
    }

    mFloatingTexts.push_back(ft);
}

void PowerUpManager::draw(sf::RenderWindow& window) {
    // 1. Draw In-Lane PowerUp Items
    for (const auto& item : mItems) {
        if (item.collected) continue;

        float bobOffset = std::sin(item.bobbingTimer) * 5.f;
        sf::Vector2f drawPos(item.position.x, item.position.y + bobOffset);

        // Hào quang phát sáng theo loại Power-Up
        sf::Color glowCol;
        const sf::Texture* texPtr = &mTexMagnet;

        switch (item.type) {
            case PowerUpType::MAGNET:
                glowCol = sf::Color(0, 220, 255, 120);
                texPtr = &mTexMagnet;
                break;
            case PowerUpType::TIME_STOP:
                glowCol = sf::Color(120, 170, 255, 130);
                texPtr = &mTexTimeStop;
                break;
            case PowerUpType::SPEED_BOOST:
                glowCol = sf::Color(255, 120, 40, 130);
                texPtr = &mTexSpeed;
                break;
            case PowerUpType::SHIELD:
                glowCol = sf::Color(255, 220, 50, 140);
                texPtr = &mTexShield;
                break;
            case PowerUpType::SCORE_X2:
                glowCol = sf::Color(255, 200, 0, 140);
                texPtr = &mTexScoreX2;
                break;
        }

        mGlowShape.setFillColor(glowCol);
        mGlowShape.setPosition(drawPos);
        window.draw(mGlowShape);

        // Vòng tròn quay quanh
        float ringPulse = 26.f + std::sin(item.glowTimer) * 3.f;
        mRingShape.setRadius(ringPulse);
        mRingShape.setOrigin(ringPulse, ringPulse);
        mRingShape.setOutlineColor(sf::Color(glowCol.r, glowCol.g, glowCol.b, 200));
        mRingShape.setPosition(drawPos);
        window.draw(mRingShape);

        // Vẽ Sprite Icon với crop chính xác
        if (texPtr && texPtr->getSize().x > 0) {
            sf::Sprite sprite(*texPtr);
            if (item.type == PowerUpType::SCORE_X2) {
                // Crop chính xác frame 32x32 mặt đồng xu từ coin.png (128x64 sprite sheet)
                sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
                sprite.setOrigin(16.f, 16.f);
                sprite.setPosition(drawPos);
                float scale = 36.f / 32.f;
                sprite.setScale(scale, scale);
            } else {
                sf::FloatRect b = sprite.getLocalBounds();
                sprite.setOrigin(b.width / 2.f, b.height / 2.f);
                sprite.setPosition(drawPos);
                // Scale icon to fit 36x36
                float scale = 36.f / std::max(b.width, b.height);
                sprite.setScale(scale, scale);
            }
            window.draw(sprite);
        }
    }

    // 2. Draw Floating Texts
    if (mFontLoaded) {
        sf::Text text;
        text.setFont(mFont);
        text.setCharacterSize(16);
        text.setStyle(sf::Text::Bold);

        for (const auto& ft : mFloatingTexts) {
            text.setString(ft.text);
            text.setFillColor(sf::Color(ft.color.r, ft.color.g, ft.color.b, static_cast<sf::Uint8>(ft.alpha)));
            text.setOutlineColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(ft.alpha)));
            text.setOutlineThickness(2.0f);

            sf::FloatRect b = text.getLocalBounds();
            text.setOrigin(b.width / 2.f, b.height / 2.f);
            text.setPosition(ft.position);
            window.draw(text);
        }
    }
}

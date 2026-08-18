#include "CPEOPLE.h"
#include "TextureManager.h"
#include "ShopData.h"
#include <cmath>
#include <memory>
#include <algorithm>

CPEOPLE::CPEOPLE()
{
    mSpeed = Player_Speed;
    mIsDead = false;
    mIsFinish = false;

    mPosition = sf::Vector2f(
        Win_W / 2.f - Player_W / 2.f,
        Win_H / 2.f - Player_H / 2.f
    );

    mSprite.setPosition(mPosition);

    if (mFlashAuraTexture.loadFromFile("assets/shop/animation_shop/flash_speed.png")) {
        mFlashAuraSprite.setTexture(mFlashAuraTexture);
        mFlashAuraSprite.setOrigin(32.f, 32.f);
        mAuraLoaded = true;
    }
    if (mRadarWaveTexture.loadFromFile("assets/shop/animation_shop/radar_wave.png")) {
        mRadarWaveSprite.setTexture(mRadarWaveTexture);
        mRadarWaveSprite.setOrigin(64.f, 64.f);
        mRadarLoaded = true;
    }
    if (mTimeFreezeTexture.loadFromFile("assets/shop/animation_shop/time_freeze.png")) {
        mTimeFreezeSprite.setTexture(mTimeFreezeTexture);
        mTimeFreezeSprite.setOrigin(64.f, 64.f);
        mTimeFreezeLoaded = true;
    }
    mWarnFontLoaded = mWarnFont.loadFromFile(Font_Path);

    loadSprite("assets/sprites/lv1_sp/player/player.png");
}

CPEOPLE::~CPEOPLE() = default;

//Load Sprite
bool CPEOPLE::loadSprite(const std::string& texturePath) {
    mRow = 0;
    mIsMoving = false;
    const auto& texture = TextureManager::getInstance().getTexture(texturePath);
    mAnim = std::make_unique<Animation>(
        mSprite, texture,
        64, 64,   // frameW, frameH
        4, 1,     // 4 cột, 1 hàng
        Frame_Time
    );
    mSprite.setScale(Player_W / 64.f, Player_H / 64.f);
    mSprite.setPosition(mPosition);
    return true;
}

void CPEOPLE::reloadSprite(const std::string& texturePath) {
    mRow = 0;
    mIsMoving = false;
    const auto& texture = TextureManager::getInstance().getTexture(texturePath);
    mAnim = std::make_unique<Animation>(mSprite, texture,
        64, 64, 4, 1, Frame_Time);
    mSprite.setScale(Player_W / 64.f, Player_H / 64.f);
    //KO load vị trí mới
}

void CPEOPLE::update(float dt) {
    if (mInvulnerableTimer > 0.f) {
        mInvulnerableTimer -= dt;
        if (mInvulnerableTimer < 0.f) mInvulnerableTimer = 0.f;
    }

    mBubblePulseTimer += dt * 5.f;

    mAuraRotation += dt * 500.f;
    if (mAuraRotation >= 360.f) mAuraRotation -= 360.f;

    if (mStats.cooldownWarningTimer > 0.f) {
        mStats.cooldownWarningTimer -= dt;
        if (mStats.cooldownWarningTimer < 0.f) mStats.cooldownWarningTimer = 0.f;
    }

    mStats.maxHp = 3;
    if (mStats.currentHp > mStats.maxHp) {
        mStats.currentHp = mStats.maxHp;
    }

    // Update Skill 'E' timer
    if (mStats.skillActive) {
        mStats.skillTimer -= dt;
        if (mStats.skillTimer <= 0.f) {
            mStats.skillActive = false;
            mStats.skillTimer = 0.f;
            mStats.skillCooldownTimer = mStats.skillCooldownDuration; // 10.0s cooldown
        }
    } else if (mStats.skillCooldownTimer > 0.f) {
        mStats.skillCooldownTimer -= dt;
        if (mStats.skillCooldownTimer < 0.f) mStats.skillCooldownTimer = 0.f;
    }

    // Update Skill 'Q' (Radar Pulse) timer
    if (mStats.radarActive) {
        mStats.radarTimer -= dt;
        mStats.radarPulseRadius += dt * 250.f;
        if (mStats.radarTimer <= 0.f) {
            mStats.radarActive = false;
            mStats.radarTimer = 0.f;
            mStats.radarCooldownTimer = mStats.radarCooldownDuration; // 10.0s cooldown
        }
    } else if (mStats.radarCooldownTimer > 0.f) {
        mStats.radarCooldownTimer -= dt;
        if (mStats.radarCooldownTimer < 0.f) mStats.radarCooldownTimer = 0.f;
    }

    // Update Skill 'T' (Time Freeze Clock) timer
    if (mStats.timeFreezeActive) {
        mStats.timeFreezeTimer -= dt;
        if (mStats.timeFreezeTimer <= 0.f) {
            mStats.timeFreezeActive = false;
            mStats.timeFreezeTimer = 0.f;
            mStats.timeFreezeCooldownTimer = mStats.timeFreezeCooldownDuration; // 12.0s cooldown
        }
    } else if (mStats.timeFreezeCooldownTimer > 0.f) {
        mStats.timeFreezeCooldownTimer -= dt;
        if (mStats.timeFreezeCooldownTimer < 0.f) mStats.timeFreezeCooldownTimer = 0.f;
    }

    if (!mAnim || !mSprite.getTexture()) return;

    unsigned int texH = mSprite.getTexture()->getSize().y;
    int maxRows = std::max(1u, texH / 64u);
    int validRow = mRow % maxRows;

    if (mIsMoving) {
        mAnim->update(dt);
        // Đổi sang đúng hàng
        sf::IntRect rect = mSprite.getTextureRect();
        rect.top = validRow * 64;
        mSprite.setTextureRect(rect);
    } else {
        // Đứng yên — hiện frame đầu tiên
        mSprite.setTextureRect(sf::IntRect(0, validRow * 64, 64, 64));
    }
}

void CPEOPLE::activateSpeedSkill() {
    if (ShopData::getItemCount("speed") > 0 || mStats.hasSpeedSkill) {
        if (mStats.skillCooldownTimer <= 0.f && !mStats.skillActive) {
            if (ShopData::getItemCount("speed") > 0) {
                ShopData::consumeItem("speed");
            }
            mStats.skillActive = true;
            mStats.skillTimer = mStats.skillDuration; // 5.0s
            printf("SPEED SURGE SKILL ACTIVATED (Count left: %d)!\n", ShopData::getItemCount("speed"));
        } else if (mStats.skillCooldownTimer > 0.f) {
            mStats.cooldownWarningMsg = "SKILL ON COOLDOWN";
            mStats.cooldownWarningTimer = 1.2f;
        }
    } else {
        mStats.cooldownWarningMsg = "NO SPEED BOOTS IN INVENTORY";
        mStats.cooldownWarningTimer = 1.2f;
    }
}

void CPEOPLE::activateRadarSkill() {
    if (ShopData::getItemCount("radar") > 0 || mStats.hasRadarSkill) {
        if (mStats.radarCooldownTimer <= 0.f && !mStats.radarActive) {
            if (ShopData::getItemCount("radar") > 0) {
                ShopData::consumeItem("radar");
            }
            mStats.radarActive = true;
            mStats.radarTimer = mStats.radarDuration; // 6.0s duration of Coin Magnet suction
            mStats.radarPulseRadius = 0.f;
            printf("COIN MAGNET RADAR ACTIVATED (Count left: %d)!\n", ShopData::getItemCount("radar"));
        } else if (mStats.radarCooldownTimer > 0.f) {
            mStats.cooldownWarningMsg = "SKILL ON COOLDOWN";
            mStats.cooldownWarningTimer = 1.2f;
        }
    } else {
        mStats.cooldownWarningMsg = "NO COIN RADAR IN INVENTORY";
        mStats.cooldownWarningTimer = 1.2f;
    }
}

void CPEOPLE::activateTimeSkill() {
    if (ShopData::getItemCount("time") > 0 || mStats.hasTimeSkill) {
        if (mStats.timeFreezeCooldownTimer <= 0.f && !mStats.timeFreezeActive) {
            if (ShopData::getItemCount("time") > 0) {
                ShopData::consumeItem("time");
            }
            mStats.timeFreezeActive = true;
            mStats.timeFreezeTimer = mStats.timeFreezeDuration; // 5.0s
            printf("TIME FREEZE CLOCK ACTIVATED (Count left: %d)!\n", ShopData::getItemCount("time"));
        } else if (mStats.timeFreezeCooldownTimer > 0.f) {
            mStats.cooldownWarningMsg = "SKILL ON COOLDOWN";
            mStats.cooldownWarningTimer = 1.2f;
        }
    } else {
        mStats.cooldownWarningMsg = "NO TIME EXTENDER IN INVENTORY";
        mStats.cooldownWarningTimer = 1.2f;
    }
}

void CPEOPLE::triggerCooldownWarning() {
    mStats.cooldownWarningMsg = "SKILL ON COOLDOWN";
    mStats.cooldownWarningTimer = 1.2f;
}

// Move
void CPEOPLE::Move(float dt, int score, bool hasActiveHazard)
{
    if (mIsDead || mIsFinish) return;

    // Check Skill 'E' Key
    bool ePressedNow = sf::Keyboard::isKeyPressed(sf::Keyboard::E);
    if (ePressedNow && !mEPressedLast) {
        activateSpeedSkill();
    }
    mEPressedLast = ePressedNow;

    // Check Skill 'Q' Key (Radar EMP Pulse)
    bool qPressedNow = sf::Keyboard::isKeyPressed(sf::Keyboard::Q);
    if (qPressedNow && !mQPressedLast) {
        activateRadarSkill();
    }
    mQPressedLast = qPressedNow;

    // Check Skill 'T' Key (Time Freeze Clock)
    bool tPressedNow = sf::Keyboard::isKeyPressed(sf::Keyboard::T);
    if (tPressedNow && !mTPressedLast) {
        activateTimeSkill();
    }
    mTPressedLast = tPressedNow;

    sf::Vector2f dir(0.f, 0.f);
    mIsMoving = false;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        dir.y -= 1.f; mRow = 3; mIsMoving = true;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        dir.y += 1.f; mRow = 0; mIsMoving = true;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        dir.x -= 1.f; mRow = 1; mIsMoving = true;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        dir.x += 1.f; mRow = 2; mIsMoving = true;
    }

    // Check Sprinting (Shift key)
    bool wantSprint = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ||
                      sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);

    if (mIsMoving && wantSprint && mStats.stamina > 0.f) {
        mStats.isSprinting = true;
        mStats.stamina -= mStats.staminaDrainRate * dt;
        if (mStats.stamina < 0.f) mStats.stamina = 0.f;
    } else {
        mStats.isSprinting = false;
        mStats.stamina += mStats.staminaRegenRate * dt;
        if (mStats.stamina > mStats.maxStamina) mStats.stamina = mStats.maxStamina;
    }

    // Energy drain & regen (Energy drains faster when high score)
    float scoreDrainFactor = 1.0f + std::min(2.0f, static_cast<float>(score) / 800.f);
    if (mIsMoving) {
        mStats.energy -= mStats.baseEnergyDrain * scoreDrainFactor * dt;
        if (mStats.energy < 0.f) mStats.energy = 0.f;
    } else {
        mStats.energy += mStats.energyRegenRate * dt;
        if (mStats.energy > mStats.maxEnergy) mStats.energy = mStats.maxEnergy;
    }
    // Energy Speed Multiplier (Ranges from 60% to 100% based on energy)
    float energyMultiplier = 0.6f + 0.4f * (mStats.energy / mStats.maxEnergy);

    // Hazard Slow Penalty (25% slower when hazard is active)
    float hazardPenalty = hasActiveHazard ? 0.75f : 1.0f;

    // Speed Skill 'E' Boost (+50% speed while active for 5.0 seconds)
    float skillBoost = 0.f;
    if (mStats.skillActive) {
        skillBoost = 0.5f;
    }

    float sprintBoost = mStats.isSprinting ? 0.35f : 0.f;
    float bootsMultiplier = 1.0f;

    // Total Multiplier Calculation
    float totalMultiplier = bootsMultiplier * energyMultiplier * hazardPenalty * (1.0f + skillBoost + sprintBoost) * mPowerUpSpeedMultiplier;
    mStats.currentCalculatedSpeed = mSpeed * totalMultiplier;
    mPosition.x += dir.x * mStats.currentCalculatedSpeed * dt;
    mPosition.y += dir.y * mStats.currentCalculatedSpeed * dt;

    // Clamp inside window boundaries
    mPosition.x = std::max(0.f, std::min(mPosition.x, (float)Win_W - Player_W));
    mPosition.y = std::max(0.f, std::min(mPosition.y, (float)Win_H - Player_H));

    mSprite.setPosition(mPosition);
}

bool CPEOPLE::takeDamage(int amount) {
    if (mInvulnerableTimer > 0.f || mIsDead) return false;
    mStats.currentHp -= amount;
    if (mStats.currentHp <= 0) {
        mStats.currentHp = 0;
        return true;
    } else {
        triggerInvulnerability(1.5f);
        knockback(160.f); // Push player back down ~2 rows (2 * Lane_H)
        return false;
    }
}


void CPEOPLE::heal(int amount) {
    mStats.currentHp += amount;
    if (mStats.currentHp > mStats.maxHp) {
        mStats.currentHp = mStats.maxHp;
    }
}

void CPEOPLE::knockback(float distanceY) {
    mPosition.y += distanceY;
    mPosition.y = std::max(0.f, std::min(mPosition.y, (float)Win_H - Player_H));
    mSprite.setPosition(mPosition);
}

void CPEOPLE::resetStats() {
    mStats.maxHp = 3;
    mStats.resetHp();
    mIsDead = false;
    mIsFinish = false;
}


void CPEOPLE::setPosition(float x, float y)
{
    mPosition = sf::Vector2f(x, y);
    mSprite.setPosition(mPosition);
}

sf::Vector2f CPEOPLE::getPosition() const
{
    return mPosition;
}

sf::FloatRect CPEOPLE::getBounds() const
{
    return mSprite.getGlobalBounds();
}

sf::FloatRect CPEOPLE::getHitbox() const
{
    sf::FloatRect r = mSprite.getGlobalBounds();
    float dx = r.width * 0.28f;
    float dy = r.height * 0.26f;
    return sf::FloatRect(r.left + dx, r.top + dy, r.width - dx * 2.f, r.height - dy * 2.f);
}

bool CPEOPLE::isDead() const
{
    return mIsDead;
}

bool CPEOPLE::isFinish() const
{
    return mIsFinish;
}

void CPEOPLE::setDead(bool dead)
{
    mIsDead = dead;
}

void CPEOPLE::setFinish(bool finish)
{
    mIsFinish = finish;
}

void CPEOPLE::triggerInvulnerability(float duration) {
    mInvulnerableTimer = duration;
}

bool CPEOPLE::isInvulnerable() const {
    return mInvulnerableTimer > 0.f;
}

void CPEOPLE::Draw(sf::RenderWindow& window)
{
    float playerCenterX = mPosition.x + Player_W / 2.f;
    float playerCenterY = mPosition.y + Player_H / 2.f;

    // 1. Coin Magnet Radar Suction Field Effect [Q]
    if (!mIsBot && mStats.radarActive) {
        float alphaRatio = std::clamp(mStats.radarTimer / mStats.radarDuration, 0.f, 1.f);
        sf::Uint8 alpha = static_cast<sf::Uint8>(220 * alphaRatio);

        // Core pulsating magnetic vortex sphere
        float pulseR = 28.f + std::sin(mAuraRotation * 4.f) * 4.f;
        sf::CircleShape coreRing(pulseR);
        coreRing.setOrigin(pulseR, pulseR);
        coreRing.setPosition(playerCenterX, playerCenterY);
        coreRing.setFillColor(sf::Color(0, 210, 255, static_cast<sf::Uint8>(45 * alphaRatio)));
        coreRing.setOutlineColor(sf::Color(0, 245, 255, alpha));
        coreRing.setOutlineThickness(2.2f);
        window.draw(coreRing);

        // Orbiting magnetic suction nodes & inward converging stream lines
        for (int k = 0; k < 4; ++k) {
            float angle = mAuraRotation * 2.5f + k * (3.14159265f / 2.f);
            float orbitRadius = 40.f + std::sin(mAuraRotation * 5.f + k) * 5.f;
            float px = playerCenterX + std::cos(angle) * orbitRadius;
            float py = playerCenterY + std::sin(angle) * orbitRadius;

            sf::CircleShape node(4.f);
            node.setOrigin(4.f, 4.f);
            node.setPosition(px, py);
            node.setFillColor(sf::Color(255, 235, 100, alpha)); // Golden suction spark
            node.setOutlineColor(sf::Color(0, 240, 255, alpha));
            node.setOutlineThickness(1.5f);
            window.draw(node);

            // Inward magnetic stream line converging towards player center
            sf::Vertex streamLine[] = {
                sf::Vertex(sf::Vector2f(px, py), sf::Color(0, 240, 255, static_cast<sf::Uint8>(180 * alphaRatio))),
                sf::Vertex(sf::Vector2f(playerCenterX, playerCenterY), sf::Color(255, 215, 0, static_cast<sf::Uint8>(60 * alphaRatio)))
            };
            window.draw(streamLine, 2, sf::Lines);
        }
    }

    // 2. Flash Speed Lightning Aura Effect [E] or In-Lane Speed Boost
    if ((!mIsBot && mStats.skillActive) || mPowerUpSpeedMultiplier > 1.0f) {
        if (mAuraLoaded) {
            mFlashAuraSprite.setPosition(playerCenterX, playerCenterY);
            mFlashAuraSprite.setScale(1.25f, 1.25f);
            mFlashAuraSprite.setRotation(mAuraRotation);
            mFlashAuraSprite.setColor(sf::Color(255, 230, 80, 230));
            window.draw(mFlashAuraSprite);
        } else {
            sf::CircleShape aura(30.f);
            aura.setOrigin(30.f, 30.f);
            aura.setPosition(playerCenterX, playerCenterY);
            aura.setFillColor(sf::Color::Transparent);
            aura.setOutlineColor(sf::Color(255, 220, 50, 220));
            aura.setOutlineThickness(3.f);
            window.draw(aura);
        }
    }

    // 3. Time Freeze Clock Aura Effect [T]
    if (!mIsBot && mStats.timeFreezeActive) {
        if (mTimeFreezeLoaded) {
            mTimeFreezeSprite.setPosition(playerCenterX, playerCenterY);
            mTimeFreezeSprite.setScale(0.85f, 0.85f);
            mTimeFreezeSprite.setRotation(-mAuraRotation * 0.5f);
            mTimeFreezeSprite.setColor(sf::Color(160, 240, 255, 230));
            window.draw(mTimeFreezeSprite);
        } else {
            sf::CircleShape clockAura(35.f);
            clockAura.setOrigin(35.f, 35.f);
            clockAura.setPosition(playerCenterX, playerCenterY);
            clockAura.setFillColor(sf::Color(100, 220, 255, 40));
            clockAura.setOutlineColor(sf::Color(120, 240, 255, 230));
            clockAura.setOutlineThickness(2.5f);
            window.draw(clockAura);
        }
    }

    // 4. Energy Shield Aura (Shop shield for human player)
    if (!mIsBot && ShopData::getItemCount("shield") > 0 && !mIsDead) {
        float centerX = playerCenterX;
        float centerY = playerCenterY;

        static float auraPulse = 0.f;
        auraPulse += 0.04f;

        float pulseScale = 1.0f + 0.05f * std::sin(auraPulse * 2.f);
        float radius = 32.f * pulseScale;

        // Inner Energy Field
        sf::CircleShape shieldAura(radius);
        shieldAura.setOrigin(radius, radius);
        shieldAura.setPosition(centerX, centerY);
        shieldAura.setFillColor(sf::Color(0, 180, 255, 60));
        shieldAura.setOutlineColor(sf::Color(100, 235, 255, 230));
        shieldAura.setOutlineThickness(2.5f);
        window.draw(shieldAura);

        // Hexagonal Shield Nodes
        sf::CircleShape hexShield(radius + 2.f, 6);
        hexShield.setOrigin(radius + 2.f, radius + 2.f);
        hexShield.setPosition(centerX, centerY);
        hexShield.setRotation(auraPulse * 45.f);
        hexShield.setFillColor(sf::Color::Transparent);
        hexShield.setOutlineColor(sf::Color(0, 220, 255, 160));
        hexShield.setOutlineThickness(1.8f);
        window.draw(hexShield);

        // Orbiting Energy Particles
        for (int i = 0; i < 3; ++i) {
            float angle = auraPulse * 3.f + i * (2.f * 3.14159f / 3.f);
            float px = centerX + std::cos(angle) * (radius + 4.f);
            float py = centerY + std::sin(angle) * (radius + 4.f);

            sf::CircleShape mote(3.5f);
            mote.setOrigin(3.5f, 3.5f);
            mote.setPosition(px, py);
            mote.setFillColor(sf::Color(200, 245, 255, 240));
            window.draw(mote);
        }

        // Outer Wave Ring
        sf::CircleShape outerWave(radius + 7.f);
        outerWave.setOrigin(radius + 7.f, radius + 7.f);
        outerWave.setPosition(centerX, centerY);
        outerWave.setFillColor(sf::Color::Transparent);
        outerWave.setOutlineColor(sf::Color(0, 200, 255, 90));
        outerWave.setOutlineThickness(1.5f);
        window.draw(outerWave);
    }

    // Flash white/cyan if invulnerable grace period is active
    if (mInvulnerableTimer > 0.f) {
        if (static_cast<int>(mInvulnerableTimer * 20.f) % 2 == 0) {
            mSprite.setColor(sf::Color(100, 220, 255, 220));
        } else {
            mSprite.setColor(mBaseColor);
        }
    } else {
        mSprite.setColor(mBaseColor);
    }

    // Nitro Speed Power-Up After-Image Trail
    if (mPowerUpSpeedMultiplier > 1.1f && mIsMoving) {
        sf::Sprite ghost = mSprite;
        ghost.setColor(sf::Color(255, 120, 40, 110));
        ghost.setPosition(mPosition.x - (mRow == 1 ? -10.f : (mRow == 2 ? 10.f : 0.f)),
                           mPosition.y - (mRow == 3 ? -10.f : (mRow == 0 ? 10.f : 0.f)));
        window.draw(ghost);
    }

    window.draw(mSprite);

    // Bot Identification Badge & 3 Mini Hearts
    if (mIsBot && !mIsDead && mWarnFontLoaded) {
        sf::RectangleShape botBadgeBg(sf::Vector2f(34.f, 14.f));
        botBadgeBg.setOrigin(17.f, 7.f);
        botBadgeBg.setPosition(playerCenterX, mPosition.y - 8.f);
        botBadgeBg.setFillColor(sf::Color(220, 40, 40, 210));
        botBadgeBg.setOutlineColor(sf::Color(255, 220, 220, 240));
        botBadgeBg.setOutlineThickness(1.2f);
        window.draw(botBadgeBg);

        sf::Text botBadgeText;
        botBadgeText.setFont(mWarnFont);
        botBadgeText.setString("BOT");
        botBadgeText.setCharacterSize(11);
        botBadgeText.setFillColor(sf::Color::White);
        sf::FloatRect b = botBadgeText.getLocalBounds();
        botBadgeText.setOrigin(b.left + b.width / 2.f, b.top + b.height / 2.f);
        botBadgeText.setPosition(playerCenterX, mPosition.y - 8.f);
        window.draw(botBadgeText);

        // 3 Mini Heart Indicators for Bot HP
        int curHp = mStats.currentHp;
        for (int h = 0; h < 3; ++h) {
            sf::CircleShape miniHeart(2.5f);
            miniHeart.setOrigin(2.5f, 2.5f);
            miniHeart.setPosition(playerCenterX - 8.f + h * 8.f, mPosition.y - 18.f);
            if (h < curHp) {
                miniHeart.setFillColor(sf::Color(255, 60, 60));
                miniHeart.setOutlineColor(sf::Color(255, 200, 200));
            } else {
                miniHeart.setFillColor(sf::Color(50, 50, 50, 200));
                miniHeart.setOutlineColor(sf::Color(90, 90, 90, 160));
            }
            miniHeart.setOutlineThickness(0.8f);
            window.draw(miniHeart);
        }
    }

    // Dynamic PowerUp Shield Bubble (Golden Energy Sphere)
    if (mHasPowerUpShield && !mIsDead) {
        float bubbleRadius = 27.f + std::sin(mBubblePulseTimer) * 2.5f;
        sf::CircleShape bubble(bubbleRadius);
        bubble.setOrigin(bubbleRadius, bubbleRadius);
        bubble.setPosition(playerCenterX, playerCenterY);
        bubble.setFillColor(sf::Color(255, 215, 0, 45));
        bubble.setOutlineColor(sf::Color(255, 230, 80, 200));
        bubble.setOutlineThickness(2.2f);
        window.draw(bubble);

        // Orbiting golden spark
        float sparkAngle = mBubblePulseTimer * 2.5f;
        sf::CircleShape spark(3.f);
        spark.setOrigin(3.f, 3.f);
        spark.setPosition(playerCenterX + std::cos(sparkAngle) * (bubbleRadius + 3.f),
                          playerCenterY + std::sin(sparkAngle) * (bubbleRadius + 3.f));
        spark.setFillColor(sf::Color(255, 255, 200, 240));
        window.draw(spark);
    }

    // Draw Cooldown Warning Floating Text if active ("SKILL ON COOLDOWN")
    if (mStats.cooldownWarningTimer > 0.f && mWarnFontLoaded) {
        sf::Text warnText;
        warnText.setFont(mWarnFont);
        warnText.setString(mStats.cooldownWarningMsg.empty() ? "SKILL ON COOLDOWN" : mStats.cooldownWarningMsg);
        warnText.setCharacterSize(14);

        float alphaRatio = std::clamp(mStats.cooldownWarningTimer / 1.2f, 0.f, 1.f);
        sf::Uint8 alpha = static_cast<sf::Uint8>(255 * alphaRatio);

        warnText.setFillColor(sf::Color(255, 220, 80, alpha));
        warnText.setOutlineColor(sf::Color(20, 10, 10, alpha));
        warnText.setOutlineThickness(1.8f);

        sf::FloatRect b = warnText.getLocalBounds();
        warnText.setOrigin(b.left + b.width / 2.f, b.top + b.height / 2.f);

        float textY = mPosition.y - 18.f - (1.2f - mStats.cooldownWarningTimer) * 18.f;
        warnText.setPosition(playerCenterX, textY);
        window.draw(warnText);
    }

    // Draw mini stamina bar under player if stamina < maxStamina
    if (mStats.stamina < mStats.maxStamina && !mIsDead) {
        float barW = Player_W;
        float barH = 5.f;
        float barX = mPosition.x;
        float barY = mPosition.y + Player_H + 4.f;

        sf::RectangleShape bgBar(sf::Vector2f(barW, barH));
        bgBar.setPosition(barX, barY);
        bgBar.setFillColor(sf::Color(30, 30, 30, 200));
        bgBar.setOutlineColor(sf::Color(100, 100, 100, 250));
        bgBar.setOutlineThickness(1.f);
        window.draw(bgBar);

        float fillRatio = mStats.stamina / mStats.maxStamina;
        sf::RectangleShape fillBar(sf::Vector2f(barW * fillRatio, barH));
        fillBar.setPosition(barX, barY);
        fillBar.setFillColor(mStats.isSprinting ? sf::Color(255, 200, 50, 240) : sf::Color(50, 220, 100, 240));
        window.draw(fillBar);
    }
}
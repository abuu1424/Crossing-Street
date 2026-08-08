#include "CPEOPLE.h"
#include "TextureManager.h"
#include "ShopData.h"
#include <cmath>
#include <memory>

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
    if (ShopData::isItemPurchased("speed_skill") || mStats.hasSpeedSkill) {
        if (mStats.skillCooldownTimer <= 0.f && !mStats.skillActive) {
            mStats.skillActive = true;
            mStats.skillTimer = mStats.skillDuration; // 5.0s
            printf("SPEED SURGE SKILL ACTIVATED (+0.5 Initial Speed Boost)!\n");
        }
    }
}

// Move
void CPEOPLE::Move(float dt, int score, bool hasActiveHazard)
{
    if (mIsDead || mIsFinish) return;

    // Check Skill 'E' Key
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
        activateSpeedSkill();
    }

    // Update Skill timers
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

    // Decaying Speed Skill 'E' Boost (+0.5 initially, decaying to 0 over 5 seconds)
    float skillBoost = 0.f;
    if (mStats.skillActive && mStats.skillDuration > 0.f) {
        float ratio = mStats.skillTimer / mStats.skillDuration; // 1.0 -> 0.0
        skillBoost = 0.5f * ratio; // Starts at +0.5 multiplier, decays down
    }

    float sprintBoost = mStats.isSprinting ? 0.35f : 0.f;
    float bootsMultiplier = ShopData::isItemPurchased("speed") ? 1.2f : 1.0f;

    float totalMultiplier = bootsMultiplier * energyMultiplier * hazardPenalty * (1.0f + skillBoost + sprintBoost);
    mStats.currentCalculatedSpeed = mSpeed * totalMultiplier;

    mPosition.x += dir.x * mStats.currentCalculatedSpeed * dt;
    mPosition.y += dir.y * mStats.currentCalculatedSpeed * dt;
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
    if (ShopData::isItemPurchased("shield") && !mIsDead) {
        float centerX = mPosition.x + Player_W / 2.f;
        float centerY = mPosition.y + Player_H / 2.f;

        static float auraPulse = 0.f;
        auraPulse += 0.04f;

        float pulseScale = 1.0f + 0.05f * std::sin(auraPulse * 2.f);
        float radius = 32.f * pulseScale;

        // 1. Inner Energy Field (Translucent Glowing Cyan)
        sf::CircleShape shieldAura(radius);
        shieldAura.setOrigin(radius, radius);
        shieldAura.setPosition(centerX, centerY);
        shieldAura.setFillColor(sf::Color(0, 180, 255, 60));
        shieldAura.setOutlineColor(sf::Color(100, 235, 255, 230));
        shieldAura.setOutlineThickness(2.5f);
        window.draw(shieldAura);

        // 2. Hexagonal Shield Nodes (6 Vertices)
        sf::CircleShape hexShield(radius + 2.f, 6);
        hexShield.setOrigin(radius + 2.f, radius + 2.f);
        hexShield.setPosition(centerX, centerY);
        hexShield.setRotation(auraPulse * 45.f);
        hexShield.setFillColor(sf::Color::Transparent);
        hexShield.setOutlineColor(sf::Color(0, 220, 255, 160));
        hexShield.setOutlineThickness(1.8f);
        window.draw(hexShield);

        // 3. Orbiting Energy Particles (3 Motes)
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

        // 4. Outer Wave Ring
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
            mSprite.setColor(sf::Color(255, 255, 255, 255));
        }
    } else {
        mSprite.setColor(sf::Color(255, 255, 255, 255));
    }

    window.draw(mSprite);

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
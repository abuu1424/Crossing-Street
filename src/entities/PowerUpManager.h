#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <unordered_map>

class SoundManager;
class CPEOPLE;

enum class PowerUpType {
    MAGNET,       // Hút coin trong phạm vi lớn (8s)
    TIME_STOP,    // Đóng băng/làm chậm 85% xe cộ & thú (4s)
    SPEED_BOOST,  // Tăng 60% tốc độ di chuyển (6s)
    SHIELD,       // Khiên năng lượng chống 1 lần va chạm
    SCORE_X2      // Nhân đôi điểm thưởng (10s)
};

struct PowerUpItem {
    PowerUpType type;
    sf::Vector2f position;
    bool collected = false;
    float bobbingTimer = 0.f;
    float glowTimer = 0.f;
};

struct FloatingPowerUpText {
    std::string text;
    sf::Vector2f position;
    sf::Color color;
    float alpha = 255.f;
    float lifetime = 1.0f;
};

class PowerUpManager {
public:
    PowerUpManager();

    void init(const sf::Font& font);
    void reset();
    void spawnForLevel(int level, bool isEndless = false);
    void update(float dt, const sf::FloatRect& playerHitbox, SoundManager* sound = nullptr);
    void draw(sf::RenderWindow& window);

    // Active Buffs State
    bool isMagnetActive() const { return mMagnetTimer > 0.f; }
    bool isTimeStopActive() const { return mTimeStopTimer > 0.f; }
    bool isSpeedBoostActive() const { return mSpeedBoostTimer > 0.f; }
    bool isScoreX2Active() const { return mScoreX2Timer > 0.f; }
    bool hasBubbleShield() const { return mHasShield; }

    void breakShield() { mHasShield = false; }
    void addShield() { mHasShield = true; }

    // Buff Timers for HUD
    float getMagnetRemaining() const { return mMagnetTimer; }
    float getTimeStopRemaining() const { return mTimeStopTimer; }
    float getSpeedBoostRemaining() const { return mSpeedBoostTimer; }
    float getScoreX2Remaining() const { return mScoreX2Timer; }

    float getMagnetDuration() const { return 8.0f; }
    float getTimeStopDuration() const { return 4.0f; }
    float getSpeedBoostDuration() const { return 6.0f; }
    float getScoreX2Duration() const { return 10.0f; }

    // Bot PowerUp pickup handling
    void checkBotPickup(const sf::FloatRect& botHitbox, CPEOPLE& botPlayer, SoundManager* sound = nullptr);
    bool isBotSpeedBoostActive() const { return mBotSpeedBoostTimer > 0.f; }

    bool hasItemNear(const sf::FloatRect& cellRect) const {
        for (const auto& item : mItems) {
            if (!item.collected && cellRect.contains(item.position)) {
                return true;
            }
        }
        return false;
    }

private:
    sf::Font mFont;
    bool mFontLoaded = false;

    // Textures for 5 Power-up items
    sf::Texture mTexMagnet;
    sf::Texture mTexTimeStop;
    sf::Texture mTexSpeed;
    sf::Texture mTexShield;
    sf::Texture mTexScoreX2;

    std::vector<PowerUpItem> mItems;
    std::vector<FloatingPowerUpText> mFloatingTexts;

    // Active Timers
    float mMagnetTimer = 0.f;
    float mTimeStopTimer = 0.f;
    float mSpeedBoostTimer = 0.f;
    float mScoreX2Timer = 0.f;
    float mBotSpeedBoostTimer = 0.f;
    bool mHasShield = false;

    // Visual aura & particles
    sf::CircleShape mGlowShape;
    sf::CircleShape mRingShape;

    void applyBuff(PowerUpType type, const sf::Vector2f& pos, SoundManager* sound);
};

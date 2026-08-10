#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include "Utils.h"
#include "Animation.h"
#include "PlayerStats.h"

class CPEOPLE {
    sf::Sprite                 mSprite;
    sf::Texture                mTexture;
    std::unique_ptr<Animation> mAnim;

    sf::Vector2f mPosition;
    float        mSpeed;
    bool         mIsDead;
    bool         mIsFinish;
    int          mRow      = 0;
    bool         mIsMoving = false;

    PlayerStats  mStats;

public:
    CPEOPLE();
    ~CPEOPLE();
    bool loadSprite(const std::string& texturePath);
    void reloadSprite(const std::string& texturePath);
    void Move(float dt, int score = 0, bool hasActiveHazard = false);
    void activateSpeedSkill();
    void activateRadarSkill();
    void activateTimeSkill();
    void triggerCooldownWarning();
    void update(float dt);

    void setPosition(float x, float y);
    void setSpeed(float speed) { mSpeed = speed; }
    float getSpeed() const { return mSpeed; }
    sf::Vector2f  getPosition() const;
    sf::FloatRect getBounds() const;
    sf::FloatRect getHitbox() const;
    bool isDead() const;
    bool isFinish() const;
    void setDead(bool dead);
    void setFinish(bool finish);
    void triggerInvulnerability(float duration = 1.2f);
    bool isInvulnerable() const;

    // Stats & Combat mechanics
    const PlayerStats& getStats() const { return mStats; }
    PlayerStats& getStats() { return mStats; }
    bool takeDamage(int amount = 1);
    void heal(int amount = 1);
    void knockback(float distanceY = 160.f);
    void resetStats();

    void Draw(sf::RenderWindow& window);
private:
    float mInvulnerableTimer = 0.f;
    bool  mEPressedLast = false;
    bool  mQPressedLast = false;
    bool  mTPressedLast = false;

    // Skill animation visual assets
    sf::Texture mFlashAuraTexture;
    sf::Sprite  mFlashAuraSprite;
    sf::Texture mRadarWaveTexture;
    sf::Sprite  mRadarWaveSprite;
    sf::Texture mTimeFreezeTexture;
    sf::Sprite  mTimeFreezeSprite;
    sf::Font    mWarnFont;
    sf::Text    mWarnText;
    float       mAuraRotation = 0.f;
    bool        mAuraLoaded = false;
    bool        mRadarLoaded = false;
    bool        mTimeFreezeLoaded = false;
    bool        mWarnFontLoaded = false;
};
#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <string>

enum class HazardType {
    NONE,
    DINO_STAMPEDE, // Level 1: Prehistoric
    SANDSTORM,     // Level 2: Ancient Egypt
    ARROW_RAIN,    // Level 3: Medieval
    RUSH_HOUR,     // Level 4: Modern City
    LASER_SCAN     // Level 5: Sci-Fi / Cyberpunk
};

struct SandParticle {
    sf::Vector2f position;
    float speed;
    float radius;
    int colorIdx;
    float alpha;
};

struct WindStreak {
    sf::Vector2f position;
    float speed;
    float scaleX;
    float scaleY;
    float alpha;
};

struct ArrowTarget {
    sf::Vector2f position;
    float warningTimer;
    float maxWarningTime;
    bool active;
    bool struck;
    float arrowY;
};

struct StampedeDino {
    int textureIdx;
    float xOffset;
    float laneY;
    float scale;
};

struct LaserLane {
    float laneY;
    float warningTimer;
    float maxWarningTime;
    float activeTimer;
    bool active;
};

class HazardManager {
public:
    HazardManager();
    ~HazardManager() = default;

    void init(const sf::Font& font);
    void startLevel(int levelNumber);
    void reset();

    void update(float dt, const sf::Vector2f& playerPos, std::vector<std::pair<sf::FloatRect, float>>& extraObstacleHitboxes);
    void draw(sf::RenderWindow& window) const;
    void drawUI(sf::RenderWindow& window) const;

    // Shake offset for window view
    sf::Vector2f getShakeOffset() const { return mShakeOffset; }

    // Wind drift applied to player during Sandstorm
    sf::Vector2f getPlayerWindDrift() const { return mWindDrift; }

    // Multiplier for vehicle speeds during Rush Hour
    float getSpeedMultiplier() const { return mSpeedMultiplier; }

    bool isWarningActive() const { return mIsWarning; }
    bool isHazardActive() const { return mIsActive; }

private:
    HazardType mCurrentHazard = HazardType::NONE;
    int mCurrentLevel = 1;
    sf::Font mFont;

    float mCooldownTimer = 10.0f;
    float mWarningTimer = 0.0f;
    float mActiveTimer = 0.0f;

    bool mIsWarning = false;
    bool mIsActive = false;

    // Visuals & Effects
    sf::Vector2f mPlayerPos = {0.f, 0.f};
    sf::Vector2f mShakeOffset = {0.f, 0.f};
    sf::Vector2f mWindDrift = {0.f, 0.f};
    float mSpeedMultiplier = 1.0f;

    // Level 1: Stampede (2 Lanes with mixed Dino types)
    float mStampedeProgress = 0.f;
    std::vector<sf::Texture> mDinoTextures;
    std::vector<StampedeDino> mStampedeDinos;

    // Level 2: Sandstorm
    std::vector<SandParticle> mSandParticles;
    std::vector<WindStreak> mWindStreaks;
    sf::RectangleShape mSandOverlay;
    sf::Texture mSandParticleTexture;
    sf::Texture mWindTexture;

    // Level 3: Arrow Rain
    std::vector<ArrowTarget> mArrowTargets;
    sf::Texture mArrowTexture;
    sf::Texture mReticleTexture;

    // Level 4: Rush Hour
    sf::RectangleShape mRushHourBanner;
    sf::Text mRushHourText;

    // Level 5: Sci-Fi Per-Lane Lasers
    std::vector<LaserLane> mLaserLanes;
    sf::Texture mLaserTexture;

    // Warning Banner Text
    sf::Text mWarningText;
    sf::RectangleShape mWarningBox;

    void triggerHazard();
    void endHazard();
    void updateParticles(float dt);
};

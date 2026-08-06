#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

enum class HazardType {
    NONE,
    DINO_STAMPEDE,  // Level 1: Prehistoric
    SANDSTORM,      // Level 2: Desert
    ARROW_RAIN,     // Level 3: Medieval
    RUSH_HOUR,      // Level 4: Modern City (Thundercloud)
    LASER_SCAN      // Level 5: Sci-Fi
};

class SoundManager;

struct SandParticle {
    sf::Vector2f position;
    float speed, radius, alpha;
    int colorIdx;
};

struct WindStreak {
    sf::Vector2f position;
    float speed, scaleX, scaleY, alpha;
};

struct ArrowTarget {
    sf::Vector2f position;
    float warningTimer, maxWarningTime, arrowY;
    bool active, struck;
    bool soundPlayed = false;
};

struct LaserLane {
    float laneY, warningTimer, maxWarningTime, activeTimer;
    bool active;
    bool soundPlayed = false;
};

struct HerdAnimal {
    float posX, laneY, speed, scale;
    int textureIdx, direction;
    int frameCount, frameW, frameH;
    float animTimer = 0.f;
    int currentFrame = 0;
    int animCols = 2;
    bool offScreen = false;
};

struct HerdLane {
    float laneY, warningTimer, maxWarningTime;
    int direction;
    bool active;
    std::vector<HerdAnimal> animals;
};

// Level 4: Thundercloud - vertical lightning strike down to a lane
struct LightningStrike {
    float laneY;
    float strikeX;
    float warningTimer;  // flashes warning before strike
    float activeTimer;   // strike is lethal while > 0
    bool active;
    bool soundPlayed = false;
};

struct RainDrop {
    sf::Vector2f position;
    float speed;
    float length;
};

class HazardManager {
public:
    HazardManager();
    ~HazardManager() = default;

    void init(const sf::Font& font, SoundManager* sound = nullptr);
    void setSoundManager(SoundManager* sound) { mSound = sound; }
    void startLevel(int levelNumber);
    void reset();

    void update(float dt, const sf::Vector2f& playerPos,
                std::vector<std::pair<sf::FloatRect, float>>& extraHitboxes);
    void draw(sf::RenderWindow& window) const;
    void drawUI(sf::RenderWindow& window) const;

    sf::Vector2f getShakeOffset()     const { return mShakeOffset; }
    sf::Vector2f getPlayerWindDrift() const { return mWindDrift; }
    float        getSpeedMultiplier() const { return mSpeedMultiplier; }
    bool         isWarningActive()    const { return mIsWarning; }
    bool         isHazardActive()     const { return mIsActive; }

private:
    SoundManager* mSound         = nullptr;
    HazardType    mCurrentHazard = HazardType::NONE;
    int           mCurrentLevel  = 1;
    sf::Font      mFont;

    float mCooldownTimer = 10.f;
    float mWarningTimer  = 0.f;
    float mActiveTimer   = 0.f;
    bool  mIsWarning     = false;
    bool  mIsActive      = false;

    sf::Vector2f mPlayerPos       = {0.f, 0.f};
    sf::Vector2f mShakeOffset     = {0.f, 0.f};
    sf::Vector2f mWindDrift       = {0.f, 0.f};
    float        mSpeedMultiplier = 1.f;

    sf::Text           mWarningText;
    sf::RectangleShape mWarningBox;

    // Level 1: Herd Stampede
    std::vector<sf::Texture> mDinoTextures;
    struct DinoFrameInfo { int frameCount, frameW, frameH; };
    std::vector<DinoFrameInfo> mDinoFrameData;
    std::vector<HerdLane>      mHerdLanes;

    // Level 2: Sandstorm
    std::vector<SandParticle> mSandParticles;
    std::vector<WindStreak>   mWindStreaks;
    sf::Texture               mWindTexture;

    // Level 3: Arrow Rain
    std::vector<ArrowTarget> mArrowTargets;
    sf::Texture              mArrowTexture;
    sf::Texture              mReticleTexture;

    // Level 4: Thundercloud & Heavy Rain
    sf::Texture                  mThundercloudTexture;
    sf::Texture                  mLightningTexture;
    float                        mCloudPosX     = -200.f;
    float                        mLightningTimer = 0.f;
    std::vector<LightningStrike> mLightningStrikes;
    std::vector<RainDrop>        mRainDrops;

    // Level 5: Laser
    std::vector<LaserLane> mLaserLanes;
    sf::Texture            mLaserTexture;

    void triggerHazard();
    void endHazard();
    void updateParticles(float dt);
};

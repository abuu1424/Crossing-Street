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
    BLACK_HOLE      // Level 5: Sci-Fi (Black Hole & Gravity Surge)
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
    float animTimer = 0.f;
    int currentFrame = 0;
};

struct LaserLane {
    float laneY, warningTimer, maxWarningTime, activeTimer;
    bool active;
    bool soundPlayed = false;
    float animTimer = 0.f;
    int currentFrame = 0;
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
    bool soundPlayed = false;
    std::vector<HerdAnimal> animals;
    float animTimer = 0.f;
    int currentFrame = 0;
};

// Level 4: Thundercloud - vertical lightning strike down to a lane
struct LightningStrike {
    float laneY;
    float strikeX;
    float warningTimer;  // flashes warning before strike
    float activeTimer;   // strike is lethal while > 0
    bool active;
    bool soundPlayed = false;
    float animTimer = 0.f;
    int currentFrame = 0;
};

struct RainDrop {
    sf::Vector2f position;
    float speed;
    float length;
};

// Level 5: Black Hole & Gravity Surge
struct BlackHoleParticle {
    float angle;
    float dist;
    float speed;
    float radius;
    sf::Color color;
};

struct BlackHoleInstance {
    sf::Vector2f position;
    float animTimer = 0.f;
    int currentFrame = 0;
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
                std::vector<std::pair<sf::FloatRect, float>>& extraHitboxes,
                int currentScore = 0);
    void draw(sf::RenderWindow& window) const;
    void drawUI(sf::RenderWindow& window) const;

    sf::Vector2f getShakeOffset()     const { return mShakeOffset; }
    sf::Vector2f getPlayerWindDrift() const { return mWindDrift; }
    float        getSpeedMultiplier() const { return mSpeedMultiplier; }
    bool         isWarningActive()    const { return mIsWarning; }
    bool         isHazardActive()     const { return mIsActive; }

    std::vector<sf::FloatRect> getDangerZones() const;

private:
    SoundManager* mSound         = nullptr;
    HazardType    mCurrentHazard = HazardType::NONE;
    int           mCurrentLevel  = 1;
    int           mPlayerScore   = 0;
    sf::Font      mFont;

    float mCooldownTimer = 10.f;
    float mWarningTimer  = 0.f;
    float mActiveTimer   = 0.f;
    bool  mIsWarning     = false;
    bool  mIsActive      = false;

    float calculateCooldownFromScore(int score) const;

    sf::Vector2f mPlayerPos       = {0.f, 0.f};
    sf::Vector2f mShakeOffset     = {0.f, 0.f};
    sf::Vector2f mWindDrift       = {0.f, 0.f};
    float        mSpeedMultiplier = 1.f;

    sf::Text           mWarningText;
    sf::RectangleShape mWarningBox;

    // Level 1: Herd Stampede (12-Frame Animations)
    std::vector<sf::Texture> mDinoTextures;
    struct DinoFrameInfo { int frameCount, frameW, frameH; };
    std::vector<DinoFrameInfo> mDinoFrameData;
    std::vector<HerdLane>      mHerdLanes;
    sf::Texture                mDinoWarningSignAnimTexture;
    sf::Texture                mDinoLaneWarningAnimTexture;

    // Level 2: Sandstorm (12-Frame Animation & Fine Sand)
    std::vector<SandParticle> mSandParticles;
    std::vector<WindStreak>   mWindStreaks;
    sf::Texture               mSandstormAnimTexture;
    float                     mSandAnimTimer = 0.f;

    // Level 3: Arrow Rain (12-Frame Animation)
    std::vector<ArrowTarget> mArrowTargets;
    sf::Texture              mArrowTexture;
    sf::Texture              mReticleAnimTexture;
    float                    mArrowSpawnTimer = 0.f;

    // Level 4: Thundercloud & Heavy Rain (12-Frame Animations)
    sf::Texture                  mThundercloudAnimTexture;
    sf::Texture                  mLightningAnimTexture;
    sf::Texture                  mLightningWarningAnimTexture;
    float                        mCloudPosX     = -200.f;
    float                        mCloudAnimTimer = 0.f;
    float                        mLightningTimer = 0.f;
    std::vector<LightningStrike> mLightningStrikes;
    std::vector<RainDrop>        mRainDrops;

    // Level 5: 12-Frame Laser & Cyber Black Hole
    std::vector<LaserLane>         mLaserLanes;
    sf::Texture                    mLaserAnimTexture;
    sf::Texture                    mBlackHoleAnimTexture;
    sf::Texture                    mBlackHoleRingAnimTexture;
    std::vector<BlackHoleInstance> mBlackHoles;
    std::vector<BlackHoleParticle> mBlackHoleParticles;

    void triggerHazard();
    void endHazard();
    void updateParticles(float dt);
};

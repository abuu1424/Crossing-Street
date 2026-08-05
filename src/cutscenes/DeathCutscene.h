#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include "Animation.h"

enum class DeathPhase {
    IDLE,
    RUNNING,
    FINISHED
};

class DeathCutscene {
private:
    DeathPhase mPhase = DeathPhase::IDLE;
    int mCurrentLevel = 1;
    sf::Vector2f mPosition;
    
    sf::Texture mTexture;
    sf::Sprite mSprite;
    std::unique_ptr<Animation> mAnim;

    float mPhaseTimer = 0.f;
    float mTotalDuration = 1.0f;
    float mHidePlayerRatio = 0.4f; // Tỉ lệ thời gian để ẩn nhân vật (khi bị nuốt/phân rã)
    
    float mFadeAlpha = 0.f;       // Độ mờ màn hình (0.f -> 180.f)
    sf::Vector2f mShakeOffset;   // Hiệu ứng rung màn hình

public:
    DeathCutscene();

    void start(sf::Vector2f playerPos, int level);
    void update(float dt);
    void render(sf::RenderWindow& window);
    void reset();

    bool isActive() const { return mPhase == DeathPhase::RUNNING; }
    bool isFinished() const { return mPhase == DeathPhase::FINISHED; }
    bool shouldHidePlayer() const;
    sf::Vector2f getShakeOffset() const { return mShakeOffset; }
};

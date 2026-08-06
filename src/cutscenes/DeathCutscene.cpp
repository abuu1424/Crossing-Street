#include "DeathCutscene.h"
#include <cmath>
#include <cstdio>
#include <cstdlib>

DeathCutscene::DeathCutscene()
    : mPhase(DeathPhase::IDLE), mCurrentLevel(1), mPosition(0.f, 0.f),
      mPhaseTimer(0.f), mTotalDuration(1.0f), mHidePlayerRatio(0.4f),
      mFadeAlpha(0.f), mShakeOffset(0.f, 0.f) {}

void DeathCutscene::start(sf::Vector2f playerPos, int level) {
  mPosition = playerPos;
  mCurrentLevel = level;
  mPhaseTimer = 0.f;
  mFadeAlpha = 0.f;
  mShakeOffset = sf::Vector2f(0.f, 0.f);

  std::string texturePath;
  int cols = 4;
  int rows = 3;
  float frameTime = 0.07f;
  float scale = 2.5f;

  sf::Vector2f originOffset(-1.f, -1.f);

  switch (level) {
  case 1:
    // Quái vật nuốt chửng (12 frames, 4x3)
    texturePath = "assets/sprites/lv1_sp/collision/collision_1.png";
    cols = 4;
    rows = 3;
    frameTime = 0.08f;
    mTotalDuration = 12 * frameTime;
    mHidePlayerRatio = 0.45f;
    break;
  case 2:
    // Nổ bụi vàng / Cát lún (12 frames, 4x3)
    texturePath = "assets/sprites/lv2_sp/collision/collision_2.png";
    cols = 4;
    rows = 3;
    frameTime = 0.07f;
    mTotalDuration = 12 * frameTime;
    mHidePlayerRatio = 0.35f;
    originOffset = sf::Vector2f(64.f, 98.f);
    break;
  case 3:
    // Lốc xoáy lá cây (8 frames, 4x2)
    texturePath = "assets/sprites/lv3_sp/collision/collision_3.png";
    cols = 4;
    rows = 2;
    frameTime = 0.07f;
    mTotalDuration = 8 * frameTime;
    mHidePlayerRatio = 0.40f;
    break;
  case 4:
    // Khói nổ ô tô (12 frames, 4x3)
    texturePath = "assets/sprites/lv4_sp/collision/collision_4.png";
    cols = 4;
    rows = 3;
    frameTime = 0.06f;
    mTotalDuration = 12 * frameTime;
    mHidePlayerRatio = 0.30f;
    break;
  case 5:
    // Nổ Plasma năng lượng (12 frames, 4x3)
    texturePath = "assets/sprites/lv5_sp/collision/collision_5.png";
    cols = 4;
    rows = 3;
    frameTime = 0.06f;
    mTotalDuration = 12 * frameTime;
    mHidePlayerRatio = 0.25f;
    break;
  default:
    texturePath = "assets/sprites/lv1_sp/collision/collision_1.png";
    cols = 4;
    rows = 3;
    frameTime = 0.07f;
    mTotalDuration = 12 * frameTime;
    mHidePlayerRatio = 0.4f;
    break;
  }

  if (mTexture.loadFromFile(texturePath)) {
    int fw = static_cast<int>(mTexture.getSize().x) / cols;
    int fh = static_cast<int>(mTexture.getSize().y) / rows;

    mAnim = std::make_unique<Animation>(mSprite, mTexture, fw, fh, cols, rows,
                                        frameTime, false);
    if (originOffset.x < 0.f) {
      originOffset = sf::Vector2f(static_cast<float>(fw) / 2.f,
                                  static_cast<float>(fh) / 2.f);
    }
    mSprite.setOrigin(originOffset);
    mSprite.setScale(scale, scale);
    mSprite.setPosition(mPosition);

    mPhase = DeathPhase::RUNNING;
  } else {
    printf("FAILED to load death cutscene texture: %s\n", texturePath.c_str());
    mPhase = DeathPhase::FINISHED;
  }
}

void DeathCutscene::update(float dt) {
  if (mPhase != DeathPhase::RUNNING)
    return;

  mPhaseTimer += dt;

  if (mAnim) {
    mAnim->update(dt);
  }

  // Tỷ lệ tiến trình (0.0f -> 1.0f)
  float progress =
      (mTotalDuration > 0.f) ? (mPhaseTimer / mTotalDuration) : 1.f;
  if (progress > 1.f)
    progress = 1.f;

  // Hiệu ứng Rung màn hình (Screen Shake) trong nửa đầu cutscene
  if (progress < 0.6f) {
    float shakeIntensity = (1.f - progress / 0.6f) * 6.f;
    float offsetX = ((rand() % 100) / 100.f - 0.5f) * 2.f * shakeIntensity;
    float offsetY = ((rand() % 100) / 100.f - 0.5f) * 2.f * shakeIntensity;
    mShakeOffset = sf::Vector2f(offsetX, offsetY);
  } else {
    mShakeOffset = sf::Vector2f(0.f, 0.f);
  }

  // Mờ tối dần màn hình về cuối cutscene
  if (progress > 0.5f) {
    mFadeAlpha = ((progress - 0.5f) / 0.5f) * 140.f;
  } else {
    mFadeAlpha = 0.f;
  }

  // Kiểm tra hoàn thành
  if (mAnim && mAnim->isFinished() && mPhaseTimer >= mTotalDuration) {
    mPhase = DeathPhase::FINISHED;
    mShakeOffset = sf::Vector2f(0.f, 0.f);
  }
}

void DeathCutscene::render(sf::RenderWindow &window) {
  if (mPhase != DeathPhase::RUNNING)
    return;

  // Hiển thị lớp phủ làm mờ mịt màn hình
  if (mFadeAlpha > 0.f) {
    sf::RectangleShape fadeOverlay(
        sf::Vector2f(window.getSize().x, window.getSize().y));
    fadeOverlay.setFillColor(
        sf::Color(0, 0, 0, static_cast<sf::Uint8>(mFadeAlpha)));
    window.draw(fadeOverlay);
  }

  // Vẽ Sprite animation cutscene
  window.draw(mSprite);
}

void DeathCutscene::reset() {
  mPhase = DeathPhase::IDLE;
  mPhaseTimer = 0.f;
  mFadeAlpha = 0.f;
  mShakeOffset = sf::Vector2f(0.f, 0.f);
  mAnim.reset();
}

bool DeathCutscene::shouldHidePlayer() const {
  if (mPhase != DeathPhase::RUNNING)
    return false;
  float progress =
      (mTotalDuration > 0.f) ? (mPhaseTimer / mTotalDuration) : 0.f;
  return progress >= mHidePlayerRatio;
}

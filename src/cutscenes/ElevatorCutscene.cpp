#include "ElevatorCutscene.h"
#include "SoundManager.h"
#include <algorithm>
#include <cmath>
#include <iostream>

ElevatorCutscene::ElevatorCutscene() {
  mFloors = {{1, "LEVEL 1 - PREHISTORIC", "PREHISTORIC ERA",
              "Wild Jungle, Dinosaurs & Mammoths", sf::Color(230, 150, 40)},
             {2, "LEVEL 2 - ANCIENT EGYPT", "ANCIENT EGYPT & ROME",
              "Golden Pyramids, Sphinx & Sand Dunes", sf::Color(240, 190, 50)},
             {3, "LEVEL 3 - MEDIEVAL FEUDAL", "MEDIEVAL FEUDAL ERA",
              "Stone Castles, Knights & Shield Crests", sf::Color(210, 60, 70)},
             {4, "LEVEL 4 - MODERN METROPOLIS", "MODERN METROPOLIS",
              "Saigon City Streets & Vehicles", sf::Color(50, 180, 230)},
             {5, "LEVEL 5 - FUTURE CYBERPUNK", "FUTURE CYBERPUNK",
              "Anti-Gravity Flying Cars & Plasma UFOs",
              sf::Color(190, 60, 240)}};
}

void ElevatorCutscene::loadElevatorAssets() {
  if (!mFramePixelTexture.loadFromFile(
          "assets/ui/elevator/elevator_frame_full.png")) {
    std::cerr << "Failed to load elevator_frame_full.png" << std::endl;
  }
  mFramePixelTexture.setSmooth(false);

  if (!mDoorPixelTexture.loadFromFile(
          "assets/ui/elevator/elevator_door_full.png")) {
    std::cerr << "Failed to load elevator_door_full.png" << std::endl;
  }
  mDoorPixelTexture.setSmooth(false);

  // DÙNG CHÍNH XÁC 100% FILE BACKGROUND GỐC CỦA CÁC LEVEL DÀNH CHO BẢO TÀNG
  // THỜI GIAN
  std::string bgFiles[5] = {"assets/background/lv1_bg/bg_lv1.png",
                            "assets/background/lv2_bg/bg_lv2.png",
                            "assets/background/lv3_bg/bg_lv3.png",
                            "assets/background/lv4_bg/bg_lv4.png",
                            "assets/background/lv5_bg/bg_lv5.png"};

  for (int i = 0; i < 5; ++i) {
    if (mEraCardTextures[i].loadFromFile(bgFiles[i])) {
      mEraCardTextures[i].setSmooth(false);
      mEraCardSprites[i].setTexture(mEraCardTextures[i]);
      sf::Vector2u sz = mEraCardTextures[i].getSize();
      mEraCardSprites[i].setScale(static_cast<float>(Win_W) / sz.x,
                                  static_cast<float>(Win_H) / sz.y);
      mEraCardSprites[i].setOrigin(0.f, 0.f);
    } else {
      std::cerr << "Failed to load background image: " << bgFiles[i]
                << std::endl;
    }
  }
}

void ElevatorCutscene::init(const sf::Font &font, SoundManager *sound) {
  mFont = font;
  mSound = sound;
  loadElevatorAssets();
  setupLayout();
}

void ElevatorCutscene::setupLayout() {
  // 1280x720 Fullscreen Background Overlay
  mBgOverlay.setSize(
      sf::Vector2f(static_cast<float>(Win_W), static_cast<float>(Win_H)));
  mBgOverlay.setFillColor(sf::Color(10, 12, 20, 255));

  // Panoramic Glass Window Area (1040x600 centered)
  float glassW = 1040.f;
  float glassH = 600.f;
  float glassX = (Win_W - glassW) / 2.f;
  float glassY = (Win_H - glassH) / 2.f + 10.f;

  mElevatorGlassWindow.setSize(sf::Vector2f(glassW, glassH));
  mElevatorGlassWindow.setPosition(glassX, glassY);
  mElevatorGlassWindow.setFillColor(
      sf::Color(15, 18, 30, 0)); // Trong suốt để thấy background gốc

  // Top LED Panel Indicator (Pure English)
  mLedPanel.setSize(sf::Vector2f(440.f, 46.f));
  mLedPanel.setPosition((Win_W - 440.f) / 2.f, 12.f);
  mLedPanel.setFillColor(sf::Color(12, 14, 22));
  mLedPanel.setOutlineColor(sf::Color(255, 215, 0));
  mLedPanel.setOutlineThickness(3.f);

  mLedText.setFont(mFont);
  mLedText.setCharacterSize(22);
  mLedText.setFillColor(sf::Color(255, 220, 100));

  mFlashOverlay.setSize(
      sf::Vector2f(static_cast<float>(Win_W), static_cast<float>(Win_H)));
  mFlashOverlay.setPosition(0.f, 0.f);
  mFlashOverlay.setFillColor(sf::Color(255, 255, 255, 0));

  mSkipButton.setup("assets/ui/menu/btn_yes.png", "SKIP [ENTER]", mFont,
                    static_cast<float>(Win_W) - 110.f,
                    static_cast<float>(Win_H) - 45.f,
                    "assets/ui/menu/btn_yes_hover.png", 18);
}

void ElevatorCutscene::start(int fromLevel, int toLevel) {
  mFromLevel = std::max(1, std::min(fromLevel, Max_Level));
  mToLevel = std::max(1, std::min(toLevel, Max_Level));

  mPhase = ElevatorPhase::DOOR_CLOSING;
  mPhaseTimer = 0.f;
  mDoorProgress = 0.f;
  mScrollOffset = static_cast<float>(mFromLevel - 1);
  mFlashAlpha = 0.f;
  mIsMorphed = false;

  if (mSound) {
    mSound->stopLevelClear();
    mSound->stopVictory();
    mSound->stopAllEffects();
    mSound->stopMusic();
    mSound->resetDucking();
    mSound->playElevatorDoor();
  }

  LevelConfig oldCfg = getLevel(mFromLevel);
  LevelConfig newCfg = getLevel(mToLevel);

  float playerCenterX = Win_W / 2.f;
  float playerCenterY = Win_H / 2.f + 140.f;

  if (mOldPlayerTexture.loadFromFile(oldCfg.playerSpritePath)) {
    mOldPlayerSprite.setTexture(mOldPlayerTexture);
    mOldPlayerSprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
    mOldPlayerSprite.setOrigin(32.f, 32.f);
    mOldPlayerSprite.setScale(3.0f, 3.0f);
    mOldPlayerSprite.setPosition(playerCenterX, playerCenterY);
  }

  if (mNewPlayerTexture.loadFromFile(newCfg.playerSpritePath)) {
    mNewPlayerSprite.setTexture(mNewPlayerTexture);
    mNewPlayerSprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
    mNewPlayerSprite.setOrigin(32.f, 32.f);
    mNewPlayerSprite.setScale(3.0f, 3.0f);
    mNewPlayerSprite.setPosition(playerCenterX, playerCenterY);
  }

  updateLedDisplay();
}

void ElevatorCutscene::skip() {
  if (mPhase == ElevatorPhase::FINISHED || mPhase == ElevatorPhase::IDLE)
    return;

  if (mSound) {
    mSound->stopElevatorMove();
    mSound->stopAllEffects();
  }

  mIsMorphed = true;
  mPhase = ElevatorPhase::FINISHED;
}

bool ElevatorCutscene::isSkipButtonClicked(sf::Vector2f mousePos) const {
  if (mPhase == ElevatorPhase::FINISHED || mPhase == ElevatorPhase::IDLE)
    return false;

  return mSkipButton.contains(mousePos);
}

void ElevatorCutscene::updateLedDisplay() {
  int displayLevel =
      (mPhase == ElevatorPhase::DOOR_CLOSING) ? mFromLevel : mToLevel;
  int idx = std::max(
      0, std::min(displayLevel - 1, static_cast<int>(mFloors.size()) - 1));

  mLedText.setString(mFloors[idx].name);
  sf::FloatRect b = mLedText.getLocalBounds();
  mLedText.setOrigin(b.left + b.width / 2.f, b.top + b.height / 2.f);
  mLedText.setPosition(Win_W / 2.f, mLedPanel.getPosition().y + 21.f);
}

void ElevatorCutscene::update(float dt, sf::Vector2f mousePos) {
  if (mPhase == ElevatorPhase::IDLE || mPhase == ElevatorPhase::FINISHED)
    return;

  mSkipButton.update(mousePos, dt);
  mPhaseTimer += dt;

  switch (mPhase) {
  case ElevatorPhase::DOOR_CLOSING: {
    // Khớp 2.47s của elevator_door.ogg
    float duration = 2.47f;
    mDoorProgress = std::min(1.f, mPhaseTimer / duration);
    mShakeOffset = sf::Vector2f(0.f, 0.f);
    if (mPhaseTimer >= duration) {
      mPhase = ElevatorPhase::MOVING;
      mPhaseTimer = 0.f;
      if (mSound)
        mSound->playElevatorMove();
    }
    break;
  }
  case ElevatorPhase::MOVING: {
    float duration = 6.38f;
    float progress = std::min(1.f, mPhaseTimer / duration);
    float smoothProgress = progress * progress * (3.f - 2.f * progress);

    mScrollOffset =
        static_cast<float>(mFromLevel - 1) +
        (static_cast<float>(mToLevel - mFromLevel)) * smoothProgress;

    float shakeIntensity =
        4.5f * std::sin(progress *
                        3.14159f); // Rung nảy nhịp nhàng theo thời gian chạy
    float sx = std::sin(mPhaseTimer * 45.f) * shakeIntensity;
    float sy = std::cos(mPhaseTimer * 35.f) * (shakeIntensity * 0.6f);
    mShakeOffset = sf::Vector2f(sx, sy);

    updateLedDisplay();

    if (mPhaseTimer >= duration) {
      mPhase = ElevatorPhase::MORPHING;
      mPhaseTimer = 0.f;
      mShakeOffset = sf::Vector2f(0.f, 0.f);
      if (mSound) {
        mSound->stopElevatorMove();
        mSound->playElevatorDing();
      }
    }
    break;
  }
  case ElevatorPhase::MORPHING: {
    float duration = 1.08f;
    float halfDuration = duration / 2.f;
    mShakeOffset = sf::Vector2f(0.f, 0.f);

    if (mPhaseTimer < halfDuration) {
      mFlashAlpha = (mPhaseTimer / halfDuration) * 255.f;
    } else {
      mFlashAlpha = (1.f - (mPhaseTimer - halfDuration) / halfDuration) * 255.f;
      mIsMorphed = true;
    }

    mFlashOverlay.setFillColor(
        sf::Color(255, 255, 255,
                  static_cast<sf::Uint8>(std::clamp(mFlashAlpha, 0.f, 255.f))));

    if (mPhaseTimer >= duration) {
      mFlashAlpha = 0.f;
      mFlashOverlay.setFillColor(sf::Color(255, 255, 255, 0));
      mPhase = ElevatorPhase::DOOR_OPENING;
      mPhaseTimer = 0.f;
      if (mSound)
        mSound->playElevatorDoor();
    }
    break;
  }
  case ElevatorPhase::DOOR_OPENING: {
    float duration = 2.47f;
    mDoorProgress = 1.f - std::min(1.f, mPhaseTimer / duration);
    mShakeOffset = sf::Vector2f(0.f, 0.f);
    if (mPhaseTimer >= duration) {
      mPhase = ElevatorPhase::FINISHED;
    }
    break;
  }
  default:
    break;
  }
}

void ElevatorCutscene::drawPixelElevatorFrame(sf::RenderWindow &window,
                                              float glassX, float glassY,
                                              float glassW, float glassH) {
  if (mFramePixelTexture.getSize().x > 0) {
    sf::Sprite frameSprite(mFramePixelTexture);
    frameSprite.setPosition(mShakeOffset.x, mShakeOffset.y);
    window.draw(frameSprite);
  }
}

void ElevatorCutscene::drawPixelDoors(sf::RenderWindow &window, float glassX,
                                      float glassY, float glassW,
                                      float glassH) {
  if (mDoorPixelTexture.getSize().x > 0) {
    float halfW = Win_W / 2.f; // 640px
    float currentOffset = halfW * mDoorProgress;

    sf::Sprite doorLeft(mDoorPixelTexture);
    doorLeft.setPosition(-halfW + currentOffset + mShakeOffset.x,
                         mShakeOffset.y);
    window.draw(doorLeft);

    sf::Sprite doorRight(mDoorPixelTexture);
    doorRight.setScale(-1.f, 1.f);
    doorRight.setPosition(Win_W + (halfW - currentOffset) + mShakeOffset.x,
                          mShakeOffset.y);
    window.draw(doorRight);
  }
}

void ElevatorCutscene::render(sf::RenderWindow &window) {
  if (mPhase == ElevatorPhase::IDLE)
    return;

  window.draw(mBgOverlay);

  float panelH = static_cast<float>(Win_H);
  for (int i = 0; i < static_cast<int>(mFloors.size()); ++i) {
    float panelY = (i - mScrollOffset) * panelH;

    if (panelY + panelH >= 0 && panelY <= static_cast<float>(Win_H)) {
      if (mEraCardSprites[i].getTexture()) {
        mEraCardSprites[i].setPosition(mShakeOffset.x, panelY + mShakeOffset.y);
        window.draw(mEraCardSprites[i]);
      }
    }
  }

  sf::ConvexShape glassStreak1;
  glassStreak1.setPointCount(4);
  glassStreak1.setPoint(0,
                        sf::Vector2f(220.f + mShakeOffset.x, mShakeOffset.y));
  glassStreak1.setPoint(1,
                        sf::Vector2f(340.f + mShakeOffset.x, mShakeOffset.y));
  glassStreak1.setPoint(
      2, sf::Vector2f(140.f + mShakeOffset.x,
                      static_cast<float>(Win_H) + mShakeOffset.y));
  glassStreak1.setPoint(
      3, sf::Vector2f(20.f + mShakeOffset.x,
                      static_cast<float>(Win_H) + mShakeOffset.y));
  glassStreak1.setFillColor(sf::Color(255, 255, 255, 22));
  window.draw(glassStreak1);

  sf::ConvexShape glassStreak2;
  glassStreak2.setPointCount(4);
  glassStreak2.setPoint(0,
                        sf::Vector2f(380.f + mShakeOffset.x, mShakeOffset.y));
  glassStreak2.setPoint(1,
                        sf::Vector2f(440.f + mShakeOffset.x, mShakeOffset.y));
  glassStreak2.setPoint(
      2, sf::Vector2f(240.f + mShakeOffset.x,
                      static_cast<float>(Win_H) + mShakeOffset.y));
  glassStreak2.setPoint(
      3, sf::Vector2f(180.f + mShakeOffset.x,
                      static_cast<float>(Win_H) + mShakeOffset.y));
  glassStreak2.setFillColor(sf::Color(255, 255, 255, 14));
  window.draw(glassStreak2);

  if (!mIsMorphed) {
    mOldPlayerSprite.setPosition(Win_W / 2.f + mShakeOffset.x,
                                 Win_H / 2.f + 140.f + mShakeOffset.y);
    window.draw(mOldPlayerSprite);
  } else {
    mNewPlayerSprite.setPosition(Win_W / 2.f + mShakeOffset.x,
                                 Win_H / 2.f + 140.f + mShakeOffset.y);
    window.draw(mNewPlayerSprite);
  }

  // 5. Flash Overlay khi Morphing biến đổi
  if (mFlashAlpha > 0.f) {
    window.draw(mFlashOverlay);
  }

  drawPixelDoors(window, 0.f, 0.f, static_cast<float>(Win_W),
                 static_cast<float>(Win_H));

  drawPixelElevatorFrame(window, 0.f, 0.f, static_cast<float>(Win_W),
                         static_cast<float>(Win_H));

  mLedPanel.setPosition((Win_W - 440.f) / 2.f + mShakeOffset.x,
                        12.f + mShakeOffset.y);
  mLedText.setPosition(Win_W / 2.f + mShakeOffset.x,
                       mLedPanel.getPosition().y + 21.f);
  window.draw(mLedPanel);
  window.draw(mLedText);

  // 8. Popup Skip Button UI
  mSkipButton.draw(window);
}

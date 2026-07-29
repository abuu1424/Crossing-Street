#pragma once
#include "CPEOPLE.h"
#include "EntityManager.h"
#include "HUD.h"
#include "LevelConfig.h"
#include "Menu.h"
#include "SaveData.h"
#include "SoundManager.h"
#include "Utils.h"
#include <SFML/Graphics.hpp>
#include <vector>

class CGAME {
  sf::RenderWindow &mWindow;

  // Background
  sf::Texture mBgTexture;
  sf::Sprite mBgSprite;

  // Menu
  bool mInMenu = true;
  Menu mMenu;

  // Player
  CPEOPLE mPlayer;

  EntityManager mEntities;

  // Trạng thái
  bool mLevelCleared = false;
  int mCurrentLevel = 1;

  // HUD
  HUD mHUD;
  int mScore = 0;
  float mlevelTime = 0.f;
  // Helpers
  void loadLevel(int level);
  void clearEntities();
  void handleCollision();
  void checkFinish();
  void setupUI();
  void centerText(sf::Text &text);
  void setupLevelClearOptions();
  void setupSaveSlotOptions();
  void updatePauseSliders(sf::Vector2f mouse);

  // Bảng DEAD
  sf::Font mFont;
  sf::Text mDeadText;
  sf::RectangleShape mDeadBox;
  // Bảng VICTORY
  sf::RectangleShape mVictoryBox;
  sf::Text mVictoryTitle;
  sf::Text mVictorySubText;
  sf::Text mVictoryScore;
  sf::Text mVictoryHighScore;

  // Bảng nhập tên file save
  bool mEnteringSaveName = false;
  std::string mCurrentSaveName;
  int mSaveSlotPending = 0;
  int mActiveSlot = 0;

  sf::RectangleShape mSaveBox;
  sf::Text mSaveTitle;
  sf::Text mSaveInput;

  // Bảng Exit
  bool mShowQuitConfirm = false;
  sf::RectangleShape mQuitBox;
  sf::Text mQuitTitle;
  sf::Text mYesText;
  sf::Text mNoText;
  bool mYesHovered = false;
  bool mNoHovered = false;

  // Bảng Menu Confirm
  bool mShowMenuConfirm = false;
  sf::Text mMenuConfirmTitle;

  // Bảng Level Clear
  bool mShowLevelClear = false;
  sf::RectangleShape mLevelClearBox;
  sf::Text mLevelClearTitle;
  sf::Text mLevelClearScore;
  sf::Text mOpt1Text; // Next Level
  sf::Text mOpt2Text; // Save
  sf::Text mOpt3Text; // Save & Exit
  sf::Text mOpt4Text; // Exit
  bool mPendingSaveAndExit = false;
  bool mSelectingSaveSlot = false;

  // Nút Pause
  bool mPaused = false;
  sf::RectangleShape mPauseBox;
  sf::Text mPauseTitle;
  sf::Text mResumeText;
  sf::Text mMenuFromPauseText;
  sf::Text mQuitFromPauseText;
  sf::Text mPauseSettingsTitle;
  float mPauseMusicVol = 50.f;
  float mPauseSFXVol = 50.f;
  sf::RectangleShape mPauseMusicTrack;
  sf::RectangleShape mPauseMusicThumb;
  sf::Text mPauseMusicLabel;
  sf::Text mPauseMusicVal;

  sf::RectangleShape mPauseSFXTrack;
  sf::RectangleShape mPauseSFXThumb;
  sf::Text mPauseSFXLabel;
  sf::Text mPauseSFXVal;

  bool mDraggingMusicSlider = false;
  bool mDraggingSFXSlider = false;

  // Sound (nhạc nền + 3 sound effect, xem SoundManager.h)
  SoundManager mSound;

  // Popup UI Panel Textures & Sprites
  sf::Texture mTexturePopupLevelClear;
  sf::Sprite mSpritePopupLevelClear;
  sf::Texture mTexturePopupGameOver;
  sf::Sprite mSpritePopupGameOver;
  sf::Texture mTexturePopupPause;
  sf::Sprite mSpritePopupPause;
  sf::Texture mTexturePopupQuitConfirm;
  sf::Sprite mSpritePopupQuitConfirm;

  sf::Clock mResetCooldownClock;

public:
  explicit CGAME(sf::RenderWindow &window);
  ~CGAME();

  void run(); // vòng lặp chính
  void handleEvents();
  void update(float dt);
  void render();
  void reset();
  void restartLevel();

  // Save game
  void saveGame(int slot);
  bool loadGame(int slot);
};
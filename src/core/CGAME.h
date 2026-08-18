#pragma once
#include "CPEOPLE.h"
#include "CollisionEffect.h"
#include "DeathCutscene.h"
#include "ElevatorCutscene.h"
#include "EntityManager.h"
#include "HUD.h"
#include "LevelConfig.h"
#include "Menu.h"
#include "SaveData.h"
#include "SoundManager.h"
#include "Utils.h"
#include "HazardManager.h"
#include "CoinManager.h"
#include "PowerUpManager.h"
#include "BotAI.h"
#include <SFML/Graphics.hpp>
#include <memory>
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

  // VS BOT Mode State
  bool mIsVsBotMode = false;
  BotDifficulty mSelectedBotDifficulty = BotDifficulty::NORMAL;
  BotAI mBotAI;
  CPEOPLE mBotPlayer;

  void startVsBotGame(BotDifficulty diff);
  void restartCurrentMode();

  EntityManager mEntities;
  HazardManager mHazardManager;
  CoinManager mCoinManager;
  PowerUpManager mPowerUpManager;

  // Endless Mode State
  bool mIsEndlessMode = false;
  int mEndlessWave = 1;
  int mEndlessHighScore = 0;

  void startEndlessGame();
  void loadEndlessHighScore();
  void saveEndlessHighScore();

  // Collision Effects
  std::string mCollisionSpritePath;
  std::vector<std::unique_ptr<CollisionEffect>> mEffects;

  // Cutscenes
  ElevatorCutscene mCutscene;
  bool mInCutscene = false;

  DeathCutscene mDeathCutscene;

  // Trạng thái
  bool mIsDying = false;
  bool mLevelCleared = false;
  int mCurrentLevel = 1;

  // HUD
  HUD mHUD;
  int mScore = 0;
  int mLevelStartScore = 0;
  float mlevelTime = 0.f;
  // Helpers
  void handleResize(unsigned int width, unsigned int height);
  void toggleFullscreen();
  bool mIsFullscreen = false;
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
  sf::Text mDeadSubText;
  sf::Text mDeadScore;
  sf::RectangleShape mDeadBox;
  MenuButton mBtnDeadRevive;
  MenuButton mBtnDeadRestart;
  MenuButton mBtnDeadMenu;
  // Bảng VICTORY
  sf::RectangleShape mVictoryBox;
  sf::Text mVictoryTitle;
  sf::Text mVictorySubText;
  sf::Text mVictoryStarsText;
  sf::Text mVictoryScore;
  sf::Text mVictoryHighScore;
  MenuButton mBtnVictoryPlayAgain;
  MenuButton mBtnVictoryMenu;

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
  MenuButton mBtnYes;
  MenuButton mBtnNo;

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
  sf::Text mOptShopText; // Shop
  bool mShowShopInGame = false;
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

  // Sound
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
  bool mDebugHitbox = false;

  // Developer Command Console (']' Key)
  bool mShowDevConsole = false;
  bool mGodMode = false;
  std::string mDevInputString;
  std::string mDevFeedbackMsg;
  float mDevFeedbackTimer = 0.f;
  sf::RectangleShape mDevConsoleBox;
  sf::Text mDevConsoleText;
  sf::Text mDevFeedbackText;

  bool handleDevConsoleEvent(const sf::Event& event);
  void executeDevCommand(const std::string& cmd);
  void renderDevConsole();


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
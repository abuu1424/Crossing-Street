#include "CGAME.h"
#include "HighScore.h"
#include "LevelConfig.h"
#include "ShopData.h"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <sstream>

const float SPAWN_X = Win_W / 2.f - Player_W / 2.f;
const float SPAWN_Y = Win_H - Player_H;

CGAME::CGAME(sf::RenderWindow &window)
    : mWindow(window), mScore(0), mLevelStartScore(0), mlevelTime(0.f) {
  sf::View view(sf::FloatRect(0.f, 0.f, (float)Win_W, (float)Win_H));
  mWindow.setView(view);
  setupUI();
}

CGAME::~CGAME() { clearEntities(); }

void CGAME::setupUI() {
  mFont.loadFromFile(Font_Path);
  mCutscene.init(mFont, &mSound);
  mHazardManager.init(mFont, &mSound);

  // Sound
  mSound.loadEffects("assets/sounds/victory/vt1.ogg",
                     "assets/sounds/dead/dead.ogg",
                     "assets/sounds/victory/level_clear.ogg");

  mSound.loadElevatorSounds("assets/sounds/elevator/elevator_door.ogg",
                            "assets/sounds/elevator/elevator_move.ogg",
                            "assets/sounds/elevator/elevator_ding.ogg");

  mSound.loadLevelDeathSounds();
  mSound.loadHazardSounds();

  // Bảng GAME OVER (DEAD) Cao cấp
  float boxW = 540.f, boxH = 340.f;
  mDeadBox.setSize(sf::Vector2f(boxW, boxH));
  mDeadBox.setFillColor(sf::Color(25, 12, 18, 235));
  mDeadBox.setOutlineColor(sf::Color(255, 60, 60));
  mDeadBox.setOutlineThickness(3.5f);
  mDeadBox.setOrigin(boxW / 2.f, boxH / 2.f);
  mDeadBox.setPosition(Win_W / 2.f, Win_H / 2.f);

  mDeadText.setFont(mFont);
  mDeadText.setString("GAME OVER");
  mDeadText.setCharacterSize(48);
  mDeadText.setFillColor(sf::Color(255, 70, 70));
  mDeadText.setOutlineColor(sf::Color(20, 0, 0));
  mDeadText.setOutlineThickness(2.f);
  sf::FloatRect db = mDeadText.getLocalBounds();
  mDeadText.setOrigin(db.left + db.width / 2.f, db.top + db.height / 2.f);
  mDeadText.setPosition(Win_W / 2.f, Win_H / 2.f - 105.f);

  mDeadSubText.setFont(mFont);
  mDeadSubText.setString("Failed to cross the street!");
  mDeadSubText.setCharacterSize(22);
  mDeadSubText.setFillColor(sf::Color(220, 200, 200));
  sf::FloatRect dsb = mDeadSubText.getLocalBounds();
  mDeadSubText.setOrigin(dsb.left + dsb.width / 2.f,
                         dsb.top + dsb.height / 2.f);
  mDeadSubText.setPosition(Win_W / 2.f, Win_H / 2.f - 55.f);

  mDeadScore.setFont(mFont);
  mDeadScore.setString("SCORE: 0");
  mDeadScore.setCharacterSize(24);
  mDeadScore.setFillColor(sf::Color(255, 215, 0));

  mBtnDeadRestart.setup("assets/ui/menu/btn_yes.png", "RESTART", mFont,
                        Win_W / 2.f - 130.f, Win_H / 2.f + 85.f,
                        "assets/ui/menu/btn_yes_hover.png", 18);

  mBtnDeadMenu.setup("assets/ui/menu/btn_yes.png", "MAIN MENU", mFont,
                     Win_W / 2.f + 130.f, Win_H / 2.f + 85.f,
                     "assets/ui/menu/btn_yes_hover.png", 18);

  float vboxW = 620.f, vboxH = 400.f;
  mVictoryBox.setSize(sf::Vector2f(vboxW, vboxH));
  mVictoryBox.setFillColor(sf::Color(10, 14, 24, 235));
  mVictoryBox.setOutlineColor(sf::Color(0, 210, 255, 230));
  mVictoryBox.setOutlineThickness(3.5f);
  mVictoryBox.setOrigin(vboxW / 2.f, vboxH / 2.f);
  mVictoryBox.setPosition(Win_W / 2.f, Win_H / 2.f);

  mVictoryTitle.setFont(mFont);
  mVictoryTitle.setString("VICTORY!");
  mVictoryTitle.setCharacterSize(50);
  mVictoryTitle.setFillColor(sf::Color(255, 215, 0)); // Vàng sáng neon
  mVictoryTitle.setOutlineColor(sf::Color(20, 10, 0));
  mVictoryTitle.setOutlineThickness(2.f);
  sf::FloatRect vt = mVictoryTitle.getLocalBounds();
  mVictoryTitle.setOrigin(vt.left + vt.width / 2.f, vt.top + vt.height / 2.f);
  mVictoryTitle.setPosition(Win_W / 2.f, Win_H / 2.f - 140.f);

  mVictorySubText.setFont(mFont);
  mVictorySubText.setString("You Escaped Through Time!");
  mVictorySubText.setCharacterSize(20);
  mVictorySubText.setFillColor(sf::Color(180, 225, 255)); // Cyan / Trắng sáng
  sf::FloatRect vs = mVictorySubText.getLocalBounds();
  mVictorySubText.setOrigin(vs.left + vs.width / 2.f, vs.top + vs.height / 2.f);
  mVictorySubText.setPosition(Win_W / 2.f, Win_H / 2.f - 90.f);

  mVictoryStarsText.setFont(mFont);
  mVictoryStarsText.setCharacterSize(22);
  mVictoryStarsText.setFillColor(sf::Color(255, 220, 50));

  mVictoryScore.setFont(mFont);
  mVictoryScore.setString("FINAL SCORE: 0");
  mVictoryScore.setCharacterSize(24);
  mVictoryScore.setFillColor(sf::Color(0, 230, 255));
  sf::FloatRect sc = mVictoryScore.getLocalBounds();
  mVictoryScore.setOrigin(sc.left + sc.width / 2.f, sc.top + sc.height / 2.f);
  mVictoryScore.setPosition(Win_W / 2.f, Win_H / 2.f + 5.f);

  mVictoryHighScore.setFont(mFont);
  mVictoryHighScore.setString("HIGH SCORE: 0");
  mVictoryHighScore.setCharacterSize(24);
  mVictoryHighScore.setFillColor(sf::Color(255, 215, 0));
  sf::FloatRect hs = mVictoryHighScore.getLocalBounds();
  mVictoryHighScore.setOrigin(hs.left + hs.width / 2.f,
                              hs.top + hs.height / 2.f);
  mVictoryHighScore.setPosition(Win_W / 2.f, Win_H / 2.f + 48.f);

  // Hai nút nằm ngang cùng kích thước, cùng thiết kế chữ nhật crimson red pixel
  // art
  mBtnVictoryPlayAgain.setup("assets/ui/menu/btn_yes.png", "PLAY AGAIN", mFont,
                             Win_W / 2.f - 135.f, Win_H / 2.f + 130.f,
                             "assets/ui/menu/btn_yes_hover.png", 18);

  mBtnVictoryMenu.setup("assets/ui/menu/btn_yes.png", "MAIN MENU", mFont,
                        Win_W / 2.f + 135.f, Win_H / 2.f + 130.f,
                        "assets/ui/menu/btn_yes_hover.png", 18);

  // Bảng nhập tên save
  float sboxW = 450.f, sboxH = 180.f;
  mSaveBox.setSize(sf::Vector2f(sboxW, sboxH));
  mSaveBox.setFillColor(sf::Color(20, 20, 30, 230));
  mSaveBox.setOutlineColor(sf::Color(100, 200, 255));
  mSaveBox.setOutlineThickness(2.f);
  mSaveBox.setOrigin(sboxW / 2.f, sboxH / 2.f);
  mSaveBox.setPosition(Win_W / 2.f, Win_H / 2.f);

  mSaveTitle.setFont(mFont);
  mSaveTitle.setString("Enter save name:");
  mSaveTitle.setCharacterSize(24);
  mSaveTitle.setFillColor(sf::Color::White);
  sf::FloatRect stb = mSaveTitle.getLocalBounds();
  mSaveTitle.setOrigin(stb.left + stb.width / 2.f, stb.top + stb.height / 2.f);
  mSaveTitle.setPosition(Win_W / 2.f, Win_H / 2.f - 40.f);

  mSaveInput.setFont(mFont);
  mSaveInput.setCharacterSize(28);
  mSaveInput.setFillColor(sf::Color(255, 215, 0));
  mSaveInput.setPosition(Win_W / 2.f - 180.f, Win_H / 2.f);

  // Bảng Quit
  float qboxW = 400.f, qboxH = 180.f;
  mQuitBox.setSize(sf::Vector2f(qboxW, qboxH));
  mQuitBox.setFillColor(sf::Color(20, 20, 30, 230));
  mQuitBox.setOutlineColor(sf::Color(255, 100, 100));
  mQuitBox.setOutlineThickness(2.f);
  mQuitBox.setOrigin(qboxW / 2.f, qboxH / 2.f);
  mQuitBox.setPosition(Win_W / 2.f, Win_H / 2.f);

  mQuitTitle.setFont(mFont);
  mQuitTitle.setString("Quit the game?");
  mQuitTitle.setCharacterSize(28);
  mQuitTitle.setFillColor(sf::Color::White);
  mQuitTitle.setOutlineColor(sf::Color(20, 15, 10, 230));
  mQuitTitle.setOutlineThickness(1.5f);
  sf::FloatRect qt = mQuitTitle.getLocalBounds();
  mQuitTitle.setOrigin(qt.left + qt.width / 2.f, qt.top + qt.height / 2.f);
  mQuitTitle.setPosition(Win_W / 2.f, Win_H / 2.f - 40.f);

  mBtnYes.setup("assets/ui/menu/btn_yes.png", "YES", mFont, Win_W / 2.f - 85.f,
                Win_H / 2.f + 35.f, "assets/ui/menu/btn_yes_hover.png");

  mBtnNo.setup("assets/ui/menu/btn_no.png", "NO", mFont, Win_W / 2.f + 85.f,
               Win_H / 2.f + 35.f, "assets/ui/menu/btn_no_hover.png");

  // Bảng Menu Confirm
  mMenuConfirmTitle.setFont(mFont);
  mMenuConfirmTitle.setString("Return to Menu?");
  mMenuConfirmTitle.setCharacterSize(28);
  mMenuConfirmTitle.setFillColor(sf::Color::White);
  mMenuConfirmTitle.setOutlineColor(sf::Color(20, 15, 10, 230));
  mMenuConfirmTitle.setOutlineThickness(1.5f);
  sf::FloatRect mt = mMenuConfirmTitle.getLocalBounds();
  mMenuConfirmTitle.setOrigin(mt.left + mt.width / 2.f,
                              mt.top + mt.height / 2.f);
  mMenuConfirmTitle.setPosition(Win_W / 2.f, Win_H / 2.f - 40.f);

  // Bảng Pause
  // Bảng Pause — dùng size lớn hơn để chứa slider
  float pboxW = 450.f, pboxH = 340.f;
  mPauseBox.setSize(sf::Vector2f(pboxW, pboxH));
  mPauseBox.setFillColor(sf::Color(20, 20, 30, 230));
  mPauseBox.setOutlineColor(sf::Color(180, 140, 90));
  mPauseBox.setOutlineThickness(3.f);
  mPauseBox.setOrigin(pboxW / 2.f, pboxH / 2.f);
  mPauseBox.setPosition(Win_W / 2.f, Win_H / 2.f);

  mPauseTitle.setFont(mFont);
  mPauseTitle.setString("PAUSED");
  mPauseTitle.setCharacterSize(36);
  mPauseTitle.setFillColor(sf::Color(255, 215, 0));
  {
    sf::FloatRect pt = mPauseTitle.getLocalBounds();
    mPauseTitle.setOrigin(pt.left + pt.width / 2.f, pt.top + pt.height / 2.f);
    mPauseTitle.setPosition(Win_W / 2.f, Win_H / 2.f - 140.f);
  }

  mResumeText.setFont(mFont);
  mResumeText.setString("Press P or click to RESUME");
  mResumeText.setCharacterSize(20);
  mResumeText.setFillColor(sf::Color(150, 255, 150));
  {
    sf::FloatRect rt = mResumeText.getLocalBounds();
    mResumeText.setOrigin(rt.left + rt.width / 2.f, rt.top + rt.height / 2.f);
    mResumeText.setPosition(Win_W / 2.f, Win_H / 2.f - 95.f);
  }

  mMenuFromPauseText.setFont(mFont);
  mMenuFromPauseText.setString("Press M for MENU");
  mMenuFromPauseText.setCharacterSize(20);
  mMenuFromPauseText.setFillColor(sf::Color(255, 255, 150));
  {
    sf::FloatRect mft = mMenuFromPauseText.getLocalBounds();
    mMenuFromPauseText.setOrigin(mft.left + mft.width / 2.f,
                                 mft.top + mft.height / 2.f);
    mMenuFromPauseText.setPosition(Win_W / 2.f, Win_H / 2.f - 55.f);
  }

  mQuitFromPauseText.setFont(mFont);
  mQuitFromPauseText.setString("Press ESC to QUIT");
  mQuitFromPauseText.setCharacterSize(20);
  mQuitFromPauseText.setFillColor(sf::Color(255, 150, 150));
  {
    sf::FloatRect qft = mQuitFromPauseText.getLocalBounds();
    mQuitFromPauseText.setOrigin(qft.left + qft.width / 2.f,
                                 qft.top + qft.height / 2.f);
    mQuitFromPauseText.setPosition(Win_W / 2.f, Win_H / 2.f - 15.f);
  }

  // Slider Music
  mPauseMusicLabel.setFont(mFont);
  mPauseMusicLabel.setString("Music Volume");
  mPauseMusicLabel.setCharacterSize(18);
  mPauseMusicLabel.setFillColor(sf::Color::White);
  mPauseMusicLabel.setPosition(Win_W / 2.f - 150.f, Win_H / 2.f + 20.f);

  mPauseMusicTrack.setSize(sf::Vector2f(300.f, 6.f));
  mPauseMusicTrack.setFillColor(sf::Color(80, 80, 80));
  mPauseMusicTrack.setPosition(Win_W / 2.f - 150.f, Win_H / 2.f + 50.f);

  mPauseMusicThumb.setSize(sf::Vector2f(16.f, 24.f));
  mPauseMusicThumb.setFillColor(sf::Color(255, 215, 0));
  mPauseMusicThumb.setOrigin(8.f, 12.f);

  mPauseMusicVal.setFont(mFont);
  mPauseMusicVal.setCharacterSize(16);
  mPauseMusicVal.setFillColor(sf::Color(200, 200, 200));

  // Slider SFX
  mPauseSFXLabel.setFont(mFont);
  mPauseSFXLabel.setString("SFX Volume");
  mPauseSFXLabel.setCharacterSize(18);
  mPauseSFXLabel.setFillColor(sf::Color::White);
  mPauseSFXLabel.setPosition(Win_W / 2.f - 150.f, Win_H / 2.f + 100.f);

  mPauseSFXTrack.setSize(sf::Vector2f(300.f, 6.f));
  mPauseSFXTrack.setFillColor(sf::Color(80, 80, 80));
  mPauseSFXTrack.setPosition(Win_W / 2.f - 150.f, Win_H / 2.f + 130.f);

  mPauseSFXThumb.setSize(sf::Vector2f(16.f, 24.f));
  mPauseSFXThumb.setFillColor(sf::Color(255, 215, 0));
  mPauseSFXThumb.setOrigin(8.f, 12.f);

  mPauseSFXVal.setFont(mFont);
  mPauseSFXVal.setCharacterSize(16);
  mPauseSFXVal.setFillColor(sf::Color(200, 200, 200));

  // Bảng Level Clear
  float lcW = 500.f, lcH = 300.f;
  mLevelClearBox.setSize(sf::Vector2f(lcW, lcH));
  mLevelClearBox.setFillColor(sf::Color(20, 20, 30, 230));
  mLevelClearBox.setOutlineColor(sf::Color(255, 215, 0));
  mLevelClearBox.setOutlineThickness(3.f);
  mLevelClearBox.setOrigin(lcW / 2.f, lcH / 2.f);
  mLevelClearBox.setPosition(Win_W / 2.f, Win_H / 2.f);

  mLevelClearTitle.setFont(mFont);
  mLevelClearTitle.setCharacterSize(32);
  mLevelClearTitle.setFillColor(sf::Color(255, 215, 0));
  {
    sf::FloatRect b = mLevelClearTitle.getLocalBounds();
    mLevelClearTitle.setOrigin(b.left + b.width / 2.f, b.top + b.height / 2.f);
    mLevelClearTitle.setPosition(Win_W / 2.f, Win_H / 2.f - 110.f);
  }

  mLevelClearScore.setFont(mFont);
  mLevelClearScore.setCharacterSize(20);
  mLevelClearScore.setFillColor(sf::Color::White);
  mLevelClearScore.setPosition(Win_W / 2.f, Win_H / 2.f - 65.f);

  // 4 lựa chọn
  auto setupOpt = [&](sf::Text &t, const std::string &str, float y) {
    t.setFont(mFont);
    t.setString(str);
    t.setCharacterSize(20);
    t.setFillColor(sf::Color(150, 220, 255));
    sf::FloatRect b = t.getLocalBounds();
    t.setOrigin(b.left + b.width / 2.f, b.top + b.height / 2.f);
    t.setPosition(Win_W / 2.f, y);
  };

  setupOpt(mOpt1Text, "[1]  Next Level", Win_H / 2.f - 25.f);
  setupOpt(mOpt2Text, "[2]  Save", Win_H / 2.f + 20.f);
  setupOpt(mOpt3Text, "[3]  Save & Exit", Win_H / 2.f + 65.f);
  setupOpt(mOpt4Text, "[4]  Exit", Win_H / 2.f + 110.f);
  setupOpt(mOptShopText, "[S]  ITEM SHOP", Win_H / 2.f + 155.f);

  // Tải các bảng Popup từ assets/ui/hud/
  if (mTexturePopupLevelClear.loadFromFile(
          "assets/ui/hud/popup_level_clear.png")) {
    mSpritePopupLevelClear.setTexture(mTexturePopupLevelClear);
    sf::Vector2u sz = mTexturePopupLevelClear.getSize();
    mSpritePopupLevelClear.setOrigin(sz.x / 2.f, sz.y / 2.f);
    mSpritePopupLevelClear.setScale(550.f / sz.x, 550.f / sz.y);
    mSpritePopupLevelClear.setPosition(Win_W / 2.f, Win_H / 2.f);
  }

  if (mTexturePopupGameOver.loadFromFile("assets/ui/hud/popup_game_over.png")) {
    mSpritePopupGameOver.setTexture(mTexturePopupGameOver);
    sf::Vector2u sz = mTexturePopupGameOver.getSize();
    mSpritePopupGameOver.setOrigin(sz.x / 2.f, sz.y / 2.f);
    mSpritePopupGameOver.setScale(540.f / sz.x, 540.f / sz.y);
    mSpritePopupGameOver.setPosition(Win_W / 2.f, Win_H / 2.f);
  }

  if (mTexturePopupPause.loadFromFile("assets/ui/hud/popup_pause.png")) {
    mSpritePopupPause.setTexture(mTexturePopupPause);
    sf::Vector2u sz = mTexturePopupPause.getSize();
    mSpritePopupPause.setOrigin(sz.x / 2.f, sz.y / 2.f);
    mSpritePopupPause.setScale(580.f / sz.x, 520.f / sz.y);
    mSpritePopupPause.setPosition(Win_W / 2.f, Win_H / 2.f);
  }

  if (mTexturePopupQuitConfirm.loadFromFile(
          "assets/ui/hud/quit_confirm_panel.png")) {
    mSpritePopupQuitConfirm.setTexture(mTexturePopupQuitConfirm);
    sf::Vector2u sz = mTexturePopupQuitConfirm.getSize();
    mSpritePopupQuitConfirm.setOrigin(sz.x / 2.f, sz.y / 2.f);
    mSpritePopupQuitConfirm.setScale(480.f / sz.x, 240.f / sz.y);
    mSpritePopupQuitConfirm.setPosition(Win_W / 2.f, Win_H / 2.f);
  }
}

void CGAME::centerText(sf::Text &text) {
  sf::FloatRect b = text.getLocalBounds();
  text.setOrigin(b.left + b.width / 2.f, b.top + b.height / 2.f);
}

void CGAME::setupLevelClearOptions() {
  if (mCurrentLevel == 5) {
    mOpt1Text.setString("[1]  See Victory Screen");
  } else {
    mOpt1Text.setString("[1]  Next Level");
  }

  mOpt2Text.setString("[2]  Save");
  mOpt3Text.setString("[3]  Save & Exit");
  mOpt4Text.setString("[4]  Exit");
  mOptShopText.setString("[S]  ITEM SHOP");

  centerText(mOpt1Text);
  centerText(mOpt2Text);
  centerText(mOpt3Text);
  centerText(mOpt4Text);
  centerText(mOptShopText);
}

void CGAME::setupSaveSlotOptions() {
  mLevelClearTitle.setString("SELECT SAVE SLOT");
  centerText(mLevelClearTitle);

  mOpt1Text.setString("[1]  Slot 1");
  mOpt2Text.setString("[2]  Slot 2");
  mOpt3Text.setString("[3]  Slot 3");
  mOpt4Text.setString("[ESC]  Back");

  centerText(mOpt1Text);
  centerText(mOpt2Text);
  centerText(mOpt3Text);
  centerText(mOpt4Text);
}

sf::FloatRect shrinkBoxPercent(sf::FloatRect r, float percentX,
                               float percentY) {
  float dx = r.width * percentX;
  float dy = r.height * percentY;
  r.left += dx;
  r.top += dy;
  r.width -= dx * 2.f;
  r.height -= dy * 2.f;
  return r;
}

sf::FloatRect shrinkBoxPercent(sf::FloatRect r, float percent) {
  return shrinkBoxPercent(r, percent, percent);
}

void CGAME::clearEntities() {
  mEntities.clear();
  mEffects.clear();
}

void CGAME::loadLevel(int level) {
  clearEntities();
  LevelConfig cfg = getLevel(level);

  mDeathCutscene.reset();
  mIsDying = false;
  mLevelCleared = false;
  mCurrentLevel = cfg.level;
  mlevelTime = 0.f;
  mCollisionSpritePath = cfg.collisionSpritePath;

  // Background
  if (!mBgTexture.loadFromFile(cfg.backgroundPath))
    printf("FAILED background\n");
  mBgSprite.setTexture(mBgTexture);
  mBgSprite.setScale(static_cast<float>(Win_W) / mBgTexture.getSize().x,
                     static_cast<float>(Win_H) / mBgTexture.getSize().y);

  mSound.playLevelMusic(cfg.musicPath, 40.f);

  // Player - Speed tuned per level + Speed Boots upgrade (+15% speed if
  // purchased)
  float levelPlayerSpeed = std::max(120.f, 155.f - (cfg.level - 1) * 7.f);
  if (ShopData::isItemPurchased("speed")) {
    levelPlayerSpeed *= 1.15f;
  }
  mPlayer.reloadSprite(cfg.playerSpritePath);
  mPlayer.setSpeed(levelPlayerSpeed);
  mPlayer.setDead(false);
  mPlayer.setFinish(false);
  mPlayer.setPosition(SPAWN_X, SPAWN_Y);

  // HUD
  mHUD.reloadHudBar(cfg.hudBarPath);
  mHUD.update(mCurrentLevel, mScore, mlevelTime);

  // Spawn obstacle/animal/traffic light — xem EntityManager::spawnFromLevel
  mEntities.spawnFromLevel(cfg);
  mHazardManager.startLevel(cfg.level);
  mCoinManager.spawnForLevel(cfg.level);
}

void CGAME::reset() {
  mSound.stopAllEffects();
  mSound.stopMusic();

  mDeathCutscene.reset();
  mScore = 0;
  mLevelStartScore = 0;
  mlevelTime = 0.f;
  mIsDying = false;
  mLevelCleared = false;
  mShowLevelClear = false;
  mShowMenuConfirm = false;
  mSelectingSaveSlot = false;
  mEnteringSaveName = false;

  mPlayer.resetStats();
  mPlayer.setDead(false);
  mPlayer.setFinish(false);
  mPlayer.setPosition(SPAWN_X, SPAWN_Y);

  loadLevel(1);
  mHUD.update(mCurrentLevel, mScore, mlevelTime);
}

void CGAME::restartLevel() {
  mSound.stopAllEffects();
  mSound.stopLevelClear();

  mlevelTime = 0.f;
  mShowLevelClear = false;
  mShowMenuConfirm = false;
  mSelectingSaveSlot = false;
  mEnteringSaveName = false;

  mPlayer.resetStats();
  mPlayer.setDead(false);
  mPlayer.setFinish(false);
  mPlayer.setPosition(SPAWN_X, SPAWN_Y);

  mScore = mLevelStartScore;
  loadLevel(mCurrentLevel);
  mHUD.update(mCurrentLevel, mScore, mlevelTime);
}

void CGAME::handleEvents() {
  sf::Event event;

  while (mWindow.pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      mWindow.close();
      continue;
    }

    if (mShowShopInGame) {
      mMenu.setScreen(MenuScreen::SHOP);
      MenuResult res = MenuResult::NONE;
      mMenu.handleShopEvent(event, mWindow, res);
      if (mMenu.getScreen() != MenuScreen::SHOP) {
        mShowShopInGame = false;
        mMenu.setScreen(MenuScreen::MAIN);
      }
      continue;
    }

    if (event.type == sf::Event::Resized) {
      float windowRatio = (float)event.size.width / (float)event.size.height;
      float viewRatio = (float)Win_W / (float)Win_H;
      sf::View view(sf::FloatRect(0.f, 0.f, (float)Win_W, (float)Win_H));
      float sizeX = 1.f;
      float sizeY = 1.f;
      float posX = 0.f;
      float posY = 0.f;

      if (windowRatio >= viewRatio) {
        sizeX = viewRatio / windowRatio;
        posX = (1.f - sizeX) / 2.f;
      } else {
        sizeY = windowRatio / viewRatio;
        posY = (1.f - sizeY) / 2.f;
      }

      view.setViewport(sf::FloatRect(posX, posY, sizeX, sizeY));
      mWindow.setView(view);
      continue;
    }

    if (handleDevConsoleEvent(event)) {
      continue;
    }

    if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::V) {
      mDebugHitbox = !mDebugHitbox;
      printf("Debug Hitbox %s\n", mDebugHitbox ? "ON" : "OFF");
    }

    sf::Vector2f mouse;
    if (event.type == sf::Event::MouseButtonPressed) {
      mouse = mWindow.mapPixelToCoords(
          sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
    }

    // Cutscene Thang Máy
    if (mInCutscene) {
      if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Enter ||
            event.key.code == sf::Keyboard::Return ||
            event.key.code == sf::Keyboard::Space) {
          mCutscene.skip();
        }
      }

      if (event.type == sf::Event::MouseButtonPressed &&
          event.mouseButton.button == sf::Mouse::Left) {
        if (mCutscene.isSkipButtonClicked(mouse)) {
          mCutscene.skip();
        }
      }

      continue;
    }

    // Bảng QUIT confirm
    if (mShowQuitConfirm) {
      if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape ||
            event.key.code == sf::Keyboard::N) {
          mShowQuitConfirm = false;
        } else if (event.key.code == sf::Keyboard::Y) {
          mWindow.close();
        }
      }

      if (event.type == sf::Event::MouseButtonPressed &&
          event.mouseButton.button == sf::Mouse::Left) {

        if (mBtnYes.contains(mouse)) {
          mWindow.close();
        } else if (mBtnNo.contains(mouse)) {
          mShowQuitConfirm = false;
        }
      }

      continue;
    }

    // Bảng MENU confirm
    if (mShowMenuConfirm) {
      if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape ||
            event.key.code == sf::Keyboard::N) {
          mShowMenuConfirm = false;
        } else if (event.key.code == sf::Keyboard::Y) {
          mShowMenuConfirm = false;
          mSound.stopAllEffects();
          mSound.stopMusic();
          mPaused = false;
          mInMenu = true;
        }
      }

      if (event.type == sf::Event::MouseButtonPressed &&
          event.mouseButton.button == sf::Mouse::Left) {

        if (mBtnYes.contains(mouse)) {
          mShowMenuConfirm = false;
          mSound.stopAllEffects();
          mSound.stopMusic();
          mPaused = false;
          mInMenu = true;
        } else if (mBtnNo.contains(mouse)) {
          mShowMenuConfirm = false;
        }
      }

      continue;
    }

    // Bảng GAME OVER (DEAD)
    if (mPlayer.isDead()) {
      if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::R ||
            event.key.code == sf::Keyboard::Enter ||
            event.key.code == sf::Keyboard::Space) {
          if (mResetCooldownClock.getElapsedTime().asSeconds() >= 0.35f) {
            mResetCooldownClock.restart();
            reset();
          }
        } else if (event.key.code == sf::Keyboard::M) {
          mSound.stopAllEffects();
          mSound.stopMusic();
          mPaused = false;
          mInMenu = true;
        } else if (event.key.code == sf::Keyboard::Escape) {
          mShowQuitConfirm = true;
        }
      }

      if (event.type == sf::Event::MouseButtonPressed &&
          event.mouseButton.button == sf::Mouse::Left) {
        if (mBtnDeadRestart.contains(mouse)) {
          if (mResetCooldownClock.getElapsedTime().asSeconds() >= 0.35f) {
            mResetCooldownClock.restart();
            reset();
          }
        } else if (mBtnDeadMenu.contains(mouse)) {
          mSound.stopAllEffects();
          mSound.stopMusic();
          mPaused = false;
          mInMenu = true;
        }
      }

      continue;
    }

    // Bảng VICTORY
    if (mPlayer.isFinish() && mCurrentLevel == Max_Level && !mShowLevelClear) {
      if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::R ||
            event.key.code == sf::Keyboard::Enter ||
            event.key.code == sf::Keyboard::Space) {
          if (mResetCooldownClock.getElapsedTime().asSeconds() >= 0.35f) {
            mResetCooldownClock.restart();
            reset();
          }
        } else if (event.key.code == sf::Keyboard::M) {
          mSound.stopAllEffects();
          mSound.stopMusic();
          mPaused = false;
          mInMenu = true;
        } else if (event.key.code == sf::Keyboard::Escape) {
          mShowQuitConfirm = true;
        }
      }

      if (event.type == sf::Event::MouseButtonPressed &&
          event.mouseButton.button == sf::Mouse::Left) {
        if (mBtnVictoryPlayAgain.contains(mouse)) {
          if (mResetCooldownClock.getElapsedTime().asSeconds() >= 0.35f) {
            mResetCooldownClock.restart();
            reset();
          }
        } else if (mBtnVictoryMenu.contains(mouse)) {
          mSound.stopAllEffects();
          mSound.stopMusic();
          mPaused = false;
          mInMenu = true;
        }
      }

      continue;
    }

    // Bảng PAUSE
    if (mPaused) {
      if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::P) {
          mPaused = false;
        } else if (event.key.code == sf::Keyboard::M) {
          mPaused = false;
          mShowMenuConfirm = true;
        } else if (event.key.code == sf::Keyboard::Escape) {
          mPaused = false;
          mShowQuitConfirm = true;
        }
      }

      if (event.type == sf::Event::MouseButtonPressed &&
          event.mouseButton.button == sf::Mouse::Left) {
        if (mResumeText.getGlobalBounds().contains(mouse)) {
          mPaused = false;
        } else if (mMenuFromPauseText.getGlobalBounds().contains(mouse)) {
          mPaused = false;
          mShowMenuConfirm = true;
        } else if (mQuitFromPauseText.getGlobalBounds().contains(mouse)) {
          mPaused = false;
          mShowQuitConfirm = true;
        } else if (mHUD.getPauseIconBounds().contains(mouse)) {
          mPaused = false;
        } else if (mPauseMusicThumb.getGlobalBounds().contains(mouse) ||
                   mPauseMusicTrack.getGlobalBounds().contains(mouse)) {
          mDraggingMusicSlider = true;
          updatePauseSliders(mouse);
        } else if (mPauseSFXThumb.getGlobalBounds().contains(mouse) ||
                   mPauseSFXTrack.getGlobalBounds().contains(mouse)) {
          mDraggingSFXSlider = true;
          updatePauseSliders(mouse);
        }
      }
      if (event.type == sf::Event::MouseButtonReleased) {
        mDraggingMusicSlider = false;
        mDraggingSFXSlider = false;
      }

      if (event.type == sf::Event::MouseMoved) {
        sf::Vector2f m = mWindow.mapPixelToCoords(
            sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
        updatePauseSliders(m);
      }
      continue;
    }

    // Bảng chọn save slot
    if (mSelectingSaveSlot) {
      if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Num1)
          mSaveSlotPending = 1;
        else if (event.key.code == sf::Keyboard::Num2)
          mSaveSlotPending = 2;
        else if (event.key.code == sf::Keyboard::Num3)
          mSaveSlotPending = 3;
        else if (event.key.code == sf::Keyboard::Escape) {
          mSelectingSaveSlot = false;
          setupLevelClearOptions();
          mShowLevelClear = true;
          mSaveSlotPending = 0;
          continue;
        } else if (event.key.code == sf::Keyboard::R) {
          if (mPlayer.isFinish() && mCurrentLevel == Max_Level) {
            reset();
          } else {
            restartLevel();
          }
          continue;
        }

        if (mSaveSlotPending != 0) {
          if (SaveData::hasData(mSaveSlotPending)) {
            auto slots = SaveData::getAllSlots();

            mCurrentSaveName =
                slots[mSaveSlotPending - 1].saveName.empty()
                    ? "Save Slot " + std::to_string(mSaveSlotPending)
                    : slots[mSaveSlotPending - 1].saveName;

            saveGame(mSaveSlotPending);

            mSelectingSaveSlot = false;
            mSaveSlotPending = 0;
            mCurrentSaveName.clear();

            if (mPendingSaveAndExit) {
              mPendingSaveAndExit = false;
              mWindow.close();
            } else {
              setupLevelClearOptions();
              mShowLevelClear = true;
            }
          } else {
            mSelectingSaveSlot = false;
            mEnteringSaveName = true;
            mCurrentSaveName.clear();
          }
        }
      }
      if (event.type == sf::Event::MouseButtonPressed &&
          event.mouseButton.button == sf::Mouse::Left) {

        int clickedSlot = 0;
        if (mOpt1Text.getGlobalBounds().contains(mouse))
          clickedSlot = 1;
        else if (mOpt2Text.getGlobalBounds().contains(mouse))
          clickedSlot = 2;
        else if (mOpt3Text.getGlobalBounds().contains(mouse))
          clickedSlot = 3;

        if (clickedSlot != 0) {
          if (SaveData::hasData(clickedSlot)) {
            auto slots = SaveData::getAllSlots();

            mCurrentSaveName = slots[clickedSlot - 1].saveName.empty()
                                   ? "Save Slot " + std::to_string(clickedSlot)
                                   : slots[clickedSlot - 1].saveName;

            saveGame(clickedSlot);

            mSelectingSaveSlot = false;
            mSaveSlotPending = 0;
            mCurrentSaveName.clear();

            if (mPendingSaveAndExit) {
              mPendingSaveAndExit = false;
              mWindow.close();
            } else {
              setupLevelClearOptions();
              mShowLevelClear = true;
            }
          } else {
            mSelectingSaveSlot = false;
            mEnteringSaveName = true;
            mSaveSlotPending = clickedSlot;
            mCurrentSaveName.clear();
          }
        } else if (mOpt4Text.getGlobalBounds().contains(mouse)) {
          mSelectingSaveSlot = false;
          setupLevelClearOptions();
          mShowLevelClear = true;
          mSaveSlotPending = 0;
        }
      }

      continue;
    }

    // Level Clear
    if (mShowLevelClear) {
      if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Num1) {
          mShowLevelClear = false;
          mSound.stopLevelClear();
          mPlayer.setFinish(false);

          if (mCurrentLevel < Max_Level) {
            mLevelStartScore = mScore;
            mInCutscene = true;
            mCutscene.start(mCurrentLevel, mCurrentLevel + 1);
          } else {
            mShowLevelClear = false;
            mPlayer.setFinish(true);
            mSound.stopLevelClear();
            mSound.playVictory();
          }
        } else if (event.key.code == sf::Keyboard::Num2) {
          // Save: mở bảng chọn slot
          mShowLevelClear = false;
          mSelectingSaveSlot = true;
          mPendingSaveAndExit = false;
          mSaveSlotPending = 0;
          mCurrentSaveName.clear();
        } else if (event.key.code == sf::Keyboard::Num3) {
          mShowLevelClear = false;
          mSelectingSaveSlot = true;
          mPendingSaveAndExit = true;
          mSaveSlotPending = 0;
          mCurrentSaveName.clear();
        } else if (event.key.code == sf::Keyboard::Num4) {
          mWindow.close();
        } else if (event.key.code == sf::Keyboard::S) {
          mShowShopInGame = true;
        } else if (event.key.code == sf::Keyboard::R) {
          if (mResetCooldownClock.getElapsedTime().asSeconds() >= 0.35f) {
            mResetCooldownClock.restart();
            if (mPlayer.isFinish() && mCurrentLevel == Max_Level) {
              reset();
            } else {
              restartLevel();
            }
          }
        }
      }

      if (event.type == sf::Event::MouseButtonPressed &&
          event.mouseButton.button == sf::Mouse::Left) {

        sf::Vector2f mouse = mWindow.mapPixelToCoords(
            sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

        if (mOpt1Text.getGlobalBounds().contains(mouse)) {
          mShowLevelClear = false;
          mSound.stopLevelClear();
          mPlayer.setFinish(false);

          if (mCurrentLevel < Max_Level) {
            mLevelStartScore = mScore;
            mInCutscene = true;
            mCutscene.start(mCurrentLevel, mCurrentLevel + 1);
          } else {
            mPlayer.setFinish(true);
            mSound.playVictory();
          }
        } else if (mOpt2Text.getGlobalBounds().contains(mouse)) {
          // Save: mở bảng chọn slot
          mShowLevelClear = false;
          mSelectingSaveSlot = true;
          mPendingSaveAndExit = false;
          mSaveSlotPending = 0;
          mCurrentSaveName.clear();
        } else if (mOpt3Text.getGlobalBounds().contains(mouse)) {
          // Save & Exit: mở bảng chọn slot
          mShowLevelClear = false;
          mSelectingSaveSlot = true;
          mPendingSaveAndExit = true;
          mSaveSlotPending = 0;
          mCurrentSaveName.clear();
        } else if (mOpt4Text.getGlobalBounds().contains(mouse)) {
          mWindow.close();
        } else if (mOptShopText.getGlobalBounds().contains(mouse)) {
          mShowShopInGame = true;
        }
      }

      continue;
    }

    // Đang nhập tên save
    if (mEnteringSaveName) {
      if (event.type == sf::Event::TextEntered) {
        if (event.text.unicode >= 32 && event.text.unicode < 128 &&
            mCurrentSaveName.size() < 20) {
          mCurrentSaveName += static_cast<char>(event.text.unicode);
        }
      }

      if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::BackSpace &&
            !mCurrentSaveName.empty()) {
          mCurrentSaveName.pop_back();
        } else if (event.key.code == sf::Keyboard::Enter) {
          if (mCurrentSaveName.empty()) {
            mCurrentSaveName = "Save Slot " + std::to_string(mSaveSlotPending);
          }

          saveGame(mSaveSlotPending);

          mEnteringSaveName = false;
          mSaveSlotPending = 0;
          mCurrentSaveName.clear();

          if (mPendingSaveAndExit) {
            mPendingSaveAndExit = false;
            mWindow.close();
          } else {
            setupLevelClearOptions();
            mShowLevelClear = true;
          }
        } else if (event.key.code == sf::Keyboard::Escape) {
          mEnteringSaveName = false;
          mSaveSlotPending = 0;
          mCurrentSaveName.clear();
          setupLevelClearOptions();
          mShowLevelClear = true;
        }
      }

      continue;
    }

    // Click icon pause trên HUD
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left) {

      if (mHUD.getPauseIconBounds().contains(mouse)) {
        mPaused = true;
        mPauseMusicVol = mMenu.getMusicVolume();
        mPauseSFXVol = mMenu.getSFXVolume();
        continue;
      }
    }

    // Bình thường
    if (event.type == sf::Event::KeyPressed) {
      if (event.key.code == sf::Keyboard::Escape) {
        mShowQuitConfirm = true;
      } else if (event.key.code == sf::Keyboard::M) {
        mShowMenuConfirm = true;
      } else if (event.key.code == sf::Keyboard::P) {
        mPaused = true;
        mPauseMusicVol = mMenu.getMusicVolume();
        mPauseSFXVol = mMenu.getSFXVolume();
      } else if (event.key.code == sf::Keyboard::F1) {
        if (SaveData::hasData(1)) {
          auto slots = SaveData::getAllSlots();
          mCurrentSaveName =
              slots[0].saveName.empty() ? "Save Slot 1" : slots[0].saveName;
          saveGame(1);
          mCurrentSaveName.clear();
        } else {
          mEnteringSaveName = true;
          mSaveSlotPending = 1;
          mCurrentSaveName.clear();
        }
      } else if (event.key.code == sf::Keyboard::F2) {
        if (SaveData::hasData(2)) {
          auto slots = SaveData::getAllSlots();
          mCurrentSaveName =
              slots[1].saveName.empty() ? "Save Slot 2" : slots[1].saveName;
          saveGame(2);
          mCurrentSaveName.clear();
        } else {
          mEnteringSaveName = true;
          mSaveSlotPending = 2;
          mCurrentSaveName.clear();
        }
      } else if (event.key.code == sf::Keyboard::F3) {
        if (SaveData::hasData(3)) {
          auto slots = SaveData::getAllSlots();
          mCurrentSaveName =
              slots[2].saveName.empty() ? "Save Slot 3" : slots[2].saveName;
          saveGame(3);
          mCurrentSaveName.clear();
        } else {
          mEnteringSaveName = true;
          mSaveSlotPending = 3;
          mCurrentSaveName.clear();
        }
      } else if (event.key.code == sf::Keyboard::F4) {
        loadGame(1);
      } else if (event.key.code == sf::Keyboard::F5) {
        loadGame(2);
      } else if (event.key.code == sf::Keyboard::F6) {
        loadGame(3);
      }
    }
  }
}

void CGAME::handleCollision() {
  if (mPlayer.isDead() || mPlayer.isFinish() || mIsDying ||
      mDeathCutscene.isActive())
    return;

  sf::FloatRect pb = mPlayer.getHitbox();

  auto triggerHit = [&](sf::Vector2f hitPos) {
    if (mPlayer.isInvulnerable()) {
      return;
    }
    if (ShopData::isItemPurchased("shield")) {
      ShopData::consumeShield();
      mPlayer.triggerInvulnerability(1.5f);
      mPlayer.knockback(160.f);
      printf("Energy Shield absorbed hit at (%.1f, %.1f)!\n", hitPos.x,
             hitPos.y);
      mEffects.push_back(std::make_unique<CollisionEffect>(hitPos));
      return;
    }

    bool fatalHit = mPlayer.takeDamage(1);

    if (fatalHit) {
      mSound.stopMusic();
      mSound.stopHazardSounds();
      mHazardManager.reset();
      mSound.playLevelDeathSound(mCurrentLevel);
      mDeathCutscene.start(hitPos, mCurrentLevel);
    } else {
      mEffects.push_back(std::make_unique<CollisionEffect>(hitPos));
      mSound.playLevelDeathSound(mCurrentLevel);
    }
  };

  for (const auto &obs : mEntities.obstacles()) {
    sf::FloatRect ob = obs->getHitbox();

    if (pb.intersects(ob)) {
      sf::Vector2f hitPos(pb.left + pb.width / 2.f, pb.top + pb.height / 2.f);
      triggerHit(hitPos);
      return;
    }
  }

  for (const auto &ani : mEntities.animals()) {
    sf::FloatRect ab = ani->getHitbox();

    if (pb.intersects(ab)) {
      sf::Vector2f hitPos(pb.left + pb.width / 2.f, pb.top + pb.height / 2.f);
      triggerHit(hitPos);
      return;
    }
  }
}

void CGAME::checkFinish() {
  if (mPlayer.isDead() || mPlayer.isFinish() || mLevelCleared)
    return;

  if (mPlayer.getPosition().y < 80.f) {
    mLevelCleared = true;

    // Tính score
    float effectiveTimeLimit = Level_Time_Limit + 15.f * ShopData::getItemCount("time");
    float timeRemaining = effectiveTimeLimit - mlevelTime;
    if (timeRemaining < 0.f)
      timeRemaining = 0.f;
    int baseScore = 100 * mCurrentLevel;
    int timeBonus = static_cast<int>(timeRemaining) * 10;
    mScore = mLevelStartScore + baseScore + timeBonus;
    HighScore::updateIfHigher(mScore);
    printf("Level %d clear! +%d (base=%d, bonus=%d)\n", mCurrentLevel,
           baseScore + timeBonus, baseScore, timeBonus);

    // Cập nhật text bảng Level Clear
    mLevelClearTitle.setString("Level " + std::to_string(mCurrentLevel) +
                               " Complete!");
    {
      sf::FloatRect b = mLevelClearTitle.getLocalBounds();
      mLevelClearTitle.setOrigin(b.left + b.width / 2.f,
                                 b.top + b.height / 2.f);
    }

    mLevelClearScore.setString("+" + std::to_string(baseScore) + " base  +  " +
                               std::to_string(timeBonus) + " time bonus  =  " +
                               std::to_string(mScore) + " total");
    {
      sf::FloatRect b = mLevelClearScore.getLocalBounds();
      mLevelClearScore.setOrigin(b.left + b.width / 2.f,
                                 b.top + b.height / 2.f);
      mLevelClearScore.setPosition(Win_W / 2.f, Win_H / 2.f - 65.f);
    }

    setupLevelClearOptions();

    mSound.stopMusic();
    mSound.stopHazardSounds();
    mHazardManager.reset();
    mPlayer.setFinish(true);

    if (mCurrentLevel < Max_Level) {
      mSound.playLevelClear();
      mShowLevelClear = true;
    } else {
      mSound.playVictory();
      bool isNewHighScore = HighScore::updateIfHigher(mScore);
      if (isNewHighScore)
        printf("NEW HIGH SCORE: %d\n", mScore);
    }
  }
}

void CGAME::update(float dt) {
  if (mDevFeedbackTimer > 0.f) {
    mDevFeedbackTimer -= dt;
    if (mDevFeedbackTimer < 0.f)
      mDevFeedbackTimer = 0.f;
  }

  if (mGodMode) {
    mPlayer.triggerInvulnerability(999999.f);
  }

  for (auto it = mEffects.begin(); it != mEffects.end();) {

    (*it)->update(dt);
    if ((*it)->isFinished()) {
      it = mEffects.erase(it);
    } else {
      ++it;
    }
  }

  if (mIsDying) {
    if (mEffects.empty()) {
      mIsDying = false;
      mPlayer.setDead(true);
      mSound.stopMusic();
      mSound.stopHazardSounds();
      mHazardManager.reset();
      HighScore::updateIfHigher(mScore);
      printf("DEAD\n");
    }
    return;
  }

  sf::Vector2f mousePos =
      mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow));

  if (mInCutscene) {
    mCutscene.update(dt, mousePos);
    if (mCutscene.isFinished()) {
      mInCutscene = false;
      int nextLvl = mCutscene.getTargetLevel();
      loadLevel(nextLvl);
      mHUD.update(mCurrentLevel, mScore, mlevelTime);
      if (mActiveSlot > 0)
        saveGame(mActiveSlot);
    }
    return;
  }

  if (mShowQuitConfirm || mShowMenuConfirm) {
    mBtnYes.update(mousePos, dt);
    mBtnNo.update(mousePos, dt);
    return;
  }

  if (mPlayer.isDead()) {
    mBtnDeadRestart.update(mousePos, dt);
    mBtnDeadMenu.update(mousePos, dt);
    return;
  }

  if (mPlayer.isFinish() && mCurrentLevel == Max_Level && !mShowLevelClear) {
    mBtnVictoryPlayAgain.update(mousePos, dt);
    mBtnVictoryMenu.update(mousePos, dt);
    return;
  }

  auto updateHover = [](sf::Text &t, sf::Vector2f m, sf::Color normalColor) {
    if (t.getGlobalBounds().contains(m)) {
      t.setFillColor(sf::Color(255, 215, 0));
      t.setScale(1.1f, 1.1f);
    } else {
      t.setFillColor(normalColor);
      t.setScale(1.0f, 1.0f);
    }
  };

  if (mPaused) {
    updateHover(mResumeText, mousePos, sf::Color(150, 255, 150));
    updateHover(mMenuFromPauseText, mousePos, sf::Color(220, 220, 220));
    updateHover(mQuitFromPauseText, mousePos, sf::Color(255, 150, 150));
    return;
  }

  if (mShowShopInGame) {
    return;
  }

  if (mShowLevelClear) {
    updateHover(mOpt1Text, mousePos, sf::Color::White);
    updateHover(mOpt2Text, mousePos, sf::Color::White);
    updateHover(mOpt3Text, mousePos, sf::Color::White);
    updateHover(mOpt4Text, mousePos, sf::Color::White);
    updateHover(mOptShopText, mousePos, sf::Color(255, 215, 0));
    return;
  }

  if (mEnteringSaveName)
    return;

  if (mDeathCutscene.isActive()) {
    mDeathCutscene.update(dt);
    if (mDeathCutscene.isFinished()) {
      mSound.stopMusic();
      mSound.stopHazardSounds();
      mSound.stopLevelDeathSounds();
      mHazardManager.reset();
      // Khi còn máu thì chơi lại màn hiện tại
      if (mPlayer.getStats().currentHp > 0) {
        mlevelTime = 0.f;
        mIsDying = false;
        mPlayer.setDead(false);
        mPlayer.setFinish(false);
        mPlayer.setPosition(SPAWN_X, SPAWN_Y);
        loadLevel(mCurrentLevel);
        mHUD.update(mCurrentLevel, mScore, mlevelTime);
      } else {
        // Hết máu thì từ đầu
        mIsDying = true;
        mPlayer.setDead(true);
        mSound.playDead();
      }
    }
    return;
  }

  if (!mPlayer.isDead() && !mPlayer.isFinish() && !mIsDying &&
      !mDeathCutscene.isActive()) {
    if (!mPlayer.getStats().timeFreezeActive) {
      mlevelTime += dt;
    }
    float effectiveTimeLimit = Level_Time_Limit + 15.f * ShopData::getItemCount("time");
    if (mlevelTime >= effectiveTimeLimit) {
      sf::Vector2f hitPos(mPlayer.getPosition().x + Player_W / 2.f,
                          mPlayer.getPosition().y + Player_H / 2.f);
      mSound.stopMusic();
      mSound.stopHazardSounds();
      mHazardManager.reset();
      mSound.playLevelDeathSound(mCurrentLevel);

      // Trừ 1 tim khi hết thời gian
      mPlayer.takeDamage(1);

      mDeathCutscene.start(hitPos, mCurrentLevel);
      printf("You ran out of time! Deducted 1 HP.\n");
    }
    bool isTimeFrozen = mPlayer.getStats().timeFreezeActive;
    bool hasHazard =
        mHazardManager.isHazardActive() || mHazardManager.isWarningActive();
    mPlayer.Move(dt, mScore, hasHazard);
    mPlayer.update(dt);

    float hazardDt = isTimeFrozen ? 0.0f : dt;

    std::vector<std::pair<sf::FloatRect, float>> extraHazardBoxes;
    mHazardManager.update(hazardDt, mPlayer.getPosition(), extraHazardBoxes, mScore);
    mCoinManager.update(dt, mPlayer.getHitbox(), &mSound);
    mSound.update(dt);

    // Apply wind drift during Sandstorm (only if time is not frozen)
    if (!isTimeFrozen) {
      sf::Vector2f drift = mHazardManager.getPlayerWindDrift();
      if (drift.x != 0.f || drift.y != 0.f) {
        mPlayer.setPosition(mPlayer.getPosition().x + drift.x,
                            mPlayer.getPosition().y + drift.y);
      }
    }

    float speedMult = mHazardManager.getSpeedMultiplier();
    if (isTimeFrozen) {
      speedMult = 0.0f; // 100% Freeze when Time Clock skill is active!
    } else if (mPlayer.getStats().radarActive) {
      speedMult *= 0.5f; // 50% slow-down when EMP Radar Pulse is active
    }
    mEntities.update(dt, speedMult);

    // Check collision with extra hazards (Stampede, Arrows, Lasers)
    sf::FloatRect pb = mPlayer.getHitbox();
    for (const auto &hb : extraHazardBoxes) {
      if (pb.intersects(hb.first)) {
        if (mPlayer.isInvulnerable()) {
          break;
        }
        sf::Vector2f hitPos(pb.left + pb.width / 2.f, pb.top + pb.height / 2.f);
        if (ShopData::isItemPurchased("shield")) {
          ShopData::consumeShield();
          mPlayer.triggerInvulnerability(1.5f);
          mPlayer.knockback(160.f);
          printf("Energy Shield absorbed hazard hit at (%.1f, %.1f)!\n",
                 hitPos.x, hitPos.y);
          mEffects.push_back(std::make_unique<CollisionEffect>(hitPos));
          break;
        }

        bool fatalHit = mPlayer.takeDamage(1);

        if (fatalHit) {
          mSound.stopMusic();
          mSound.stopHazardSounds();
          mHazardManager.reset();
          mSound.playLevelDeathSound(mCurrentLevel);
          mDeathCutscene.start(hitPos, mCurrentLevel);
        } else {
          mEffects.push_back(std::make_unique<CollisionEffect>(hitPos));
          mSound.playLevelDeathSound(mCurrentLevel);
        }
        break;
      }
    }

    handleCollision();
    checkFinish();
  }

  mHUD.update(mCurrentLevel, mScore, mlevelTime);
}

void CGAME::render() {
  mWindow.clear();

  if (mInCutscene) {
    mWindow.draw(mBgSprite);
    mEntities.draw(mWindow);
    mPlayer.Draw(mWindow);
    mHUD.draw(mWindow);
    mCutscene.render(mWindow);
    mWindow.display();
    return;
  }

  if (mDeathCutscene.isActive()) {
    sf::View currentView = mWindow.getView();
    sf::View shakeView = currentView;
    shakeView.move(mDeathCutscene.getShakeOffset());
    mWindow.setView(shakeView);

    mWindow.draw(mBgSprite);
    mEntities.draw(mWindow);

    if (!mDeathCutscene.shouldHidePlayer()) {
      mPlayer.Draw(mWindow);
    }

    mDeathCutscene.render(mWindow);
    mHUD.draw(mWindow);

    mWindow.setView(currentView);
    mWindow.display();
    return;
  }

  sf::View origView = mWindow.getView();
  sf::Vector2f hazardShake = mHazardManager.getShakeOffset();
  if (hazardShake.x != 0.f || hazardShake.y != 0.f) {
    sf::View shakeView = origView;
    shakeView.move(hazardShake);
    mWindow.setView(shakeView);
  }

  mWindow.draw(mBgSprite);

  mEntities.draw(mWindow);
  mCoinManager.draw(mWindow);
  mHazardManager.draw(mWindow);

  mPlayer.Draw(mWindow);

  for (const auto &effect : mEffects) {
    effect->draw(mWindow);
  }

  mWindow.setView(origView);

  // Time Freeze Icy Screen Overlay Tint
  if (mPlayer.getStats().timeFreezeActive) {
    sf::RectangleShape freezeOverlay(
        sf::Vector2f(static_cast<float>(Win_W), static_cast<float>(Win_H)));
    freezeOverlay.setFillColor(sf::Color(80, 200, 255, 45));
    mWindow.draw(freezeOverlay);
  }

  if (mDebugHitbox) {
    auto drawRect = [&](sf::FloatRect r, sf::Color color) {
      sf::RectangleShape rect(sf::Vector2f(r.width, r.height));
      rect.setPosition(r.left, r.top);
      rect.setFillColor(sf::Color::Transparent);
      rect.setOutlineColor(color);
      rect.setOutlineThickness(2.f);
      mWindow.draw(rect);
    };

    drawRect(mPlayer.getHitbox(), sf::Color::Green);
    for (const auto &obs : mEntities.obstacles())
      drawRect(obs->getHitbox(), sf::Color::Red);
    for (const auto &ani : mEntities.animals())
      drawRect(ani->getHitbox(), sf::Color::Yellow);
  }

  mHUD.draw(mWindow);
  mHUD.drawStats(mWindow, mPlayer.getStats());

  mHazardManager.drawUI(mWindow);

  if (mPlayer.isDead()) {
    mWindow.draw(mDeadBox);
    mWindow.draw(mDeadText);
    mWindow.draw(mDeadSubText);

    mDeadScore.setString("SCORE: " + std::to_string(mScore));
    sf::FloatRect ds = mDeadScore.getLocalBounds();
    mDeadScore.setOrigin(ds.left + ds.width / 2.f, ds.top + ds.height / 2.f);
    mDeadScore.setPosition(Win_W / 2.f, Win_H / 2.f - 5.f);
    mWindow.draw(mDeadScore);

    mBtnDeadRestart.draw(mWindow);
    mBtnDeadMenu.draw(mWindow);
  }

  if (mPlayer.isFinish() && mCurrentLevel == Max_Level && !mShowLevelClear) {
    mWindow.draw(mVictoryBox);

    float vboxW = 620.f, vboxH = 400.f;
    sf::RectangleShape innerFrame(sf::Vector2f(vboxW - 16.f, vboxH - 16.f));
    innerFrame.setOrigin((vboxW - 16.f) / 2.f, (vboxH - 16.f) / 2.f);
    innerFrame.setPosition(Win_W / 2.f, Win_H / 2.f);
    innerFrame.setFillColor(sf::Color::Transparent);
    innerFrame.setOutlineColor(sf::Color(0, 180, 230, 120));
    innerFrame.setOutlineThickness(1.5f);
    mWindow.draw(innerFrame);

    sf::RectangleShape topBar(sf::Vector2f(vboxW - 24.f, 3.f));
    topBar.setPosition(Win_W / 2.f - (vboxW - 24.f) / 2.f,
                       Win_H / 2.f - vboxH / 2.f + 4.f);
    topBar.setFillColor(sf::Color(0, 230, 255, 200));
    mWindow.draw(topBar);

    sf::RectangleShape botBar(sf::Vector2f(vboxW - 24.f, 3.f));
    botBar.setPosition(Win_W / 2.f - (vboxW - 24.f) / 2.f,
                       Win_H / 2.f + vboxH / 2.f - 7.f);
    botBar.setFillColor(sf::Color(0, 80, 130, 180));
    mWindow.draw(botBar);

    float cornerSize = 14.f;
    auto drawCorner = [&](float cx, float cy) {
      sf::RectangleShape c1(sf::Vector2f(cornerSize, 3.f));
      c1.setPosition(cx, cy);
      c1.setFillColor(sf::Color(0, 240, 255));
      sf::RectangleShape c2(sf::Vector2f(3.f, cornerSize));
      c2.setPosition(cx, cy);
      c2.setFillColor(sf::Color(0, 240, 255));
      mWindow.draw(c1);
      mWindow.draw(c2);
    };

    float leftX = Win_W / 2.f - vboxW / 2.f - 2.f;
    float rightX = Win_W / 2.f + vboxW / 2.f + 2.f;
    float topY = Win_H / 2.f - vboxH / 2.f - 2.f;
    float botY = Win_H / 2.f + vboxH / 2.f + 2.f;

    drawCorner(leftX, topY);
    drawCorner(rightX - cornerSize, topY);
    drawCorner(leftX, botY - cornerSize);
    drawCorner(rightX - cornerSize, botY - cornerSize);

    // 3. Title & Subtitle (Pure English Pixel Art)
    mVictoryTitle.setString("VICTORY!");
    sf::FloatRect vt = mVictoryTitle.getLocalBounds();
    mVictoryTitle.setOrigin(vt.left + vt.width / 2.f, vt.top + vt.height / 2.f);
    mWindow.draw(mVictoryTitle);

    mVictorySubText.setString("You Escaped Through Time!");
    sf::FloatRect vs = mVictorySubText.getLocalBounds();
    mVictorySubText.setOrigin(vs.left + vs.width / 2.f,
                              vs.top + vs.height / 2.f);
    mWindow.draw(mVictorySubText);

    // 4. Rank Badge
    std::string rankName = "TIME MASTER";
    sf::Color rankColor = sf::Color(255, 215, 0);
    if (mScore < 1000) {
      rankName = "TIME SURVIVOR";
      rankColor = sf::Color(200, 215, 230);
    } else if (mScore < 2000) {
      rankName = "TIME EXPLORER";
      rankColor = sf::Color(255, 230, 100);
    }
    mVictoryStarsText.setString("RANK: " + rankName);
    mVictoryStarsText.setFillColor(rankColor);
    sf::FloatRect st = mVictoryStarsText.getLocalBounds();
    mVictoryStarsText.setOrigin(st.left + st.width / 2.f,
                                st.top + st.height / 2.f);
    mVictoryStarsText.setPosition(Win_W / 2.f, Win_H / 2.f - 42.f);
    mWindow.draw(mVictoryStarsText);

    // 5. Score & High Score
    mVictoryScore.setString("FINAL SCORE: " + std::to_string(mScore));
    sf::FloatRect sc = mVictoryScore.getLocalBounds();
    mVictoryScore.setOrigin(sc.left + sc.width / 2.f, sc.top + sc.height / 2.f);

    int highScore = HighScore::load();
    mVictoryHighScore.setString("HIGH SCORE: " + std::to_string(highScore));
    sf::FloatRect hs = mVictoryHighScore.getLocalBounds();
    mVictoryHighScore.setOrigin(hs.left + hs.width / 2.f,
                                hs.top + hs.height / 2.f);

    mWindow.draw(mVictoryScore);
    mWindow.draw(mVictoryHighScore);

    mBtnVictoryPlayAgain.draw(mWindow);
    mBtnVictoryMenu.draw(mWindow);
  }

  if (mEnteringSaveName) {
    mSaveInput.setString(mCurrentSaveName + "_");

    mWindow.draw(mSaveBox);
    mWindow.draw(mSaveTitle);
    mWindow.draw(mSaveInput);
  }

  if (mShowQuitConfirm) {
    if (mSpritePopupQuitConfirm.getTexture())
      mWindow.draw(mSpritePopupQuitConfirm);
    else
      mWindow.draw(mQuitBox);

    mWindow.draw(mQuitTitle);
    mBtnYes.draw(mWindow);
    mBtnNo.draw(mWindow);
  }

  if (mShowMenuConfirm) {
    if (mSpritePopupQuitConfirm.getTexture())
      mWindow.draw(mSpritePopupQuitConfirm);
    else
      mWindow.draw(mQuitBox);

    mWindow.draw(mMenuConfirmTitle);
    mBtnYes.draw(mWindow);
    mBtnNo.draw(mWindow);
  }

  if (mPaused) {
    if (!mDraggingMusicSlider && !mDraggingSFXSlider)
      updatePauseSliders({});

    if (mSpritePopupPause.getTexture())
      mWindow.draw(mSpritePopupPause);
    else
      mWindow.draw(mPauseBox);

    mWindow.draw(mPauseTitle);
    mWindow.draw(mResumeText);
    mWindow.draw(mMenuFromPauseText);
    mWindow.draw(mQuitFromPauseText);
    // Slider
    mWindow.draw(mPauseMusicLabel);
    mWindow.draw(mPauseMusicTrack);
    mWindow.draw(mPauseMusicThumb);
    mWindow.draw(mPauseMusicVal);

    mWindow.draw(mPauseSFXLabel);
    mWindow.draw(mPauseSFXTrack);
    mWindow.draw(mPauseSFXThumb);
    mWindow.draw(mPauseSFXVal);
  }

  if (mSelectingSaveSlot) {
    setupSaveSlotOptions();

    if (mSpritePopupLevelClear.getTexture())
      mWindow.draw(mSpritePopupLevelClear);
    else
      mWindow.draw(mLevelClearBox);

    mWindow.draw(mLevelClearTitle);
    mWindow.draw(mOpt1Text);
    mWindow.draw(mOpt2Text);
    mWindow.draw(mOpt3Text);
    mWindow.draw(mOpt4Text);
  }

  if (mShowLevelClear) {
    if (mSpritePopupLevelClear.getTexture())
      mWindow.draw(mSpritePopupLevelClear);
    else
      mWindow.draw(mLevelClearBox);

    mWindow.draw(mLevelClearTitle);
    mWindow.draw(mLevelClearScore);
    mWindow.draw(mOpt1Text);
    mWindow.draw(mOpt2Text);
    mWindow.draw(mOpt3Text);
    mWindow.draw(mOpt4Text);
    mWindow.draw(mOptShopText);
  }

  if (mShowShopInGame) {
    mMenu.drawShopMenu(mWindow);
  }

  renderDevConsole();
  mWindow.display();
}

void CGAME::run() {
  sf::Clock clock;
  MenuResult menuResult = MenuResult::NONE;
  while (mWindow.isOpen()) {
    float dt = clock.restart().asSeconds();
    // Menu
    if (mInMenu) {
      sf::Event event;

      while (mWindow.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
          mWindow.close();
        }

        if (handleDevConsoleEvent(event)) {
          continue;
        }

        mMenu.handleEvent(event, mWindow, menuResult);
      }
      // NEW_GAME
      if (menuResult == MenuResult::NEW_GAME_SLOT_1 ||
          menuResult == MenuResult::NEW_GAME_SLOT_2 ||
          menuResult == MenuResult::NEW_GAME_SLOT_3) {
        mActiveSlot = (menuResult == MenuResult::NEW_GAME_SLOT_1)   ? 1
                      : (menuResult == MenuResult::NEW_GAME_SLOT_2) ? 2
                                                                    : 3;
        ShopData::setActiveSlot(mActiveSlot);
        ShopData::resetSlot(mActiveSlot);
        mInMenu = false;
        reset();
        mCurrentSaveName = mMenu.getPendingSaveName();
        saveGame(mActiveSlot);

        menuResult = MenuResult::NONE;

      } else if (menuResult == MenuResult::QUIT) {
        mWindow.close();
      } else if (menuResult == MenuResult::SETTING) {
        menuResult = MenuResult::NONE;
      }
      // LOAD_GAME
      else if (menuResult == MenuResult::LOAD_SLOT_1) {
        if (loadGame(1)) {
          mInMenu = false;
          mActiveSlot = 1;
        }
        menuResult = MenuResult::NONE;
      } else if (menuResult == MenuResult::LOAD_SLOT_2) {
        if (loadGame(2)) {
          mInMenu = false;
          mActiveSlot = 2;
        }
        menuResult = MenuResult::NONE;
      } else if (menuResult == MenuResult::LOAD_SLOT_3) {
        if (loadGame(3)) {
          mInMenu = false;
          mActiveSlot = 3;
        }
        menuResult = MenuResult::NONE;
      }
      mMenu.update(dt, mWindow);
      mWindow.clear();
      mMenu.draw(mWindow);
      renderDevConsole();
      mWindow.display();

      continue;
    }

    // Game
    handleEvents();
    update(dt);
    render();
  }
}

void CGAME::updatePauseSliders(sf::Vector2f mouse) {
  if (mDraggingMusicSlider) {
    float v = (mouse.x - (Win_W / 2.f - 150.f)) / 300.f * 100.f;
    mPauseMusicVol = std::clamp(v, 0.f, 100.f);

    mMenu.setMusicVolume(mPauseMusicVol);
    mSound.setMusicVolume(mPauseMusicVol);
  }

  if (mDraggingSFXSlider) {
    float v = (mouse.x - (Win_W / 2.f - 150.f)) / 300.f * 100.f;
    mPauseSFXVol = std::clamp(v, 0.f, 100.f);

    mMenu.setSFXVolume(mPauseSFXVol);
    mSound.setSFXVolume(mPauseSFXVol);
  }
  // Music
  float musicX = Win_W / 2.f - 150.f + (mPauseMusicVol / 100.f) * 300.f;
  mPauseMusicThumb.setPosition(musicX, Win_H / 2.f + 53.f);
  mPauseMusicVal.setString(std::to_string((int)mPauseMusicVol));
  mPauseMusicVal.setPosition(Win_W / 2.f + 160.f, Win_H / 2.f + 42.f);
  // SFX
  float sfxX = Win_W / 2.f - 150.f + (mPauseSFXVol / 100.f) * 300.f;
  mPauseSFXThumb.setPosition(sfxX, Win_H / 2.f + 133.f);
  mPauseSFXVal.setString(std::to_string((int)mPauseSFXVol));
  mPauseSFXVal.setPosition(Win_W / 2.f + 160.f, Win_H / 2.f + 122.f);
}

// Save Game
void CGAME::saveGame(int slot) {
  sf::Vector2f pos = mPlayer.getPosition();

  std::string name = mCurrentSaveName;
  if (name.empty()) {
    if (SaveData::hasData(slot)) {
      auto slots = SaveData::getAllSlots();
      if (slot >= 1 && slot <= (int)slots.size() &&
          !slots[slot - 1].saveName.empty()) {
        name = slots[slot - 1].saveName;
      }
    }
  }
  if (name.empty()) {
    name = "Save Slot " + std::to_string(slot);
  }
  mCurrentSaveName = name;

  SaveData::save(slot, mCurrentLevel, mScore, pos.x, pos.y, name);
}

bool CGAME::loadGame(int slot) {
  int level = 1, score = 0;
  float playerX = SPAWN_X;
  float playerY = SPAWN_Y;
  std::string saveName;
  if (!SaveData::load(slot, level, score, playerX, playerY, saveName))
    return false;

  mScore = score;
  mLevelStartScore = score;
  mActiveSlot = slot;
  ShopData::setActiveSlot(slot);

  mCurrentSaveName =
      saveName.empty() ? ("Save Slot " + std::to_string(slot)) : saveName;
  loadLevel(level);
  mPlayer.setDead(false);
  mPlayer.setFinish(false);
  mPlayer.setPosition(playerX, playerY);
  mHUD.update(mCurrentLevel, mScore, mlevelTime);
  return true;
}

void CGAME::executeDevCommand(const std::string &rawCmd) {
  std::string cmd = rawCmd;
  std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

  if (cmd.empty())
    return;

  // Convert underscores to spaces
  for (char &c : cmd) {
    if (c == '_')
      c = ' ';
  }

  std::istringstream iss(cmd);
  std::string action;
  iss >> action;

  if (action == "coins" || action == "coin") {
    int val = 1000;
    if (iss >> val) {
    }
    ShopData::addCoins(val);
    mDevFeedbackMsg = "[DEV] Added +" + std::to_string(val) +
                      " Coins to Slot " + std::to_string(mActiveSlot) + "!";
  } else if (action == "hp") {
    int val = 3;
    if (iss >> val) {
    }
    val = std::clamp(val, 1, 10);
    mPlayer.getStats().maxHp = val;
    mPlayer.getStats().currentHp = val;
    mDevFeedbackMsg = "[DEV] Max HP set to " + std::to_string(val) + " Hearts!";
  } else if (action == "heal") {
    mPlayer.getStats().currentHp = mPlayer.getStats().maxHp;
    mPlayer.getStats().energy = mPlayer.getStats().maxEnergy;
    mPlayer.setDead(false);
    mDevFeedbackMsg = "[DEV] Player HP & Energy fully restored!";
  } else if (action == "level" || action == "lvl") {
    int lvl = 1;
    if (iss >> lvl) {
    }
    lvl = std::clamp(lvl, 1, Max_Level);
    mInCutscene = false;
    mShowLevelClear = false;
    mPlayer.setDead(false);
    mPlayer.setFinish(false);
    loadLevel(lvl);
    mDevFeedbackMsg = "[DEV] Teleported to Level " + std::to_string(lvl) + "!";
  } else if (action == "god" || action == "invuln") {
    mGodMode = !mGodMode;
    if (mGodMode) {
      mPlayer.triggerInvulnerability(999999.f);
      mDevFeedbackMsg = "[DEV] GOD MODE: ACTIVATED (Infinite Invulnerability)";
    } else {
      mPlayer.triggerInvulnerability(0.f);
      mDevFeedbackMsg = "[DEV] GOD MODE: DEACTIVATED";
    }
  } else if (action == "skill" || action == "skills" || action == "buy") {
    std::string item;
    if (!(iss >> item))
      item = "all";
    if (item == "all") {
      ShopData::buyItem("shield", 0);
      ShopData::buyItem("speed", 0);
      ShopData::buyItem("time", 0);
      ShopData::buyItem("radar", 0);
      ShopData::buyItem("speed_skill", 0);
      mDevFeedbackMsg = "[DEV] UNLOCKED ALL SHOP ITEMS & SPEED SKILL!";
    } else {
      ShopData::buyItem(item, 0);
      mDevFeedbackMsg = "[DEV] Unlocked item: " + item;
    }
  } else if (action == "nrg" || action == "energy") {
    mPlayer.getStats().energy = mPlayer.getStats().maxEnergy;
    mDevFeedbackMsg = "[DEV] Energy refilled to 100%!";
  } else if (action == "score") {
    int val = 500;
    if (iss >> val) {
    }
    mScore += val;
    mDevFeedbackMsg = "[DEV] Score +" + std::to_string(val) + "!";
  } else if (action == "help") {
    mDevFeedbackMsg = "[DEV] Cmds: coins_N, hp_N, heal, level_N, god, "
                      "skill_all, nrg_100, score_N";
  } else {
    mDevFeedbackMsg =
        "[DEV ERROR] Unknown command '" + rawCmd + "'. Type 'help'";
  }

  mDevFeedbackTimer = 3.5f;
}

bool CGAME::handleDevConsoleEvent(const sf::Event &event) {
  if (event.type == sf::Event::KeyPressed) {
    if (event.key.code == sf::Keyboard::Tilde ||
        event.key.code == sf::Keyboard::F8 ||
        event.key.code == sf::Keyboard::F9 ||
        event.key.code == sf::Keyboard::F12 ||
        event.key.code == sf::Keyboard::RBracket ||
        event.key.code == sf::Keyboard::Slash ||
        event.key.code == sf::Keyboard::Tab) {
      mShowDevConsole = !mShowDevConsole;
      mDevInputString.clear();
      return true;
    }
  }

  if (mShowDevConsole) {
    if (event.type == sf::Event::TextEntered) {
      uint32_t c = event.text.unicode;
      if (c == '`' || c == '~' || c == ']' || c == '[' || c == '/') {
        // Ignore trigger hotkeys
      } else if (c >= 32 && c < 127) {
        if (mDevInputString.size() < 40) {
          mDevInputString += static_cast<char>(c);
        }
      }
    } else if (event.type == sf::Event::KeyPressed) {
      if (event.key.code == sf::Keyboard::BackSpace &&
          !mDevInputString.empty()) {
        mDevInputString.pop_back();
      } else if (event.key.code == sf::Keyboard::Enter ||
                 event.key.code == sf::Keyboard::Return) {
        executeDevCommand(mDevInputString);
        mDevInputString.clear();
        mShowDevConsole = false;
      } else if (event.key.code == sf::Keyboard::Escape) {
        mShowDevConsole = false;
      }
    }
    return true;
  }

  return false;
}

void CGAME::renderDevConsole() {
  sf::View origView = mWindow.getView();
  mWindow.setView(mWindow.getDefaultView());

  if (mDevFeedbackTimer > 0.f && !mDevFeedbackMsg.empty()) {
    mDevFeedbackText.setFont(mFont);
    mDevFeedbackText.setCharacterSize(18);
    mDevFeedbackText.setFillColor(sf::Color(255, 220, 100));
    mDevFeedbackText.setOutlineColor(sf::Color::Black);
    mDevFeedbackText.setOutlineThickness(1.5f);
    mDevFeedbackText.setPosition(15.f, Win_H - 85.f);
    mDevFeedbackText.setString(mDevFeedbackMsg);
    mWindow.draw(mDevFeedbackText);
  }

  if (mShowDevConsole) {
    mDevConsoleBox.setSize(sf::Vector2f(Win_W, 55.f));
    mDevConsoleBox.setPosition(0.f, Win_H - 55.f);
    mDevConsoleBox.setFillColor(sf::Color(10, 18, 30, 245));
    mDevConsoleBox.setOutlineColor(sf::Color(0, 230, 255, 255));
    mDevConsoleBox.setOutlineThickness(3.f);
    mWindow.draw(mDevConsoleBox);

    static sf::Clock cursorClock;
    bool showCursor =
        (static_cast<int>(cursorClock.getElapsedTime().asSeconds() * 2.5f) %
             2 ==
         0);
    mDevConsoleText.setFont(mFont);
    mDevConsoleText.setCharacterSize(20);
    mDevConsoleText.setFillColor(sf::Color(100, 240, 255));
    mDevConsoleText.setOutlineColor(sf::Color::Black);
    mDevConsoleText.setOutlineThickness(1.5f);
    mDevConsoleText.setPosition(15.f, Win_H - 42.f);
    mDevConsoleText.setString("DEV PROMPT [~ or F8 or /] > " + mDevInputString +
                              (showCursor ? "_" : " "));
    mWindow.draw(mDevConsoleText);
  }

  mWindow.setView(origView);
}

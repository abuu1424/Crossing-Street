#include "CGAME.h"
#include "LevelConfig.h"
#include "HighScore.h"
#include <cstdio>
#include <cmath>

const float SPAWN_X = Win_W / 2.f - Player_W / 2.f;
const float SPAWN_Y = 590.f;

CGAME::CGAME(sf::RenderWindow& window)
    : mWindow(window),
    mScore(0),
    mlevelTime(0.f)
{
    setupUI();
}

CGAME::~CGAME() {
    clearEntities();
}

void CGAME::setupUI() {
    mFont.loadFromFile("assets/font/pixel_operator/PixelOperator.ttf");

    //Sound
    //S_Victory
    if (mVictoryBuffer.loadFromFile("assets/sounds/victory/vt1.ogg")) mVictorySound.setBuffer(mVictoryBuffer);
    else printf("Failed to load victory sound");
    //S_Dead
    if (mDeadBuffer.loadFromFile("assets/sounds/dead/dead.ogg")) mDeadSound.setBuffer(mDeadBuffer);
    else printf("Failed to load dead sound");
    //S_LevelClear
    if (mLevelClearBuffer.loadFromFile("assets/sounds/victory/level_clear.ogg")) mLevelClearSound.setBuffer(mLevelClearBuffer);
    else printf("Failed to load level clear sound");

    // Bảng DEAD
    float boxW = 400.f, boxH = 150.f;
    mDeadBox.setSize(sf::Vector2f(boxW, boxH));
    mDeadBox.setFillColor(sf::Color(0, 0, 0, 180));
    mDeadBox.setOrigin(boxW / 2.f, boxH / 2.f);
    mDeadBox.setPosition(Win_W / 2.f, Win_H / 2.f);

    mDeadText.setFont(mFont);
    mDeadText.setString("YOU DIED!\nPress R to restart");
    mDeadText.setCharacterSize(36);
    mDeadText.setFillColor(sf::Color::Red);
    sf::FloatRect db = mDeadText.getLocalBounds();
    mDeadText.setOrigin(db.left + db.width/2.f, db.top + db.height/2.f);
    mDeadText.setPosition(Win_W / 2.f, Win_H / 2.f);

    // Bảng VICTORY
    // Bảng VICTORY
    float vboxW = 500.f, vboxH = 260.f;
    mVictoryBox.setSize(sf::Vector2f(vboxW, vboxH));
    mVictoryBox.setFillColor(sf::Color(0, 0, 0, 200));
    mVictoryBox.setOutlineColor(sf::Color(255, 215, 0));
    mVictoryBox.setOutlineThickness(3.f);
    mVictoryBox.setOrigin(vboxW / 2.f, vboxH / 2.f);
    mVictoryBox.setPosition(Win_W / 2.f, Win_H / 2.f);

    mVictoryTitle.setFont(mFont);
    mVictoryTitle.setString("VICTORY!");
    mVictoryTitle.setCharacterSize(52);
    mVictoryTitle.setFillColor(sf::Color(255, 215, 0));
    sf::FloatRect vt = mVictoryTitle.getLocalBounds();
    mVictoryTitle.setOrigin(vt.left + vt.width / 2.f, vt.top + vt.height / 2.f);
    mVictoryTitle.setPosition(Win_W / 2.f, Win_H / 2.f - 85.f);

    mVictorySubText.setFont(mFont);
    mVictorySubText.setString("You escaped through time!\nPress R to play again");
    mVictorySubText.setCharacterSize(20);
    mVictorySubText.setFillColor(sf::Color::White);
    sf::FloatRect vs = mVictorySubText.getLocalBounds();
    mVictorySubText.setOrigin(vs.left + vs.width / 2.f, vs.top + vs.height / 2.f);
    mVictorySubText.setPosition(Win_W / 2.f, Win_H / 2.f - 20.f);

    mVictoryScore.setFont(mFont);
    mVictoryScore.setString("SCORE: 0");
    mVictoryScore.setCharacterSize(26);
    mVictoryScore.setFillColor(sf::Color::White);
    sf::FloatRect sc = mVictoryScore.getLocalBounds();
    mVictoryScore.setOrigin(sc.left + sc.width / 2.f, sc.top + sc.height / 2.f);
    mVictoryScore.setPosition(Win_W / 2.f, Win_H / 2.f + 45.f);

    mVictoryHighScore.setFont(mFont);
    mVictoryHighScore.setString("HIGH SCORE: 0");
    mVictoryHighScore.setCharacterSize(26);
    mVictoryHighScore.setFillColor(sf::Color(255, 215, 0));
    sf::FloatRect hs = mVictoryHighScore.getLocalBounds();
    mVictoryHighScore.setOrigin(hs.left + hs.width / 2.f, hs.top + hs.height / 2.f);
    mVictoryHighScore.setPosition(Win_W / 2.f, Win_H / 2.f + 85.f);


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
    mSaveTitle.setOrigin(stb.left + stb.width/2.f, stb.top + stb.height/2.f);
    mSaveTitle.setPosition(Win_W / 2.f, Win_H / 2.f - 40.f);

    mSaveInput.setFont(mFont);
    mSaveInput.setCharacterSize(28);
    mSaveInput.setFillColor(sf::Color(255, 215, 0));
    mSaveInput.setPosition(Win_W / 2.f - 180.f, Win_H / 2.f);

    //Bảng Quit
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
    sf::FloatRect qt = mQuitTitle.getLocalBounds();
    mQuitTitle.setOrigin(qt.left + qt.width/2.f, qt.top + qt.height/2.f);
    mQuitTitle.setPosition(Win_W / 2.f, Win_H / 2.f - 40.f);

    mYesText.setFont(mFont);
    mYesText.setString("YES");
    mYesText.setCharacterSize(28);
    mYesText.setFillColor(sf::Color(255, 100, 100));
    sf::FloatRect yt = mYesText.getLocalBounds();
    mYesText.setOrigin(yt.left + yt.width/2.f, yt.top + yt.height/2.f);
    mYesText.setPosition(Win_W / 2.f - 70.f, Win_H / 2.f + 30.f);

    mNoText.setFont(mFont);
    mNoText.setString("NO");
    mNoText.setCharacterSize(28);
    mNoText.setFillColor(sf::Color(150, 255, 150));
    sf::FloatRect nt = mNoText.getLocalBounds();
    mNoText.setOrigin(nt.left + nt.width/2.f, nt.top + nt.height/2.f);
    mNoText.setPosition(Win_W / 2.f + 70.f, Win_H / 2.f + 30.f);

    // Bảng Pause
    float pboxW = 400.f, pboxH = 220.f;
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
    sf::FloatRect pt = mPauseTitle.getLocalBounds();
    mPauseTitle.setOrigin(pt.left + pt.width/2.f, pt.top + pt.height/2.f);
    mPauseTitle.setPosition(Win_W / 2.f, Win_H / 2.f - 60.f);

    mResumeText.setFont(mFont);
    mResumeText.setString("Press P or click to RESUME");
    mResumeText.setCharacterSize(20);
    mResumeText.setFillColor(sf::Color(150, 255, 150));
    sf::FloatRect rt = mResumeText.getLocalBounds();
    mResumeText.setOrigin(rt.left + rt.width/2.f, rt.top + rt.height/2.f);
    mResumeText.setPosition(Win_W / 2.f, Win_H / 2.f);

    mQuitFromPauseText.setFont(mFont);
    mQuitFromPauseText.setString("Press ESC to QUIT");
    mQuitFromPauseText.setCharacterSize(20);
    mQuitFromPauseText.setFillColor(sf::Color(255, 150, 150));
    sf::FloatRect qft = mQuitFromPauseText.getLocalBounds();
    mQuitFromPauseText.setOrigin(qft.left + qft.width/2.f, qft.top + qft.height/2.f);
    mQuitFromPauseText.setPosition(Win_W / 2.f, Win_H / 2.f + 50.f);

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
        mLevelClearTitle.setOrigin(b.left + b.width/2.f, b.top + b.height/2.f);
        mLevelClearTitle.setPosition(Win_W / 2.f, Win_H / 2.f - 110.f);
    }

    mLevelClearScore.setFont(mFont);
    mLevelClearScore.setCharacterSize(20);
    mLevelClearScore.setFillColor(sf::Color::White);
    mLevelClearScore.setPosition(Win_W / 2.f, Win_H / 2.f - 65.f);

    // 4 lựa chọn
    auto setupOpt = [&](sf::Text& t, const std::string& str, float y) {
        t.setFont(mFont);
        t.setString(str);
        t.setCharacterSize(20);
        t.setFillColor(sf::Color(150, 220, 255));
        sf::FloatRect b = t.getLocalBounds();
        t.setOrigin(b.left + b.width/2.f, b.top + b.height/2.f);
        t.setPosition(Win_W / 2.f, y);
    };

    setupOpt(mOpt1Text, "[1]  Next Level",    Win_H / 2.f - 10.f);
    setupOpt(mOpt2Text, "[2]  Save",          Win_H / 2.f + 40.f);
    setupOpt(mOpt3Text, "[3]  Save & Exit",   Win_H / 2.f + 90.f);
    setupOpt(mOpt4Text, "[4]  Exit",          Win_H / 2.f + 140.f);
}

void CGAME::centerText(sf::Text& text) {
    sf::FloatRect b = text.getLocalBounds();
    text.setOrigin(b.left + b.width / 2.f, b.top + b.height / 2.f);
}

void CGAME::setupLevelClearOptions() {
    if (mCurrentLevel == 2) {
        mOpt1Text.setString("[1]  See Victory Screen");
    }
    else {
        mOpt1Text.setString("[1]  Next Level");
    }

    mOpt2Text.setString("[2]  Save");
    mOpt3Text.setString("[3]  Save & Exit");
    mOpt4Text.setString("[4]  Exit");

    centerText(mOpt1Text);
    centerText(mOpt2Text);
    centerText(mOpt3Text);
    centerText(mOpt4Text);
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

sf::FloatRect shrinkBox(sf::FloatRect r, float amount)
{
    r.left += amount;
    r.top += amount;
    r.width -= amount * 2;
    r.height -= amount * 2;
    return r;
}

bool sameLane(sf::FloatRect playerBox, sf::FloatRect objectBox)
{
    float playerCenterY = playerBox.top + playerBox.height / 2.f;
    float objectCenterY = objectBox.top + objectBox.height / 2.f;

    return std::abs(playerCenterY - objectCenterY) < 40.f;
}

void CGAME::clearEntities()
{
    for (auto* o : mObstacles) delete o;
    for (auto* a : mAnimals)   delete a;
    delete mTraffic;

    mObstacles.clear();
    mAnimals.clear();
    mTraffic = nullptr;
}

void CGAME::loadLevel(int level) {
    clearEntities();
    LevelConfig cfg = getLevel(level);

    mLevelCleared = false;
    mCurrentLevel = cfg.level;
    mlevelTime = 0.f;
    mHUD.update(mCurrentLevel, mScore, mlevelTime);

    // Background
    if (!mBgTexture.loadFromFile(cfg.backgroundPath))
        printf("FAILED background\n");
    mBgSprite.setTexture(mBgTexture);
    mBgSprite.setScale(
        static_cast<float>(Win_W) / mBgTexture.getSize().x,
        static_cast<float>(Win_H) / mBgTexture.getSize().y
    );

    mLevelMusic.stop();
    if (mLevelMusic.openFromFile(cfg.musicPath)) {
        mLevelMusic.setLoop(true);
        mLevelMusic.setVolume(40.f);
        mLevelMusic.play();
    }

    // Player
    mPlayer.setDead(false);
    mPlayer.setFinish(false);
    mPlayer.setPosition(SPAWN_X, SPAWN_Y);

    // Spawn vật cản — KHÔNG CẦN if/else theo loại nữa
    for (auto& lane : cfg.lanes) {
        for (int i = 0; i < lane.count; i++) {
            float x = lane.direction > 0
                ? i * lane.spacing
                : Win_W - i * lane.spacing;

            CVEHICLE* obj = createObstacle(lane.type, lane.speed, lane.direction);
            obj->loadSprite(lane.spritePath, x, lane.y);
            mObstacles.push_back(obj);
        }
    }

    // Spawn động vật bay — tương tự
    for (auto& ani : cfg.animals) {
        for (int i = 0; i < ani.count; i++) {
            float x = ani.direction > 0
                ? i * ani.spacing
                : Win_W - i * ani.spacing;

            CANIMAL* obj = createAnimal(ani.type, ani.speed, ani.direction);
            obj->loadSprite(ani.spritePath, x, ani.y);
            mAnimals.push_back(obj);
        }
    }

    // Traffic light
    mTraffic = new CTRAFFIC_LV1(mObstacles);
    mTraffic->loadSprite(
        cfg.trafficRedPath, cfg.trafficGreenPath,
        cfg.trafficX, cfg.trafficY
    );
}

void CGAME::reset() {
    mDeadSound.stop();
    mVictorySound.stop();
    mLevelMusic.stop();

    mScore = 0;
    mlevelTime = 0.f;
    mLevelCleared = false;

    mPlayer.setDead(false);
    mPlayer.setFinish(false);
    mPlayer.setPosition(SPAWN_X, SPAWN_Y);

    loadLevel(1);
    mHUD.update(mCurrentLevel, mScore, mlevelTime);
}

void CGAME::handleEvents() {
    sf::Event event;

    while (mWindow.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            mWindow.close();
            continue;
        }

        sf::Vector2f mouse;
        if (event.type == sf::Event::MouseButtonPressed) {
            mouse = mWindow.mapPixelToCoords(
                sf::Vector2i(event.mouseButton.x, event.mouseButton.y)
            );
        }

        // Bảng QUIT confirm
        if (mShowQuitConfirm) {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape ||
                    event.key.code == sf::Keyboard::N) {
                    mShowQuitConfirm = false;
                }
                else if (event.key.code == sf::Keyboard::Y) {
                    mWindow.close();
                }
            }

            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {

                if (mYesText.getGlobalBounds().contains(mouse)) {
                    mWindow.close();
                }
                else if (mNoText.getGlobalBounds().contains(mouse)) {
                    mShowQuitConfirm = false;
                }
            }

            continue;
        }

        // Bảng PAUSE
        if (mPaused) {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::P) {
                    mPaused = false;
                }
                else if (event.key.code == sf::Keyboard::Escape) {
                    mPaused = false;
                    mShowQuitConfirm = true;
                }
            }

            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {
                if (mResumeText.getGlobalBounds().contains(mouse))
                {
                    mPaused = false;
                }
                else if (mHUD.getPauseIconBounds().contains(mouse)) {
                    mPaused = false;
                }
            }

            continue;
        }

        // Bảng chọn save slot
        if (mSelectingSaveSlot) {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Num1) mSaveSlotPending = 1;
                else if (event.key.code == sf::Keyboard::Num2) mSaveSlotPending = 2;
                else if (event.key.code == sf::Keyboard::Num3) mSaveSlotPending = 3;
                else if (event.key.code == sf::Keyboard::Escape) {
                    mSelectingSaveSlot = false;
                    mShowLevelClear = true;
                    mSaveSlotPending = 0;
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
                        }
                        else {
                            mShowLevelClear = true;
                        }
                    }
                    else {
                        mSelectingSaveSlot = false;
                        mEnteringSaveName = true;
                        mCurrentSaveName.clear();
                    }
                }
            }

            continue;
        }

        if (mShowLevelClear) {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Num1) {
                    mShowLevelClear = false;
                    mLevelClearSound.stop();
                    mPlayer.setFinish(false);

                    if (mCurrentLevel < 2)
                    {
                        loadLevel(mCurrentLevel + 1);
                    }
                    else {
                        mShowLevelClear = false;
                        mPlayer.setFinish(true);
                        mLevelClearSound.stop();
                        mVictorySound.play();
                    }
                }
                else if (event.key.code == sf::Keyboard::Num2) {
                    // Save: mở bảng chọn slot
                    mShowLevelClear = false;
                    mSelectingSaveSlot = true;
                    mPendingSaveAndExit = false;
                    mSaveSlotPending = 0;
                    mCurrentSaveName.clear();
                }
                else if (event.key.code == sf::Keyboard::Num3) {
                    // Save & Exit: mở bảng chọn slot, sau khi save sẽ exit
                    mShowLevelClear = false;
                    mSelectingSaveSlot = true;
                    mPendingSaveAndExit = true;
                    mSaveSlotPending = 0;
                    mCurrentSaveName.clear();
                }
                else if (event.key.code == sf::Keyboard::Num4) {
                    mWindow.close();
                }
            }

            if (event.type == sf::Event::MouseButtonPressed
                && event.mouseButton.button == sf::Mouse::Left) {

                sf::Vector2f mouse = mWindow.mapPixelToCoords(
                    sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

                if (mOpt1Text.getGlobalBounds().contains(mouse)) {
                    mShowLevelClear = false;
                    mLevelClearSound.stop();
                    mPlayer.setFinish(false);

                    if (mCurrentLevel < 2)
                    {
                        loadLevel(mCurrentLevel + 1);
                        mLevelClearSound.stop();
                    }
                    else {
                        mPlayer.setFinish(true);
                        mVictorySound.play();
                    }
                }
                else if (mOpt2Text.getGlobalBounds().contains(mouse)) {
                    // Save: mở bảng chọn slot
                    mShowLevelClear = false;
                    mSelectingSaveSlot = true;
                    mPendingSaveAndExit = false;
                    mSaveSlotPending = 0;
                    mCurrentSaveName.clear();
                }
                else if (mOpt3Text.getGlobalBounds().contains(mouse)) {
                    // Save & Exit: mở bảng chọn slot
                    mShowLevelClear = false;
                    mSelectingSaveSlot = true;
                    mPendingSaveAndExit = true;
                    mSaveSlotPending = 0;
                    mCurrentSaveName.clear();
                }
                else if (mOpt4Text.getGlobalBounds().contains(mouse)) {
                    mWindow.close();
                }
            }

            continue;
        }

        // Đang nhập tên save
        if (mEnteringSaveName) {
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode >= 32 &&
                    event.text.unicode < 128 &&
                    mCurrentSaveName.size() < 20) {
                    mCurrentSaveName += static_cast<char>(event.text.unicode);
                }
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::BackSpace &&
                    !mCurrentSaveName.empty()) {
                    mCurrentSaveName.pop_back();
                }
                else if (event.key.code == sf::Keyboard::Enter) {
                    if (mCurrentSaveName.empty()) {
                        mCurrentSaveName =
                            "Save Slot " + std::to_string(mSaveSlotPending);
                    }

                    saveGame(mSaveSlotPending);

                    mEnteringSaveName = false;
                    mSaveSlotPending = 0;
                    mCurrentSaveName.clear();

                    if (mPendingSaveAndExit) {
                        mPendingSaveAndExit = false;
                        mWindow.close();
                    }
                    else {
                        setupLevelClearOptions();
                        mShowLevelClear = true;
                    }
                }
                else if (event.key.code == sf::Keyboard::Escape) {
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
                continue;
            }
        }

        // Bình thường
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                mShowQuitConfirm = true;
            }
            else if (event.key.code == sf::Keyboard::R) {
                reset();
            }
            else if (event.key.code == sf::Keyboard::P) {
                mPaused = true;
            }
            else if (event.key.code == sf::Keyboard::F1) {
                if (SaveData::hasData(1)) {
                    auto slots = SaveData::getAllSlots();
                    mCurrentSaveName = slots[0].saveName.empty()
                        ? "Save Slot 1" : slots[0].saveName;
                    saveGame(1);
                    mCurrentSaveName.clear();
                }
                else {
                    mEnteringSaveName = true;
                    mSaveSlotPending = 1;
                    mCurrentSaveName.clear();
                }
            }
            else if (event.key.code == sf::Keyboard::F2) {
                if (SaveData::hasData(2)) {
                    auto slots = SaveData::getAllSlots();
                    mCurrentSaveName = slots[1].saveName.empty()
                        ? "Save Slot 2" : slots[1].saveName;
                    saveGame(2);
                    mCurrentSaveName.clear();
                }
                else {
                    mEnteringSaveName = true;
                    mSaveSlotPending = 2;
                    mCurrentSaveName.clear();
                }
            }
            else if (event.key.code == sf::Keyboard::F3) {
                if (SaveData::hasData(3)) {
                    auto slots = SaveData::getAllSlots();
                    mCurrentSaveName = slots[2].saveName.empty()
                        ? "Save Slot 3" : slots[2].saveName;
                    saveGame(3);
                    mCurrentSaveName.clear();
                }
                else {
                    mEnteringSaveName = true;
                    mSaveSlotPending = 3;
                    mCurrentSaveName.clear();
                }
            }
            else if (event.key.code == sf::Keyboard::F4) {
                loadGame(1);
            }
            else if (event.key.code == sf::Keyboard::F5) {
                loadGame(2);
            }
            else if (event.key.code == sf::Keyboard::F6) {
                loadGame(3);
            }
        }
    }
}

void CGAME::handleCollision() {
    if (mPlayer.isDead() || mPlayer.isFinish()) return;

    sf::FloatRect pb = shrinkBox(mPlayer.getBounds(), 8.f);

    for (auto* obs : mObstacles) {
        sf::FloatRect ob = shrinkBox(obs->getBounds(), 8.f);

        if (sameLane(pb, ob) && pb.intersects(ob)) {
            mPlayer.setDead(true);
            mLevelMusic.stop();
            mDeadSound.play();
            printf("DEAD\n");
            return;
        }
    }

    for (auto* ani : mAnimals)
    {
        sf::FloatRect ab = shrinkBox(ani->getBounds(), 8.f);

        if (sameLane(pb, ab) && pb.intersects(ab)) {
            mPlayer.setDead(true);
            mLevelMusic.stop();
            mDeadSound.play();
            printf("DEAD\n");
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
        float timeRemaining = Level_Time_Limit - mlevelTime;
        if (timeRemaining < 0.f) timeRemaining = 0.f;
        int baseScore = 100 * mCurrentLevel;
        int timeBonus = static_cast<int>(timeRemaining) * 10;
        mScore += baseScore + timeBonus;
        printf("Level %d clear! +%d (base=%d, bonus=%d)\n",
               mCurrentLevel, baseScore + timeBonus, baseScore, timeBonus);

        // Cập nhật text bảng Level Clear
        mLevelClearTitle.setString("Level " + std::to_string(mCurrentLevel) + " Complete!");
        {
            sf::FloatRect b = mLevelClearTitle.getLocalBounds();
            mLevelClearTitle.setOrigin(b.left + b.width/2.f, b.top + b.height/2.f);
        }

        mLevelClearScore.setString(
            "+" + std::to_string(baseScore) + " base  +  " +
            std::to_string(timeBonus) + " time bonus  =  " +
            std::to_string(mScore) + " total"
        );
        {
            sf::FloatRect b = mLevelClearScore.getLocalBounds();
            mLevelClearScore.setOrigin(b.left + b.width/2.f, b.top + b.height/2.f);
            mLevelClearScore.setPosition(Win_W / 2.f, Win_H / 2.f - 65.f);
        }

        setupLevelClearOptions();

        mLevelMusic.stop();
        mPlayer.setFinish(true);

        if (mCurrentLevel < 2) {
            mLevelClearSound.play();
            mShowLevelClear = true;
        }
        else {
            mVictorySound.play();
            bool isNewHighScore = HighScore::updateIfHigher(mScore);
            if (isNewHighScore)
                printf("NEW HIGH SCORE: %d\n", mScore);
        }
    }
}

void CGAME::update(float dt) {
    if (mEnteringSaveName) return;
    if (mShowQuitConfirm) return;
    if (mPaused) return;
    if (mLevelCleared) return;

    if (!mPlayer.isDead() && !mPlayer.isFinish()) {
        mlevelTime += dt;
        if (mlevelTime >= Level_Time_Limit)
        {
            mPlayer.setDead(true);
            mLevelMusic.stop();
            mDeadSound.play();
            printf("You ran out of time");
        }
        mPlayer.Move(dt);
        mPlayer.update(dt);

        for (auto* obs : mObstacles) {
            obs->Move(dt);
            obs->update(dt);
        }

        for (auto* ani : mAnimals) {
            ani->Move(dt);
            ani->update(dt);
        }

        if (mTraffic) {
            mTraffic->update(dt);
        }

        handleCollision();
        checkFinish();
    }

    mHUD.update(mCurrentLevel, mScore, mlevelTime);
}

void CGAME::render() {
    mWindow.clear();

    mWindow.draw(mBgSprite);

    for (auto* obs : mObstacles) {
        obs->Draw(mWindow);
    }

    for (auto* ani : mAnimals) {
        ani->Draw(mWindow);
    }

    if (mTraffic) {
        mTraffic->Draw(mWindow);
    }

    mPlayer.Draw(mWindow);

    mHUD.draw(mWindow);

    if (mPlayer.isDead()) {
        mWindow.draw(mDeadBox);
        mWindow.draw(mDeadText);
    }

    if (mPlayer.isFinish() && mCurrentLevel == 2 && !mShowLevelClear) {
        mWindow.draw(mVictoryBox);
        mWindow.draw(mVictoryTitle);
        mWindow.draw(mVictorySubText);

        mVictoryScore.setString("SCORE: " + std::to_string(mScore));
        sf::FloatRect sc = mVictoryScore.getLocalBounds();
        mVictoryScore.setOrigin(sc.left + sc.width / 2.f, sc.top + sc.height / 2.f);

        int highScore = HighScore::load();
        mVictoryHighScore.setString("HIGH SCORE: " + std::to_string(highScore));
        sf::FloatRect hs = mVictoryHighScore.getLocalBounds();
        mVictoryHighScore.setOrigin(hs.left + hs.width / 2.f, hs.top + hs.height / 2.f);

        mWindow.draw(mVictoryScore);
        mWindow.draw(mVictoryHighScore);
    }

    if (mEnteringSaveName)
    {
        mSaveInput.setString(mCurrentSaveName + "_");

        mWindow.draw(mSaveBox);
        mWindow.draw(mSaveTitle);
        mWindow.draw(mSaveInput);
    }

    if (mShowQuitConfirm)
    {
        mWindow.draw(mQuitBox);
        mWindow.draw(mQuitTitle);
        mWindow.draw(mYesText);
        mWindow.draw(mNoText);
    }

    if (mPaused)
    {
        mWindow.draw(mPauseBox);
        mWindow.draw(mPauseTitle);
        mWindow.draw(mResumeText);
        mWindow.draw(mQuitFromPauseText);
    }

    if (mSelectingSaveSlot) {
        setupSaveSlotOptions();

        mWindow.draw(mLevelClearBox);
        mWindow.draw(mLevelClearTitle);
        mWindow.draw(mOpt1Text);
        mWindow.draw(mOpt2Text);
        mWindow.draw(mOpt3Text);
        mWindow.draw(mOpt4Text);
    }

    if (mShowLevelClear) {
        mWindow.draw(mLevelClearBox);
        mWindow.draw(mLevelClearTitle);
        mWindow.draw(mLevelClearScore);
        mWindow.draw(mOpt1Text);
        mWindow.draw(mOpt2Text);
        mWindow.draw(mOpt3Text);
        mWindow.draw(mOpt4Text);
    }

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

                mMenu.handleEvent(event, mWindow, menuResult);
            }

            if (menuResult == MenuResult::NEW_GAME) {
                mInMenu = false;
                reset();
                menuResult = MenuResult::NONE;
            }
            else if (menuResult == MenuResult::QUIT) {
                mWindow.close();
            }
            else if (menuResult == MenuResult::SETTING) {
                menuResult = MenuResult::NONE;
            }

            // LOAD_GAME
            else if (menuResult == MenuResult::LOAD_SLOT_1)
            {
                if (loadGame(1)) mInMenu = false;
                menuResult = MenuResult::NONE;
            }
            else if (menuResult == MenuResult::LOAD_SLOT_2) {
                if (loadGame(2)) {
                    mInMenu = false;
                }
                menuResult = MenuResult::NONE;
            }
            else if (menuResult == MenuResult::LOAD_SLOT_3) {
                if (loadGame(3)) {
                    mInMenu = false;
                }
                menuResult = MenuResult::NONE;
            }

            mMenu.update(dt, mWindow);

            mWindow.clear();
            mMenu.draw(mWindow);
            mWindow.display();

            continue;
        }

        // Game
        handleEvents();
        update(dt);
        render();
    }
}

//Save Game
void CGAME::saveGame(int slot) {
                sf::Vector2f pos = mPlayer.getPosition();

                std::string name = mCurrentSaveName.empty()
                    ? "Save Slot " + std::to_string(slot)
                    : mCurrentSaveName;

                SaveData::save(
                    slot,
                    mCurrentLevel,
                    mScore,
                    pos.x,
                    pos.y,
                    name
                );
            }

bool CGAME::loadGame(int slot)
{
    int level = 1, score = 0;
    float playerX = SPAWN_X; float playerY = SPAWN_Y;
    if (!SaveData::load(slot, level, score, playerX, playerY))
        return false;

    mScore = score;
    loadLevel(level);
    mPlayer.setDead(false);
    mPlayer.setFinish(false);
    mPlayer.setPosition(playerX, playerY);
    mHUD.update(mCurrentLevel, mScore, mlevelTime);
    return true;
}

CVEHICLE* CGAME::createObstacle(ObstacleType type, float speed, float direction) {
    switch (type) {
    case ObstacleType::DINOSAUR: return new CDINOSOUR(speed, direction);
    case ObstacleType::MAMMOTH:  return new CMAMMOTH(speed, direction);
    case ObstacleType::CHARIOT: return new CCHARIOT(speed, direction);
    case ObstacleType::WAR_ELEPHENT: return new CWARELEPHENT(speed, direction);
    default: return new CDINOSOUR(speed, direction);
    }
}

CANIMAL* CGAME::createAnimal(AnimalType type, float speed, float direction) {
    switch (type) {
    case AnimalType::BIRD: return new CBIRD(speed, direction);
    case AnimalType::EAGLE: return new CEAGLE(speed, direction);
    default: return new CBIRD(speed, direction);
    }
}
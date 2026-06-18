#include "CGAME.h"
#include "LevelConfig.h"
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
    float vboxW = 500.f, vboxH = 200.f;
    mVictoryBox.setSize(sf::Vector2f(vboxW, vboxH));
    mVictoryBox.setFillColor(sf::Color(0, 0, 0, 200));
    mVictoryBox.setOutlineColor(sf::Color(255, 215, 0));  // viền vàng
    mVictoryBox.setOutlineThickness(3.f);
    mVictoryBox.setOrigin(vboxW / 2.f, vboxH / 2.f);
    mVictoryBox.setPosition(Win_W / 2.f, Win_H / 2.f);

    mVictoryTitle.setFont(mFont);
    mVictoryTitle.setString("VICTORY!");
    mVictoryTitle.setCharacterSize(52);
    mVictoryTitle.setFillColor(sf::Color(255, 215, 0));  // chữ vàng
    sf::FloatRect vt = mVictoryTitle.getLocalBounds();
    mVictoryTitle.setOrigin(vt.left + vt.width/2.f, vt.top + vt.height/2.f);
    mVictoryTitle.setPosition(Win_W / 2.f, Win_H / 2.f - 40.f);

    mVictorySubText.setFont(mFont);
    mVictorySubText.setString("You escaped through time!\nPress R to play again");
    mVictorySubText.setCharacterSize(20);
    mVictorySubText.setFillColor(sf::Color::White);
    sf::FloatRect vs = mVictorySubText.getLocalBounds();
    mVictorySubText.setOrigin(vs.left + vs.width/2.f, vs.top + vs.height/2.f);
    mVictorySubText.setPosition(Win_W / 2.f, Win_H / 2.f + 40.f);

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
                }
                else if (event.key.code == sf::Keyboard::Escape) {
                    mEnteringSaveName = false;
                    mSaveSlotPending = 0;
                    mCurrentSaveName.clear();
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

        if (mCurrentLevel == 1) {
            mPlayer.setFinish(true);
            mLevelMusic.stop();
            mVictorySound.play();
            printf("VICTORY!\n");
            return;
        }

        loadLevel(mCurrentLevel + 1);
    }
}

void CGAME::update(float dt) {
    if (mEnteringSaveName) return;
    if (mShowQuitConfirm) return;
    if (mPaused) return;
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

    if (mPlayer.isFinish()) {
        mWindow.draw(mVictoryBox);
        mWindow.draw(mVictoryTitle);
        mWindow.draw(mVictorySubText);
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
    default: return new CDINOSOUR(speed, direction);
    }
}

CANIMAL* CGAME::createAnimal(AnimalType type, float speed, float direction) {
    switch (type) {
    case AnimalType::BIRD: return new CBIRD(speed, direction);
    default: return new CBIRD(speed, direction);
    }
}
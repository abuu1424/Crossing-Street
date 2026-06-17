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
    loadLevel(1);
}

CGAME::~CGAME() {
    clearEntities();
}
void CGAME::setupUI() {
    mFont.loadFromFile("assets/font/pixel_operator/PixelOperator.ttf");

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

void CGAME::clearEntities() {
    for (auto* d : mDinos)  delete d;
    for (auto* b : mBirds)  delete b;
    delete mTraffic;
    mDinos.clear();
    mBirds.clear();
    mObstacles.clear();
    mAnimals.clear();
    mTraffic = nullptr;
}

void CGAME::loadLevel(int level) {
    clearEntities();

    LevelConfig cfg = getLevel(level);  // lấy config theo level

    mLevelCleared = false;
    mCurrentLevel = cfg.level;
    mlevelTime = 0.f;

    mHUD.update(mCurrentLevel, mScore, mlevelTime);

    // Background
    if (!mBgTexture.loadFromFile(cfg.backgroundPath)) {
        printf("FAILED background\n");
    }

    mBgSprite.setTexture(mBgTexture);
    mBgSprite.setScale(
        static_cast<float>(Win_W) / mBgTexture.getSize().x,
        static_cast<float>(Win_H) / mBgTexture.getSize().y
    );

    // Player
    mPlayer.setDead(false);
    mPlayer.setFinish(false);
    mPlayer.setPosition(SPAWN_X, SPAWN_Y);

    // Spawn vật cản theo config
    for (auto& lane : cfg.lanes) {
        for (int i = 0; i < lane.count; i++) {
            auto* d = new CDINOSOUR(lane.speed, lane.direction);

            float x = lane.direction > 0
                ? i * lane.spacing
                : Win_W - i * lane.spacing;

            d->loadSprite(lane.spritePath, x, lane.y);

            mDinos.push_back(d);
            mObstacles.push_back(d);
        }
    }

    // Spawn động vật bay theo config
    for (auto& ani : cfg.animals) {
        for (int i = 0; i < ani.count; i++) {
            auto* b = new CBIRD(ani.speed, ani.direction);

            float x = ani.direction > 0
                ? i * ani.spacing
                : Win_W - i * ani.spacing;

            b->loadSprite(ani.spritePath, x, ani.y);

            mBirds.push_back(b);
            mAnimals.push_back(b);
        }
    }

    // Traffic light theo config
    mTraffic = new CTRAFFIC_LV1(mObstacles);
    mTraffic->loadSprite(
        cfg.trafficRedPath,
        cfg.trafficGreenPath,
        cfg.trafficX,
        cfg.trafficY
    );
}

void CGAME::reset() {
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
        }

        //Bảng QUIT
        if (mShowQuitConfirm) {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    mShowQuitConfirm = false;  // ESC lần nữa để hủy quit
                }
                else if (event.key.code == sf::Keyboard::Y) {
                    mWindow.close();
                }
                else if (event.key.code == sf::Keyboard::N) {
                    mShowQuitConfirm = false;
                }
            }
            if (event.type == sf::Event::MouseButtonPressed
                && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mouse = mWindow.mapPixelToCoords(
                    sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

                if (mYesText.getGlobalBounds().contains(mouse))
                    mWindow.close();
                else if (mNoText.getGlobalBounds().contains(mouse))
                    mShowQuitConfirm = false;
            }
            continue;
        }

        //Đang nhập tên save
        if (mEnteringSaveName) {
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode >= 32 &&
                    event.text.unicode < 128 &&
                    mCurrentSaveName.size() < 20)
                {
                    mCurrentSaveName += static_cast<char>(event.text.unicode);
                }
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::BackSpace &&
                    !mCurrentSaveName.empty())
                {
                    mCurrentSaveName.pop_back();
                }
                else if (event.key.code == sf::Keyboard::Enter) {
                    if (mCurrentSaveName.empty()) {
                        mCurrentSaveName =
                            "Save Slot " + std::to_string(mSaveSlotPending);
                    }

                    saveGame(mSaveSlotPending);

                    mEnteringSaveName = false;
                    mSaveSlotPending  = 0;
                    mCurrentSaveName.clear();
                }
                else if (event.key.code == sf::Keyboard::Escape) {
                    mEnteringSaveName = false;
                    mSaveSlotPending  = 0;
                    mCurrentSaveName.clear();
                }
            }

            continue;
        }

        //Bình thường
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                mShowQuitConfirm = true;
            }
            else if (event.key.code == sf::Keyboard::R) {
                reset();
            }

            // Save F1
            else if (event.key.code == sf::Keyboard::F1) {
                if (SaveData::hasData(1)) {
                    auto slots = SaveData::getAllSlots();
                    mCurrentSaveName = slots[0].saveName.empty()
                        ? "Save Slot 1"
                        : slots[0].saveName;

                    saveGame(1);
                    mCurrentSaveName.clear();
                }
                else {
                    mEnteringSaveName = true;
                    mSaveSlotPending  = 1;
                    mCurrentSaveName.clear();
                }
            }

            // Save F2
            else if (event.key.code == sf::Keyboard::F2) {
                if (SaveData::hasData(2)) {
                    auto slots = SaveData::getAllSlots();
                    mCurrentSaveName = slots[1].saveName.empty()
                        ? "Save Slot 2"
                        : slots[1].saveName;

                    saveGame(2);
                    mCurrentSaveName.clear();
                }
                else {
                    mEnteringSaveName = true;
                    mSaveSlotPending  = 2;
                    mCurrentSaveName.clear();
                }
            }

            // Save F3
            else if (event.key.code == sf::Keyboard::F3) {
                if (SaveData::hasData(3)) {
                    auto slots = SaveData::getAllSlots();
                    mCurrentSaveName = slots[2].saveName.empty()
                        ? "Save Slot 3"
                        : slots[2].saveName;

                    saveGame(3);
                    mCurrentSaveName.clear();
                }
                else {
                    mEnteringSaveName = true;
                    mSaveSlotPending  = 3;
                    mCurrentSaveName.clear();
                }
            }

            // Load F4 F5 F6
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
            printf("VICTORY!\n");
            return;
        }

        loadLevel(mCurrentLevel + 1);
    }
}

void CGAME::update(float dt) {
    if (mEnteringSaveName) return;
    if (!mPlayer.isDead() && !mPlayer.isFinish()) {
        mlevelTime += dt;
        if (mlevelTime >= Level_Time_Limit)
        {
            mPlayer.setDead(true);
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
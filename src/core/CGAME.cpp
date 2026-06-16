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
        if (event.type == sf::Event::Closed)
            mWindow.close();
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape)
                mWindow.close();
            if (event.key.code == sf::Keyboard::R)
                reset();
        }
        if (event.key.code == sf::Keyboard::F1) saveGame(1);
        if (event.key.code == sf::Keyboard::F2) saveGame(2);
        if (event.key.code == sf::Keyboard::F3) saveGame(3);
        if (event.key.code == sf::Keyboard::F4) loadGame(1);
        if (event.key.code == sf::Keyboard::F5) loadGame(2);
        if (event.key.code == sf::Keyboard::F6) loadGame(3);
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

            // LOAD_GAME xử lý sau khi có save/load

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
    SaveData::save(slot, mCurrentLevel, mScore);
}

bool CGAME::loadGame(int slot)
{
    int level = 1, score = 0;
    if (!SaveData::load(slot, level, score))
        return false;

    mScore = score;
    loadLevel(level);
    return true;
}
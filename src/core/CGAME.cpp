#include "CGAME.h"
#include "LevelConfig.h"
#include <cstdio>

const float SPAWN_X = Win_W / 2.f - Player_W / 2.f;
const float SPAWN_Y = 590.f;

CGAME::CGAME(sf::RenderWindow& window) : mWindow(window) {
    loadLevel(1);
}

CGAME::~CGAME() {
    clearEntities();
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
    mLevelCleared = false;
    mCurrentLevel = level;

    LevelConfig cfg = getLevel(level);  // lấy config theo level

    // Background
    if (!mBgTexture.loadFromFile(cfg.backgroundPath))
        printf("FAILED background\n");
    mBgSprite.setTexture(mBgTexture);
    mBgSprite.setScale(
        (float)Win_W / mBgTexture.getSize().x,
        (float)Win_H / mBgTexture.getSize().y
    );

    // Player
    mPlayer.setDead(false);
    mPlayer.setFinish(false);
    mPlayer.setPosition(SPAWN_X, SPAWN_Y);

    // Spawn xe theo config
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

    // Spawn động vật
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

    // Traffic light
    mTraffic = new CTRAFFIC_LV1(mObstacles);
    mTraffic->loadSprite(
        cfg.trafficRedPath,
        cfg.trafficGreenPath,
        cfg.trafficX,
        cfg.trafficY
    );
}

void CGAME::reset() {
    mPlayer.setDead(false);
    mPlayer.setFinish(false);
    mLevelCleared = false;
    mPlayer.setPosition(SPAWN_X, SPAWN_Y);
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

    for (auto* ani : mAnimals) {
        sf::FloatRect ab = shrinkBox(ani->getBounds(), 8.f);

        if (sameLane(pb, ab) && pb.intersects(ab)) {
            mPlayer.setDead(true);
            printf("DEAD\n");
            return;
        }
    }
}

void CGAME::checkFinish() {
    if (!mPlayer.isDead() && !mLevelCleared
        && mPlayer.getPosition().y < 80.f) {
        mLevelCleared = true;
        printf("LEVEL %d CLEAR!\n", mCurrentLevel);

        if (mCurrentLevel < 5)
            loadLevel(mCurrentLevel + 1);  // sang level tiếp
        else {
            mPlayer.setFinish(true);
            printf("VICTORY!\n");
        }
    }
}

void CGAME::update(float dt) {
    if (!mPlayer.isDead() && !mPlayer.isFinish()) {
        mPlayer.Move(dt);
        mPlayer.update(dt);
    }
    for (auto* obs : mObstacles) { obs->Move(dt); obs->update(dt); }
    for (auto* ani : mAnimals)   { ani->Move(dt); ani->update(dt); }
    if (mTraffic) mTraffic->update(dt);

    handleCollision();
    checkFinish();
}

void CGAME::render() {
    mWindow.clear();
    mWindow.draw(mBgSprite);
    for (auto* obs : mObstacles) obs->Draw(mWindow);
    for (auto* ani : mAnimals)   ani->Draw(mWindow);
    if (mTraffic) mTraffic->Draw(mWindow);
    mPlayer.Draw(mWindow);
    mWindow.display();
}

void CGAME::run() {
    sf::Clock clock;
    while (mWindow.isOpen()) {
        float dt = clock.restart().asSeconds();
        handleEvents();
        update(dt);
        render();
    }
}
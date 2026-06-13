#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Utils.h"
#include "CPEOPLE.h"
#include "CDINOSOUR.h"
#include "CBIRD.h"
#include "CTRAFFIC_LV1.h"

struct LaneConfig {
    float speed;
    float direction;
    float y;
    std::string spritePath;
    int   count;
    float spacing;
};

class CGAME {
    sf::RenderWindow&        mWindow;

    // Background
    sf::Texture              mBgTexture;
    sf::Sprite               mBgSprite;

    // Player
    CPEOPLE                  mPlayer;

    // Vật cản
    std::vector<CDINOSOUR*>  mDinos;
    std::vector<CBIRD*>      mBirds;

    // Vector dùng cho collision + traffic
    std::vector<CVEHICLE*>   mObstacles;
    std::vector<CANIMAL*>    mAnimals;

    // Traffic light
    CTRAFFIC_LV1*            mTraffic= nullptr;

    // Trạng thái
    bool                     mLevelCleared = false;
    int                      mCurrentLevel = 1;

    // Helpers
    void loadLevel(int level);
    void clearEntities();
    void handleCollision();
    void checkFinish();

public:
    explicit CGAME(sf::RenderWindow& window);
    ~CGAME();

    void run();          // vòng lặp chính
    void handleEvents();
    void update(float dt);
    void render();
    void reset();
};
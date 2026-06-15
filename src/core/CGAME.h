#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Utils.h"
#include "CPEOPLE.h"
#include "CDINOSOUR.h"
#include "CBIRD.h"
#include "CTRAFFIC_LV1.h"
#include "LevelConfig.h"
#include "Menu.h"

class CGAME {
    sf::RenderWindow&        mWindow;

    // Background
    sf::Texture              mBgTexture;
    sf::Sprite               mBgSprite;

    //Menu
    bool mInMenu = true;
    Menu mMenu;

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
    void setupUI();

    //Bảng DEAD
    sf::Font mFont;
    sf::Text mDeadText;
    sf::RectangleShape mDeadBox;
    // Bảng VICTORY
    sf::RectangleShape mVictoryBox;
    sf::Text mVictoryTitle;
    sf::Text mVictorySubText;

public:
    explicit CGAME(sf::RenderWindow& window);
    ~CGAME();

    void run();          // vòng lặp chính
    void handleEvents();
    void update(float dt);
    void render();
    void reset();
};
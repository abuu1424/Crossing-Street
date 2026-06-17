#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Utils.h"
#include "CPEOPLE.h"
#include "CDINOSOUR.h"
#include "CBIRD.h"
#include "CMAMMOTH.h"
#include "CTRAFFIC_LV1.h"
#include "LevelConfig.h"
#include "Menu.h"
#include "HUD.h"
#include "SaveData.h"


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

    // HUD
    HUD                      mHUD;
	int     	             mScore = 0;
	float                    mlevelTime = 0.f;
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

    //Bảng nhập tên file save
    bool mEnteringSaveName = false;
    std::string mCurrentSaveName;
    int mSaveSlotPending = 0;

    sf::RectangleShape mSaveBox;
    sf::Text mSaveTitle;
    sf::Text mSaveInput;

    //Bảng Exit
    bool mShowQuitConfirm = false;
    sf::RectangleShape mQuitBox;
    sf::Text mQuitTitle;
    sf::Text mYesText;
    sf::Text mNoText;
    bool mYesHovered = false;
    bool mNoHovered  = false;

    //Sound
    //Victory
    sf::SoundBuffer mVictoryBuffer;
    sf::Sound mVictorySound;
    //Dead
    sf::SoundBuffer mDeadBuffer;
    sf::Sound mDeadSound;

    //Music
    sf::Music mLevelMusic;

public:
    explicit CGAME(sf::RenderWindow& window);
    ~CGAME();

    void run();          // vòng lặp chính
    void handleEvents();
    void update(float dt);
    void render();
    void reset();

    //Save game
    void saveGame(int slot);
    bool loadGame(int slot);
};
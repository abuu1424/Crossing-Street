#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <vector>
#include <algorithm>
#include "SaveData.h"
#include "Animation.h"
#include "TextureManager.h"
#include "MenuButton.h"

enum class ChallengeMode {
    NONE,
    TWO_PLAYERS,
    VS_BOT,
    ENDLESS
};

enum class MenuResult {
    NONE,
    NEW_GAME,
    NEW_GAME_SLOT_1,
    NEW_GAME_SLOT_2,
    NEW_GAME_SLOT_3,
    LOAD_SLOT_1,
    LOAD_SLOT_2,
    LOAD_SLOT_3,
    QUIT,
    SETTING,
    CHALLENGE_TWO_PLAYERS,
    CHALLENGE_VS_BOT_EASY,
    CHALLENGE_VS_BOT_NORMAL,
    CHALLENGE_VS_BOT_HARD,
    CHALLENGE_ENDLESS
};

enum class MenuScreen {
    MAIN,
    NEW_GAME_SELECT,
    LOAD,
    SETTINGS,
    INFO,
    SHOP,
    CHALLENGES,
    BOT_DIFFICULTY
};

class Menu
{
    // Background
    sf::Texture mBgTexture;
    sf::Sprite mBgSprite;

    // Title
    sf::Font mFont;
    sf::Text mTitle;
    sf::Texture mTitleTexture;
    sf::Sprite mTitleSprite;
    std::unique_ptr<Animation> mTitleAnim;


    // Main Buttons (Clean 5-button layout)
    MenuButton mBtnNew;
    MenuButton mBtnChallenges;
    MenuButton mBtnLoad;
    MenuButton mBtnSetting;
    MenuButton mBtnQuit;
    MenuButton mBtnBack;

    // Challenges Menu
    MenuButton mBtnChallenge2P;
    MenuButton mBtnChallengeBot;
    MenuButton mBtnChallengeEndless;
    MenuButton mBtnChallengeBack;
    sf::Text   mChallengeTitle;

    void setupChallengeMenu();
    void drawChallengeMenu(sf::RenderWindow& window);
    void handleChallengeEvent(const sf::Event& event,
                              sf::RenderWindow& window,
                              MenuResult& result);

    // Bot Difficulty Menu
    MenuButton mBtnDifficultyEasy;
    MenuButton mBtnDifficultyNormal;
    MenuButton mBtnDifficultyHard;
    MenuButton mBtnDifficultyBack;
    sf::Text   mDiffTitle;
    sf::Sprite mDiffCardSprites[3];
    sf::Text   mDiffCardTitles[3];
    sf::Text   mDiffCardDescs[3];
    sf::Text   mDiffCardBadges[3];

    void setupDifficultyMenu();
    void drawDifficultyMenu(sf::RenderWindow& window);
    void handleDifficultyEvent(const sf::Event& event,
                               sf::RenderWindow& window,
                               MenuResult& result);

    // Load menu
    MenuScreen mScreen = MenuScreen::MAIN;
    std::vector<SaveSlot> mSaveSlots;
    int mConfirmOverwriteSlot = -1;

    sf::Text mLoadTitle;
    sf::Text mSlotTexts[3];
    sf::RectangleShape mPanelBox;
    sf::Texture mPanelTexture;
    sf::Sprite  mPanelSprite;
    sf::Texture mSlotFrameTexture;
    sf::Texture mSlotFrameHoverTexture;
    sf::Sprite  mSlotFrameSprites[3];
    sf::Text mOverwriteHint;

    // Nhạc nền
    sf::Music mMusic;

    //SETTINGS
    struct Slider {
        float value     = 50.f;   // 0-100
        float x, y, w;           // vị trí và chiều rộng
        bool dragging  = false;
        sf::RectangleShape track;
        sf::RectangleShape thumb;
        sf::Text     label;
        sf::Text     valueText;
    };

    Slider mMusicSlider;
    Slider mSFXSlider;
    bool   mMuteAll   = false;

    sf::Text mSettingsTitle;
    sf::Text mMuteText;
    MenuButton mBtnBackSettings;

    bool mEnteringNewGameName = false;
    int  mPendingNewGameSlot  = -1;
    std::string mNewGameName;

    sf::Texture mNameBoxTexture;
    sf::Sprite  mNameBoxSprite;
    sf::Text mNameTitle;
    sf::Text mNameInput;
    sf::Text mNameHint;

    void setupNewGameNamePopup();
    void drawNewGameNamePopup(sf::RenderWindow& window);
    void handleNewGameNameEvent(const sf::Event& event, MenuResult& result);

    // Info / Help Popup
    MenuButton mBtnInfo;
    int mInfoTab = 0; // 0: Controls, 1: Gameplay & Eras, 2: Items & Tips
    sf::Text mInfoTitle;
    sf::Text mInfoTabButtons[3];
    sf::Text mInfoCardTitleLeft[3];
    sf::Text mInfoCardTitleRight[3];

    // Tab 0: Controls & Combat Skills
    sf::Text mInfoTab0Left[6];
    sf::Text mInfoTab0Right[6];

    // Tab 1: Gameplay & 5 Eras
    sf::Text mInfoTab1Left[6];
    sf::Text mInfoTab1Right[6];

    // Tab 2: Shop & In-Lane Power-Ups
    sf::Text mInfoTab2Left[6];
    sf::Text mInfoTab2Right[6];

    sf::Text mInfoPageHint;
    MenuButton mBtnBackInfo;

    void setupInfoMenu();
    void drawInfoMenu(sf::RenderWindow& window);
    void handleInfoEvent(const sf::Event& event,
                         sf::RenderWindow& window,
                         MenuResult& result);

    // Shop Popup
    MenuButton mBtnShop;
    sf::Text mShopTitle;
    sf::Text mShopCoinsText;
    sf::Text mShopSlotTabs[3];
    sf::Texture mItemTextures[5];
    sf::Sprite  mItemSprites[5];
    sf::Text mItemTitleTexts[5];
    sf::Text mItemDescTexts[5];
    sf::Text mItemPriceTexts[5];
    MenuButton mItemBuyButtons[5];
    MenuButton mBtnBackShop;

    void setupShopMenu();


    void setupSettingsMenu();
    void updateSlider(Slider& s, sf::Vector2f mouse, bool mouseDown);
    void drawSlider(sf::RenderWindow& w, Slider& s);
    void drawSettingsMenu(sf::RenderWindow& window);
    void handleSettingsEvent(const sf::Event& event,
                             sf::RenderWindow& window,
                             MenuResult& result);

    // Helpers
    void setupButton(MenuButton& btn,
                     const std::string& texPath,
                     const std::string& label,
                     float x, float y,
                     const std::string& hoverTexPath = "",
                     unsigned int charSize = 26);
    void updateButton(MenuButton& btn,
                      sf::Vector2f mousePos,
                      float dt);
    void drawButton(sf::RenderWindow& w, MenuButton& btn);

    void setupLoadMenu();
    void refreshSaveSlots();
    void drawLoadMenu(sf::RenderWindow& window);
    void handleLoadEvent(const sf::Event& event,
                         sf::RenderWindow& window,
                         MenuResult& result);
    void drawNewGameMenu(sf::RenderWindow& window);
    void handleNewGameEvent(const sf::Event& event,
                            sf::RenderWindow& window,
                            MenuResult& result);

public:
    Menu();
    ~Menu();
    void handleEvent(const sf::Event& event,
                     sf::RenderWindow& window,
                     MenuResult& result);
    void update(float dt, sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

    //Getter & Setter
    const std::string& getPendingSaveName() const { return mNewGameName; }
    MenuScreen getScreen() const { return mScreen; }
    void setScreen(MenuScreen s) { mScreen = s; }

    void drawShopMenu(sf::RenderWindow& window, int currentHp = 3, int maxHp = 3);
    void handleShopEvent(const sf::Event& event,
                         sf::RenderWindow& window,
                         MenuResult& result,
                         int* currentHpPtr = nullptr,
                         int maxHp = 3);

    void setTwoPlayerShop(bool is2P) { mIsTwoPlayerShop = is2P; }
    bool isTwoPlayerShop() const { return mIsTwoPlayerShop; }

private:
    bool mIsTwoPlayerShop = false;
public:


    float getMusicVolume() const { return mMuteAll ? 0.f : mMusicSlider.value; }
    float getSFXVolume()   const { return mMuteAll ? 0.f : mSFXSlider.value;   }
    bool  getMuteAll()     const { return mMuteAll; }

    // Để CGAME set volume từ pause
    void setMusicVolume(float v) {
        mMusicSlider.value = std::max(0.f, std::min(100.f, v));
        if (!mMuteAll) mMusic.setVolume(mMusicSlider.value);
    }
    void setSFXVolume(float v) {
        mSFXSlider.value = std::max(0.f, std::min(100.f, v));
    }
    void setMuteAll(bool mute) {
        mMuteAll = mute;
        mMusic.setVolume(mute ? 0.f : mMusicSlider.value);
    }

    void playMusic() {
        if (!mMuteAll && mMusic.getStatus() != sf::Music::Playing) {
            mMusic.setVolume(mMusicSlider.value);
            mMusic.play();
        }
    }
    void stopMusic() {
        mMusic.stop();
    }
};
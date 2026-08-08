#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <vector>
#include "SaveData.h"
#include "Animation.h"
#include "TextureManager.h"
#include "MenuButton.h"

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
    SETTING
};

enum class MenuScreen {
    MAIN,
    NEW_GAME_SELECT,
    LOAD,
    SETTINGS,
    INFO,
    SHOP
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


    // Buttons
    MenuButton mBtnNew;
    MenuButton mBtnLoad;
    MenuButton mBtnQuit;
    MenuButton mBtnSetting;
    MenuButton mBtnBack;

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
    sf::Text mInfoTitle;
    sf::Text mInfoControlsTitle;
    sf::Text mInfoControlsText[4];
    sf::Text mInfoGameplayTitle;
    sf::Text mInfoGameplayText[3];
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
    sf::Texture mItemTextures[4];
    sf::Sprite  mItemSprites[4];
    sf::Text mItemTitleTexts[4];
    sf::Text mItemDescTexts[4];
    sf::Text mItemPriceTexts[4];
    MenuButton mItemBuyButtons[4];
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

    void drawShopMenu(sf::RenderWindow& window);
    void handleShopEvent(const sf::Event& event,
                         sf::RenderWindow& window,
                         MenuResult& result);


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
};
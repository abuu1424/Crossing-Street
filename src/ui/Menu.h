#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include "SaveData.h"
#include <vector>

enum class MenuResult {
    NONE,
    NEW_GAME,
    LOAD_SLOT_1,
    LOAD_SLOT_2,
    LOAD_SLOT_3,
    QUIT,
    SETTING
};

enum class MenuScreen {
    MAIN,
    LOAD,
    SETTINGS
};

struct MenuButton {
    sf::Sprite      sprite;
    sf::Texture     texture;
    sf::Text        label;
    float           baseScale  = 1.f;   // scale bình thường
    float           hoverScale = 1.15f; // scale khi hover
    float           curScale   = 1.f;   // scale hiện tại
    bool            hovered    = false;
};

class Menu
{
    // Background
    sf::Texture          mBgTexture;
    sf::Sprite           mBgSprite;

    // Title
    sf::Font             mFont;
    sf::Text             mTitle;

    // Buttons
    MenuButton           mBtnNew;
    MenuButton           mBtnLoad;
    MenuButton           mBtnQuit;
    MenuButton           mBtnSetting;

    // Load menu
    MenuScreen           mScreen = MenuScreen::MAIN;
    std::vector<SaveSlot> mSaveSlots;

    sf::Text             mLoadTitle;
    sf::Text             mSlotTexts[3];
    sf::Text             mBackText;

    // Nhạc nền
    sf::Music            mMusic;

    //SETTINGS
    struct Slider {
        float        value     = 50.f;   // 0-100
        float        x, y, w;           // vị trí và chiều rộng
        bool         dragging  = false;
        sf::RectangleShape track;
        sf::RectangleShape thumb;
        sf::Text     label;
        sf::Text     valueText;
    };

    Slider mMusicSlider;
    Slider mSFXSlider;
    bool   mMuteAll   = false;

    sf::Text           mSettingsTitle;
    sf::Text           mMuteText;
    sf::Text           mBackSettingsText;

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
                     float x, float y);
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

public:
    Menu();
    void handleEvent(const sf::Event& event,
                     sf::RenderWindow& window,
                     MenuResult& result);
    void update(float dt, sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

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
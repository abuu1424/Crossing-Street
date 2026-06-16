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
    LOAD
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

class Menu {
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
};
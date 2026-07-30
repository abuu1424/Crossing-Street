#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include "SaveData.h"
#include <vector>
#include "Animation.h"

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
    INFO
};

struct MenuButton {
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Texture textureHover;
    sf::Text label;
    float baseScale  = 1.f;   // scale bình thường
    float hoverScale = 1.15f; // scale khi hover
    float curScale   = 1.f;   // scale hiện tại
    bool  hovered    = false;
    bool  hasHoverTex = false;

    void setup(const std::string &texPath, const std::string &textLabel,
               sf::Font &font, float x, float y,
               const std::string &hoverTexPath = "", unsigned int charSize = 26) {
        if (!texture.loadFromFile(texPath)) {
            printf("No img, using fallback: %s\n", texPath.c_str());
            sf::Image img;
            img.create(160, 50, sf::Color(60, 60, 80, 220));
            texture.loadFromImage(img);
        }

        hasHoverTex = false;
        if (!hoverTexPath.empty() && textureHover.loadFromFile(hoverTexPath)) {
            hasHoverTex = true;
        }

        sprite.setTexture(texture);
        float w = texture.getSize().x;
        float h = texture.getSize().y;
        sprite.setOrigin(w / 2.f, h / 2.f);
        sprite.setPosition(x, y);
        sprite.setScale(baseScale, baseScale);

        label.setFont(font);
        label.setString(textLabel);
        label.setCharacterSize(charSize);
        label.setFillColor(sf::Color::White);
        label.setOutlineColor(sf::Color::Black);
        label.setOutlineThickness(2.f);

        sf::FloatRect lb = label.getLocalBounds();
        label.setOrigin(lb.left + lb.width / 2.f, lb.top + lb.height / 2.f);
        label.setPosition(x, y);
    }

    void update(sf::Vector2f mousePos, float dt) {
        sf::FloatRect bounds = sprite.getGlobalBounds();
        hovered = bounds.contains(mousePos);

        if (hasHoverTex) {
            sprite.setTexture(hovered ? textureHover : texture);
        }

        float target = hovered ? hoverScale : baseScale;
        curScale += (target - curScale) * 12.f * dt;

        sprite.setScale(curScale, curScale);
        label.setScale(curScale, curScale);
    }

    void draw(sf::RenderWindow &w) const {
        w.draw(sprite);
        w.draw(label);
    }

    bool contains(sf::Vector2f pos) const {
        return sprite.getGlobalBounds().contains(pos);
    }
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
    Animation *mTitleAnim = nullptr;


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

    //Getter
    const std::string& getPendingSaveName() const { return mNewGameName; }


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
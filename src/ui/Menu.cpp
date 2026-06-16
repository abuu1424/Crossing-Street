#include "Menu.h"
#include "Utils.h"
#include <cstdio>
#include <cmath>

Menu::Menu() {
    // Font
    if (!mFont.loadFromFile("assets/font/pixel_operator/PixelOperator.ttf"))
        printf("FAILED font\n");

    // Background
    if (!mBgTexture.loadFromFile("assets/ui/menu/menu_bg.png"))
        printf("FAILED menu bg\n");
    mBgSprite.setTexture(mBgTexture);
    mBgSprite.setScale(
        (float)Win_W / mBgTexture.getSize().x,
        (float)Win_H / mBgTexture.getSize().y
    );

    // Title
    mTitle.setFont(mFont);
    mTitle.setString("CROSSING STREET");
    mTitle.setCharacterSize(64);
    mTitle.setFillColor(sf::Color(255, 215, 0));
    sf::FloatRect tb = mTitle.getLocalBounds();
    mTitle.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
    mTitle.setPosition(Win_W / 2.f, 160.f);

    // Buttons
    float btnY = 280.f;
    float gap  = 80.f;
    setupButton(mBtnNew,     "assets/ui/menu/btn_newgame.png",  "NEW GAME",  Win_W / 2.f, btnY);
    setupButton(mBtnLoad,    "assets/ui/menu/btn_loadgame.png", "LOAD GAME", Win_W / 2.f, btnY + gap);
    setupButton(mBtnQuit,    "assets/ui/menu/btn_quit.png",     "QUIT",      Win_W / 2.f, btnY + gap * 2);
    setupButton(mBtnSetting, "assets/ui/menu/btn_setting.png",  "SETTINGS",  Win_W / 2.f, btnY + gap * 3);

    setupLoadMenu();

    // Nhạc nền
    if (!mMusic.openFromFile("assets/sounds/menu/menu_music.ogg"))
        printf("FAILED music\n");
    mMusic.setLoop(true);
    mMusic.setVolume(50.f);
    mMusic.play();
}

void Menu::setupButton(MenuButton& btn,
                       const std::string& texPath,
                       const std::string& label,
                       float x, float y) {
    if (!btn.texture.loadFromFile(texPath)) {
        printf("No img, using fallback: %s\n", texPath.c_str());

        sf::Image img;
        img.create(300, 60, sf::Color(60, 60, 80, 220));
        btn.texture.loadFromImage(img);
    }

    btn.sprite.setTexture(btn.texture);

    float w = btn.texture.getSize().x;
    float h = btn.texture.getSize().y;
    btn.sprite.setOrigin(w / 2.f, h / 2.f);
    btn.sprite.setPosition(x, y);
    btn.sprite.setScale(btn.baseScale, btn.baseScale);

    btn.label.setFont(mFont);
    btn.label.setString(label);
    btn.label.setCharacterSize(28);
    btn.label.setFillColor(sf::Color::White);

    sf::FloatRect lb = btn.label.getLocalBounds();
    btn.label.setOrigin(lb.left + lb.width / 2.f, lb.top + lb.height / 2.f);
    btn.label.setPosition(x, y);
}

void Menu::setupLoadMenu() {
    mLoadTitle.setFont(mFont);
    mLoadTitle.setString("LOAD GAME");
    mLoadTitle.setCharacterSize(56);
    mLoadTitle.setFillColor(sf::Color(255, 215, 0));

    sf::FloatRect tb = mLoadTitle.getLocalBounds();
    mLoadTitle.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
    mLoadTitle.setPosition(Win_W / 2.f, 120.f);

    for (int i = 0; i < 3; i++) {
        mSlotTexts[i].setFont(mFont);
        mSlotTexts[i].setCharacterSize(32);
        mSlotTexts[i].setFillColor(sf::Color::White);
        mSlotTexts[i].setPosition(Win_W / 2.f - 260.f, 240.f + i * 90.f);
    }

    mBackText.setFont(mFont);
    mBackText.setString("BACK");
    mBackText.setCharacterSize(32);
    mBackText.setFillColor(sf::Color(220, 220, 220));
    mBackText.setPosition(Win_W / 2.f - 60.f, 560.f);
}

void Menu::refreshSaveSlots() {
    mSaveSlots = SaveData::getAllSlots();

    for (int i = 0; i < 3; i++) {
        if (mSaveSlots[i].isEmpty) {
            mSlotTexts[i].setString("Slot " + std::to_string(i + 1) + ": Empty");
        }
        else {
            std::string name = mSaveSlots[i].saveName.empty()
                ? "Unnamed Save"
                : mSaveSlots[i].saveName;

            mSlotTexts[i].setString(
                "Slot " + std::to_string(i + 1) + ": " + name +
                " | Lv " + std::to_string(mSaveSlots[i].level) +
                " | Score " + std::to_string(mSaveSlots[i].score)
            );
        }
    }
}

void Menu::updateButton(MenuButton& btn,
                        sf::Vector2f mousePos,
                        float dt) {
    sf::FloatRect bounds = btn.sprite.getGlobalBounds();
    btn.hovered = bounds.contains(mousePos);

    float target = btn.hovered ? btn.hoverScale : btn.baseScale;
    btn.curScale += (target - btn.curScale) * 10.f * dt;

    btn.sprite.setScale(btn.curScale, btn.curScale);
    btn.label.setScale(btn.curScale, btn.curScale);
}

void Menu::handleLoadEvent(const sf::Event& event,
                           sf::RenderWindow& window,
                           MenuResult& result) {
    if (event.type != sf::Event::MouseButtonPressed ||
        event.mouseButton.button != sf::Mouse::Left) {
        return;
    }

    sf::Vector2f mouse = window.mapPixelToCoords(
        sf::Vector2i(event.mouseButton.x, event.mouseButton.y)
    );

    for (int i = 0; i < 3; i++) {
        if (mSlotTexts[i].getGlobalBounds().contains(mouse)) {
            if (!mSaveSlots[i].isEmpty) {
                if (i == 0) result = MenuResult::LOAD_SLOT_1;
                else if (i == 1) result = MenuResult::LOAD_SLOT_2;
                else result = MenuResult::LOAD_SLOT_3;

                mMusic.stop();
            }
            return;
        }
    }

    if (mBackText.getGlobalBounds().contains(mouse)) {
        mScreen = MenuScreen::MAIN;
    }
}

void Menu::handleEvent(const sf::Event& event,
                       sf::RenderWindow& window,
                       MenuResult& result) {
    if (mScreen == MenuScreen::LOAD) {
        handleLoadEvent(event, window, result);
        return;
    }

    if (event.type == sf::Event::MouseButtonPressed
        && event.mouseButton.button == sf::Mouse::Left) {

        sf::Vector2f mouse = window.mapPixelToCoords(
            sf::Vector2i(event.mouseButton.x, event.mouseButton.y)
        );

        if (mBtnNew.sprite.getGlobalBounds().contains(mouse)) {
            mMusic.stop();
            result = MenuResult::NEW_GAME;
        }
        else if (mBtnLoad.sprite.getGlobalBounds().contains(mouse)) {
            refreshSaveSlots();
            mScreen = MenuScreen::LOAD;
        }
        else if (mBtnQuit.sprite.getGlobalBounds().contains(mouse)) {
            result = MenuResult::QUIT;
        }
        else if (mBtnSetting.sprite.getGlobalBounds().contains(mouse)) {
            mMusic.stop();
            result = MenuResult::SETTING;
        }
    }
}

void Menu::update(float dt, sf::RenderWindow& window) {
    if (mScreen == MenuScreen::LOAD) {
        return;
    }

    sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    updateButton(mBtnNew,  mouse, dt);
    updateButton(mBtnLoad, mouse, dt);
    updateButton(mBtnSetting, mouse, dt);
    updateButton(mBtnQuit, mouse, dt);
}

void Menu::drawButton(sf::RenderWindow& w, MenuButton& btn) {
    w.draw(btn.sprite);
    w.draw(btn.label);
}

void Menu::drawLoadMenu(sf::RenderWindow& window) {
    window.draw(mBgSprite);
    window.draw(mLoadTitle);

    for (int i = 0; i < 3; i++) {
        window.draw(mSlotTexts[i]);
    }

    window.draw(mBackText);
}

void Menu::draw(sf::RenderWindow& window) {
    if (mScreen == MenuScreen::LOAD) {
        drawLoadMenu(window);
        return;
    }

    window.draw(mBgSprite);
    window.draw(mTitle);
    drawButton(window, mBtnNew);
    drawButton(window, mBtnLoad);
    drawButton(window, mBtnSetting);
    drawButton(window, mBtnQuit);
}
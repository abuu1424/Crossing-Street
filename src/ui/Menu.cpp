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
    mTitle.setFillColor(sf::Color(255, 215, 0));  // vàng
    sf::FloatRect tb = mTitle.getLocalBounds();
    mTitle.setOrigin(tb.left + tb.width/2.f, tb.top + tb.height/2.f);
    mTitle.setPosition(Win_W / 2.f, 160.f);

    // Buttons — căn giữa màn hình
    float btnY = 280.f;
    float gap  = 80.f;
    setupButton(mBtnNew,  "assets/ui/menu/btn_newgame.png",  "NEW GAME",  Win_W/2.f, btnY);
    setupButton(mBtnLoad, "assets/ui/menu/btn_loadgame.png", "LOAD GAME", Win_W/2.f, btnY + gap);
    setupButton(mBtnQuit, "assets/ui/menu/btn_quit.png",     "QUIT",      Win_W/2.f, btnY + gap*2);
    setupButton(mBtnSetting, "assets/ui/menu/btn_setting.png", "SETTINGS", Win_W/2.f, btnY + gap*3);


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
    if (!btn.texture.loadFromFile(texPath))
        printf("FAILED btn: %s\n", texPath.c_str());

    btn.sprite.setTexture(btn.texture);

    // Căn giữa origin
    float w = btn.texture.getSize().x;
    float h = btn.texture.getSize().y;
    btn.sprite.setOrigin(w/2.f, h/2.f);
    btn.sprite.setPosition(x, y);
    btn.sprite.setScale(btn.baseScale, btn.baseScale);

    // Label text
    btn.label.setFont(mFont);
    btn.label.setString(label);
    btn.label.setCharacterSize(28);
    btn.label.setFillColor(sf::Color::White);
    sf::FloatRect lb = btn.label.getLocalBounds();
    btn.label.setOrigin(lb.left + lb.width/2.f, lb.top + lb.height/2.f);
    btn.label.setPosition(x, y);
}

void Menu::updateButton(MenuButton& btn,
                        sf::Vector2f mousePos,
                        float dt) {
    // Kiểm tra hover
    sf::FloatRect bounds = btn.sprite.getGlobalBounds();
    btn.hovered = bounds.contains(mousePos);

    // Scale mượt — lerp về target
    float target = btn.hovered ? btn.hoverScale : btn.baseScale;
    btn.curScale += (target - btn.curScale) * 10.f * dt;  // lerp tốc độ 10

    btn.sprite.setScale(btn.curScale, btn.curScale);

    // Label đi theo sprite
    btn.label.setScale(btn.curScale, btn.curScale);
}

void Menu::handleEvent(const sf::Event& event,
                       sf::RenderWindow& window,
                       MenuResult& result) {
    if (event.type == sf::Event::MouseButtonPressed
        && event.mouseButton.button == sf::Mouse::Left) {

        sf::Vector2f mouse = window.mapPixelToCoords(
            sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

        if (mBtnNew.sprite.getGlobalBounds().contains(mouse)) {
            mMusic.stop();
            result = MenuResult::NEW_GAME;
        }
        else if (mBtnLoad.sprite.getGlobalBounds().contains(mouse)) {
            mMusic.stop();
            result = MenuResult::LOAD_GAME;
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

void Menu::draw(sf::RenderWindow& window) {
    window.draw(mBgSprite);
    window.draw(mTitle);
    drawButton(window, mBtnNew);
    drawButton(window, mBtnLoad);
    drawButton(window, mBtnSetting);
    drawButton(window, mBtnQuit);
}
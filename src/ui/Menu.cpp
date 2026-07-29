#include "Menu.h"
#include "Utils.h"
#include <cmath>
#include <cstdio>

Menu::Menu() {
  // Font
  if (!mFont.loadFromFile(Font_Path))
    printf("FAILED font\n");

  // Background
  if (!mBgTexture.loadFromFile("assets/ui/menu/menu_bg.png"))
    printf("FAILED menu bg\n");
  mBgSprite.setTexture(mBgTexture);
  mBgSprite.setScale((float)Win_W / mBgTexture.getSize().x,
                     (float)Win_H / mBgTexture.getSize().y);

  // Title

  if (mTitleTexture.loadFromFile("assets/ui/menu/title.png")) {
    printf("OK: %dx%d\n", mTitleTexture.getSize().x, mTitleTexture.getSize().y);
    mTitleSprite.setTexture(mTitleTexture);
    mTitleSprite.setTextureRect(sf::IntRect(0, 0, 350, 40));
    mTitleSprite.setScale(3.f, 3.f);
    mTitleSprite.setOrigin(350 / 2.f, 40 / 2.f);
    mTitleSprite.setPosition(Win_W / 2.f, 120.f);
    mTitleAnim = new Animation(mTitleSprite, mTitleTexture, 350, 40, 4, 4,
                               0.12f, // frame
                               true   // Loop
    );
  } else {
    printf("FAILED TO LOAD TITLE");
    mTitle.setFont(mFont);
    mTitle.setString("CROSSING STREET");
    mTitle.setCharacterSize(64);
    mTitle.setFillColor(sf::Color(255, 215, 0));
    sf::FloatRect tb = mTitle.getLocalBounds();
    mTitle.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
    mTitle.setPosition(Win_W / 2.f, 160.f);
  }

  // Buttons
  float btnY = 280.f;
  float gap = 80.f;
  setupButton(mBtnNew, "assets/ui/menu/btn_newgame.png", "NEW GAME",
              Win_W / 2.f, btnY);
  setupButton(mBtnLoad, "assets/ui/menu/btn_loadgame.png", "LOAD GAME",
              Win_W / 2.f, btnY + gap);
  setupButton(mBtnQuit, "assets/ui/menu/btn_quit.png", "QUIT", Win_W / 2.f,
              btnY + gap * 2);
  setupButton(mBtnSetting, "assets/ui/menu/btn_setting.png", "SETTINGS",
              Win_W / 2.f, btnY + gap * 3);

  setupLoadMenu();
  setupSettingsMenu();
  setupNewGameNamePopup();

  // Nhạc nền
  if (!mMusic.openFromFile("assets/sounds/menu/menu_music.ogg"))
    printf("FAILED music\n");
  mMusic.setLoop(true);
  mMusic.setVolume(50.f);
  mMusic.play();
}

Menu::~Menu() { delete mTitleAnim; }

void Menu::setupButton(MenuButton &btn, const std::string &texPath,
                       const std::string &label, float x, float y) {
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
  btn.label.setOutlineColor(sf::Color::Black);
  btn.label.setOutlineThickness(2.f);

  sf::FloatRect lb = btn.label.getLocalBounds();
  btn.label.setOrigin(lb.left + lb.width / 2.f, lb.top + lb.height / 2.f);
  btn.label.setPosition(x, y);
}

void Menu::setupLoadMenu() {
  mLoadTitle.setFont(mFont);
  mLoadTitle.setString("LOAD GAME");
  mLoadTitle.setCharacterSize(52);
  mLoadTitle.setFillColor(sf::Color(255, 215, 0));
  mLoadTitle.setOutlineColor(sf::Color::Black);
  mLoadTitle.setOutlineThickness(3.f);

  if (!mPanelTexture.loadFromFile("assets/ui/menu/popup_panel.png")) {
    printf("FAILED popup_panel.png\n");
  }
  mPanelSprite.setTexture(mPanelTexture);
  sf::Vector2u pSize = mPanelTexture.getSize();
  if (pSize.x > 0 && pSize.y > 0) {
    mPanelSprite.setOrigin(pSize.x / 2.f, pSize.y / 2.f);
    mPanelSprite.setPosition(Win_W / 2.f, Win_H / 2.f + 20.f);
  }

  if (!mSlotFrameTexture.loadFromFile("assets/ui/menu/slot_frame.png")) {
    printf("FAILED slot_frame.png\n");
  }
  if (!mSlotFrameHoverTexture.loadFromFile(
          "assets/ui/menu/slot_frame_hover.png")) {
    printf("FAILED slot_frame_hover.png\n");
  }

  sf::Vector2u slotSize = mSlotFrameTexture.getSize();
  for (int i = 0; i < 3; i++) {
    mSlotFrameSprites[i].setTexture(mSlotFrameTexture);
    if (slotSize.x > 0 && slotSize.y > 0) {
      mSlotFrameSprites[i].setOrigin(slotSize.x / 2.f, slotSize.y / 2.f);
      mSlotFrameSprites[i].setPosition(Win_W / 2.f, 230.f + i * 85.f);
    }

    mSlotTexts[i].setFont(mFont);
    mSlotTexts[i].setCharacterSize(24);
    mSlotTexts[i].setFillColor(sf::Color::White);
    mSlotTexts[i].setOutlineColor(sf::Color::Black);
    mSlotTexts[i].setOutlineThickness(1.5f);
  }

  sf::FloatRect tb = mLoadTitle.getLocalBounds();
  mLoadTitle.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
  mLoadTitle.setPosition(Win_W / 2.f, 135.f);

  mBackText.setFont(mFont);
  mBackText.setString("BACK");
  mBackText.setCharacterSize(28);
  mBackText.setOutlineColor(sf::Color::Black);
  mBackText.setOutlineThickness(2.f);
  mBackText.setFillColor(sf::Color(220, 220, 220));
  sf::FloatRect bb = mBackText.getLocalBounds();
  mBackText.setOrigin(bb.left + bb.width / 2.f, bb.top + bb.height / 2.f);
  mBackText.setPosition(Win_W / 2.f, 520.f);

  mOverwriteHint.setFont(mFont);
  mOverwriteHint.setCharacterSize(20);
  mOverwriteHint.setFillColor(sf::Color(255, 90, 90));
  mOverwriteHint.setOutlineColor(sf::Color::Black);
  mOverwriteHint.setOutlineThickness(2.f);
  mOverwriteHint.setString(">> Click again to overwrite this slot <<");
  sf::FloatRect ob = mOverwriteHint.getLocalBounds();
  mOverwriteHint.setOrigin(ob.left + ob.width / 2.f, ob.top + ob.height / 2.f);
  mOverwriteHint.setPosition(Win_W / 2.f, 485.f);
}

void Menu::setupNewGameNamePopup() {
  if (!mNameBoxTexture.loadFromFile("assets/ui/menu/popup_namebox.png")) {
    printf("FAILED popup_namebox.png\n");
  }
  mNameBoxSprite.setTexture(mNameBoxTexture);
  sf::Vector2u texSize = mNameBoxTexture.getSize();

  const float BOX_CX = Win_W / 2.f;
  const float BOX_CY = Win_H / 2.f + 20.f;

  if (texSize.x > 0 && texSize.y > 0) {
    mNameBoxSprite.setOrigin(texSize.x / 2.f, texSize.y / 2.f);
    mNameBoxSprite.setPosition(BOX_CX, BOX_CY);
  }

  mNameTitle.setFont(mFont);
  mNameTitle.setString("Enter file save name:");
  mNameTitle.setCharacterSize(26);
  mNameTitle.setFillColor(sf::Color::White);
  mNameTitle.setOutlineColor(sf::Color::Black);
  mNameTitle.setOutlineThickness(2.f);
  sf::FloatRect tb = mNameTitle.getLocalBounds();
  mNameTitle.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
  mNameTitle.setPosition(BOX_CX, BOX_CY - 115.f);

  mNameInput.setFont(mFont);
  mNameInput.setCharacterSize(28);
  mNameInput.setFillColor(sf::Color(255, 215, 0));
  mNameInput.setOutlineColor(sf::Color::Black);
  mNameInput.setOutlineThickness(1.5f);
  mNameInput.setPosition(BOX_CX, BOX_CY - 8.f);

  mNameHint.setFont(mFont);
  mNameHint.setString("ENTER: START GAME   |   ESC: Cancel");
  mNameHint.setCharacterSize(18);
  mNameHint.setFillColor(sf::Color(220, 220, 220));
  mNameHint.setOutlineColor(sf::Color::Black);
  mNameHint.setOutlineThickness(1.f);
  sf::FloatRect hb = mNameHint.getLocalBounds();
  mNameHint.setOrigin(hb.left + hb.width / 2.f, hb.top + hb.height / 2.f);
  mNameHint.setPosition(BOX_CX, BOX_CY + 80.f);
}

void Menu::refreshSaveSlots() {
  mSaveSlots = SaveData::getAllSlots();
  for (int i = 0; i < 3; i++) {
    if (mSaveSlots[i].isEmpty) {
      mSlotTexts[i].setString("Slot " + std::to_string(i + 1) + ": Empty");
    } else {
      std::string name = mSaveSlots[i].saveName.empty()
                             ? "Unnamed Save"
                             : mSaveSlots[i].saveName;
      mSlotTexts[i].setString("Slot " + std::to_string(i + 1) + ": " + name +
                              "  |  Lv " + std::to_string(mSaveSlots[i].level) +
                              "  |  Score " +
                              std::to_string(mSaveSlots[i].score));
    }
    sf::FloatRect sb = mSlotTexts[i].getLocalBounds();
    mSlotTexts[i].setOrigin(sb.left + sb.width / 2.f, sb.top + sb.height / 2.f);
    mSlotTexts[i].setPosition(Win_W / 2.f + 25.f, 230.f + i * 85.f);
  }
}

void Menu::updateButton(MenuButton &btn, sf::Vector2f mousePos, float dt) {
  sf::FloatRect bounds = btn.sprite.getGlobalBounds();
  btn.hovered = bounds.contains(mousePos);

  float target = btn.hovered ? btn.hoverScale : btn.baseScale;
  btn.curScale += (target - btn.curScale) * 10.f * dt;

  btn.sprite.setScale(btn.curScale, btn.curScale);
  btn.label.setScale(btn.curScale, btn.curScale);
}

void Menu::handleLoadEvent(const sf::Event &event, sf::RenderWindow &window,
                           MenuResult &result) {
  if (event.type != sf::Event::MouseButtonPressed ||
      event.mouseButton.button != sf::Mouse::Left) {
    return;
  }

  sf::Vector2f mouse = window.mapPixelToCoords(
      sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

  for (int i = 0; i < 3; i++) {
    if (mSlotFrameSprites[i].getGlobalBounds().contains(mouse) ||
        mSlotTexts[i].getGlobalBounds().contains(mouse)) {
      if (!mSaveSlots[i].isEmpty) {
        if (i == 0)
          result = MenuResult::LOAD_SLOT_1;
        else if (i == 1)
          result = MenuResult::LOAD_SLOT_2;
        else
          result = MenuResult::LOAD_SLOT_3;

        mMusic.stop();
      }
      return;
    }
  }

  if (mBackText.getGlobalBounds().contains(mouse)) {
    mScreen = MenuScreen::MAIN;
  }
}

void Menu::handleEvent(const sf::Event &event, sf::RenderWindow &window,
                       MenuResult &result) {
  if (mScreen == MenuScreen::NEW_GAME_SELECT) {
    handleNewGameEvent(event, window, result);
    return;
  }
  if (mScreen == MenuScreen::LOAD) {
    handleLoadEvent(event, window, result);
    return;
  }
  if (mScreen == MenuScreen::SETTINGS) {
    handleSettingsEvent(event, window, result);
    return;
  }

  if (event.type == sf::Event::MouseButtonPressed &&
      event.mouseButton.button == sf::Mouse::Left) {

    sf::Vector2f mouse = window.mapPixelToCoords(
        sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

    if (mBtnNew.sprite.getGlobalBounds().contains(mouse)) {
      refreshSaveSlots();
      mConfirmOverwriteSlot = -1;
      mEnteringNewGameName = false;
      mPendingNewGameSlot = -1;
      mNewGameName.clear();
      mScreen = MenuScreen::NEW_GAME_SELECT;
    } else if (mBtnLoad.sprite.getGlobalBounds().contains(mouse)) {
      refreshSaveSlots();
      mScreen = MenuScreen::LOAD;
    } else if (mBtnQuit.sprite.getGlobalBounds().contains(mouse)) {
      result = MenuResult::QUIT;
    } else if (mBtnSetting.sprite.getGlobalBounds().contains(mouse)) {
      mScreen = MenuScreen::SETTINGS;
    }
  }
}

void Menu::handleNewGameEvent(const sf::Event &event, sf::RenderWindow &window,
                              MenuResult &result) {
  if (mEnteringNewGameName) {
    handleNewGameNameEvent(event, result);
    return;
  }

  if (event.type != sf::Event::MouseButtonPressed ||
      event.mouseButton.button != sf::Mouse::Left) {
    return;
  }

  sf::Vector2f mouse = window.mapPixelToCoords(
      sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

  for (int i = 0; i < 3; i++) {
    if (mSlotFrameSprites[i].getGlobalBounds().contains(mouse) ||
        mSlotTexts[i].getGlobalBounds().contains(mouse)) {
      if (mSaveSlots[i].isEmpty) {
        mPendingNewGameSlot = i + 1;
        mNewGameName.clear();
        mEnteringNewGameName = true;
      } else if (mConfirmOverwriteSlot == i) {
        mPendingNewGameSlot = i + 1;
        mNewGameName.clear();
        mEnteringNewGameName = true;
        mConfirmOverwriteSlot = -1;
      } else {
        mConfirmOverwriteSlot = i;
        refreshSaveSlots();
      }
      return;
    }
  }
  if (mBackText.getGlobalBounds().contains(mouse)) {
    mConfirmOverwriteSlot = -1;
    mScreen = MenuScreen::MAIN;
  }
}

void Menu::handleNewGameNameEvent(const sf::Event &event, MenuResult &result) {
  if (event.type == sf::Event::TextEntered) {
    if (event.text.unicode >= 32 && event.text.unicode < 128 &&
        mNewGameName.size() < 20) {
      mNewGameName += static_cast<char>(event.text.unicode);
    }
  }
  if (event.type == sf::Event::KeyPressed) {
    if (event.key.code == sf::Keyboard::BackSpace && !mNewGameName.empty()) {
      mNewGameName.pop_back();
    } else if (event.key.code == sf::Keyboard::Enter) {
      if (mNewGameName.empty()) {
        mNewGameName = "Save Slot " + std::to_string(mPendingNewGameSlot);
      }
      if (mPendingNewGameSlot == 1)
        result = MenuResult::NEW_GAME_SLOT_1;
      else if (mPendingNewGameSlot == 2)
        result = MenuResult::NEW_GAME_SLOT_2;
      else if (mPendingNewGameSlot == 3)
        result = MenuResult::NEW_GAME_SLOT_3;
      mEnteringNewGameName = false;
      mMusic.stop();
    } else if (event.key.code == sf::Keyboard::Escape) {
      mEnteringNewGameName = false;
      mPendingNewGameSlot = -1;
      mNewGameName.clear();
    }
  }
}

void Menu::update(float dt, sf::RenderWindow &window) {
  sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));

  if (mScreen == MenuScreen::LOAD || mScreen == MenuScreen::NEW_GAME_SELECT) {
    if (!mEnteringNewGameName) {
      for (int i = 0; i < 3; i++) {
        if (mSlotFrameSprites[i].getGlobalBounds().contains(mouse)) {
          mSlotFrameSprites[i].setTexture(mSlotFrameHoverTexture);
        } else {
          mSlotFrameSprites[i].setTexture(mSlotFrameTexture);
        }
      }
    }
    return;
  }
  if (mScreen == MenuScreen::SETTINGS)
    return;

  if (mTitleAnim)
    mTitleAnim->update(dt);
  updateButton(mBtnNew, mouse, dt);
  updateButton(mBtnLoad, mouse, dt);
  updateButton(mBtnSetting, mouse, dt);
  updateButton(mBtnQuit, mouse, dt);
}

void Menu::drawButton(sf::RenderWindow &w, MenuButton &btn) {
  w.draw(btn.sprite);
  w.draw(btn.label);
}

void Menu::drawLoadMenu(sf::RenderWindow &window) {
  mLoadTitle.setString("LOAD GAME");
  sf::FloatRect tb = mLoadTitle.getLocalBounds();
  mLoadTitle.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
  window.draw(mBgSprite);
  window.draw(mPanelSprite);
  window.draw(mLoadTitle);
  for (int i = 0; i < 3; i++) {
    window.draw(mSlotFrameSprites[i]);
    window.draw(mSlotTexts[i]);
  }
  window.draw(mBackText);
}

void Menu::drawNewGameMenu(sf::RenderWindow &window) {
  window.draw(mBgSprite);

  if (mEnteringNewGameName) {
    // Popup nhap ten dang mo -> AN het bang chon slot,
    // chi ve popup nhap ten len tren nen game.
    drawNewGameNamePopup(window);
    return;
  }

  // Khong co popup -> ve binh thuong bang chon slot (nhu cu)
  mLoadTitle.setString("NEW GAME - CHOOSE FILE SAVE");
  sf::FloatRect tb = mLoadTitle.getLocalBounds();
  mLoadTitle.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
  window.draw(mPanelSprite);
  window.draw(mLoadTitle);
  for (int i = 0; i < 3; i++) {
    window.draw(mSlotFrameSprites[i]);
    window.draw(mSlotTexts[i]);
  }

  if (mConfirmOverwriteSlot != -1) {
    window.draw(mOverwriteHint);
  }

  window.draw(mBackText);
}

void Menu::drawNewGameNamePopup(sf::RenderWindow &window) {
  mNameInput.setString(mNewGameName + "_");
  sf::FloatRect ib = mNameInput.getLocalBounds();
  mNameInput.setOrigin(ib.left + ib.width / 2.f, ib.top + ib.height / 2.f);
  mNameInput.setPosition(Win_W / 2.f, Win_H / 2.f + 12.f);

  window.draw(mNameBoxSprite);
  window.draw(mNameTitle);
  window.draw(mNameInput);
  window.draw(mNameHint);
}

void Menu::draw(sf::RenderWindow &window) {
  if (mScreen == MenuScreen::NEW_GAME_SELECT) {
    drawNewGameMenu(window);
    return;
  }
  if (mScreen == MenuScreen::LOAD) {
    drawLoadMenu(window);
    return;
  }
  if (mScreen == MenuScreen::SETTINGS) {
    drawSettingsMenu(window);
    return;
  }
  window.draw(mBgSprite);
  if (mTitleAnim)
    window.draw(mTitleSprite);
  else
    window.draw(mTitle);
  drawButton(window, mBtnNew);
  drawButton(window, mBtnLoad);
  drawButton(window, mBtnSetting);
  drawButton(window, mBtnQuit);
}

void Menu::setupSettingsMenu() {
  // Title
  mSettingsTitle.setFont(mFont);
  mSettingsTitle.setString("SETTINGS");
  mSettingsTitle.setCharacterSize(52);
  mSettingsTitle.setFillColor(sf::Color(255, 215, 0));
  mSettingsTitle.setOutlineColor(sf::Color::Black);
  mSettingsTitle.setOutlineThickness(3.f);

  sf::FloatRect tb = mSettingsTitle.getLocalBounds();
  mSettingsTitle.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
  mSettingsTitle.setPosition(Win_W / 2.f, 120.f);

  // Music slider
  mMusicSlider.x = Win_W / 2.f - 200.f;
  mMusicSlider.y = 260.f;
  mMusicSlider.w = 400.f;
  mMusicSlider.value = 50.f;

  mMusicSlider.label.setFont(mFont);
  mMusicSlider.label.setString("Music Volume");
  mMusicSlider.label.setCharacterSize(26);
  mMusicSlider.label.setFillColor(sf::Color::White);
  mMusicSlider.label.setPosition(Win_W / 2.f - 200.f, 220.f);

  mMusicSlider.track.setSize(sf::Vector2f(400.f, 8.f));
  mMusicSlider.track.setFillColor(sf::Color(80, 80, 80));
  mMusicSlider.track.setPosition(mMusicSlider.x, mMusicSlider.y);

  mMusicSlider.thumb.setSize(sf::Vector2f(20.f, 30.f));
  mMusicSlider.thumb.setFillColor(sf::Color(255, 215, 0));
  mMusicSlider.thumb.setOrigin(10.f, 15.f);

  mMusicSlider.valueText.setFont(mFont);
  mMusicSlider.valueText.setCharacterSize(22);
  mMusicSlider.valueText.setFillColor(sf::Color(200, 200, 200));

  // SFX slider — tương tự, y thấp hơn
  mSFXSlider.x = Win_W / 2.f - 200.f;
  mSFXSlider.y = 380.f;
  mSFXSlider.w = 400.f;
  mSFXSlider.value = 50.f;

  mSFXSlider.label.setFont(mFont);
  mSFXSlider.label.setString("SFX Volume");
  mSFXSlider.label.setCharacterSize(26);
  mSFXSlider.label.setFillColor(sf::Color::White);
  mSFXSlider.label.setPosition(Win_W / 2.f - 200.f, 340.f);

  mSFXSlider.track.setSize(sf::Vector2f(400.f, 8.f));
  mSFXSlider.track.setFillColor(sf::Color(80, 80, 80));
  mSFXSlider.track.setPosition(mSFXSlider.x, mSFXSlider.y);

  mSFXSlider.thumb.setSize(sf::Vector2f(20.f, 30.f));
  mSFXSlider.thumb.setFillColor(sf::Color(255, 215, 0));
  mSFXSlider.thumb.setOrigin(10.f, 15.f);

  mSFXSlider.valueText.setFont(mFont);
  mSFXSlider.valueText.setCharacterSize(22);
  mSFXSlider.valueText.setFillColor(sf::Color(200, 200, 200));

  // Mute toggle
  mMuteText.setFont(mFont);
  mMuteText.setCharacterSize(26);
  mMuteText.setFillColor(sf::Color(150, 255, 150));
  mMuteText.setPosition(Win_W / 2.f - 200.f, 450.f);

  // Back
  mBackSettingsText.setFont(mFont);
  mBackSettingsText.setString("BACK");
  mBackSettingsText.setCharacterSize(28);
  mBackSettingsText.setFillColor(sf::Color(220, 220, 220));
  mBackSettingsText.setOutlineColor(sf::Color::Black);
  mBackSettingsText.setOutlineThickness(2.f);
  sf::FloatRect bb = mBackSettingsText.getLocalBounds();
  mBackSettingsText.setOrigin(bb.left + bb.width / 2.f,
                              bb.top + bb.height / 2.f);
  mBackSettingsText.setPosition(Win_W / 2.f, 540.f);

  // Cập nhật thumb lần đầu
  updateSlider(mMusicSlider, {}, false);
  updateSlider(mSFXSlider, {}, false);
}

void Menu::updateSlider(Slider &s, sf::Vector2f mouse, bool mouseDown) {
  // Tính vị trí thumb theo value
  float thumbX = s.x + (s.value / 100.f) * s.w;
  s.thumb.setPosition(thumbX, s.y + 4.f);

  // Cập nhật text value
  s.valueText.setString(std::to_string((int)s.value));
  s.valueText.setPosition(s.x + s.w + 20.f, s.y - 10.f);

  // Kéo thumb
  if (mouseDown && s.dragging) {
    float newVal = (mouse.x - s.x) / s.w * 100.f;
    s.value = std::max(0.f, std::min(100.f, newVal));
  }
}

void Menu::drawSlider(sf::RenderWindow &w, Slider &s) {
  w.draw(s.label);
  w.draw(s.track);
  w.draw(s.thumb);
  w.draw(s.valueText);
}

void Menu::drawSettingsMenu(sf::RenderWindow &window) {
  window.draw(mBgSprite);
  window.draw(mPanelSprite);
  window.draw(mSettingsTitle);

  drawSlider(window, mMusicSlider);
  drawSlider(window, mSFXSlider);

  mMuteText.setString(mMuteAll ? "[M]  Mute: ON" : "[M]  Mute: OFF");
  window.draw(mMuteText);
  window.draw(mBackSettingsText);
}

void Menu::handleSettingsEvent(const sf::Event &event, sf::RenderWindow &window,
                               MenuResult &result) {
  sf::Vector2f mouse;
  bool mouseDown = false;

  if (event.type == sf::Event::MouseButtonPressed &&
      event.mouseButton.button == sf::Mouse::Left) {
    mouse = window.mapPixelToCoords(
        sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
    mouseDown = true;

    // Check bắt đầu kéo slider
    if (mMusicSlider.thumb.getGlobalBounds().contains(mouse))
      mMusicSlider.dragging = true;
    if (mSFXSlider.thumb.getGlobalBounds().contains(mouse))
      mSFXSlider.dragging = true;

    // Click Mute
    if (mMuteText.getGlobalBounds().contains(mouse)) {
      mMuteAll = !mMuteAll;
      mMusic.setVolume(mMuteAll ? 0.f : mMusicSlider.value / 2.f);
    }

    // Click Back
    if (mBackSettingsText.getGlobalBounds().contains(mouse))
      mScreen = MenuScreen::MAIN;
  }

  if (event.type == sf::Event::MouseButtonReleased) {
    mMusicSlider.dragging = false;
    mSFXSlider.dragging = false;
  }

  if (event.type == sf::Event::MouseMoved) {
    mouse = window.mapPixelToCoords(
        sf::Vector2i(event.mouseMove.x, event.mouseMove.y));

    if (mMusicSlider.dragging) {
      float newVal = (mouse.x - mMusicSlider.x) / mMusicSlider.w * 100.f;
      mMusicSlider.value = std::max(0.f, std::min(100.f, newVal));
      if (!mMuteAll)
        mMusic.setVolume(mMusicSlider.value);
    }
    if (mSFXSlider.dragging) {
      float newVal = (mouse.x - mSFXSlider.x) / mSFXSlider.w * 100.f;
      mSFXSlider.value = std::max(0.f, std::min(100.f, newVal));
    }
  }

  if (event.type == sf::Event::KeyPressed) {
    // Phím tắt tăng/giảm volume
    if (event.key.code == sf::Keyboard::Left) {
      mMusicSlider.value = std::max(0.f, mMusicSlider.value - 5.f);
      if (!mMuteAll)
        mMusic.setVolume(mMusicSlider.value);
    }
    if (event.key.code == sf::Keyboard::Right) {
      mMusicSlider.value = std::min(100.f, mMusicSlider.value + 5.f);
      if (!mMuteAll)
        mMusic.setVolume(mMusicSlider.value);
    }
    if (event.key.code == sf::Keyboard::M) {
      mMuteAll = !mMuteAll;
      mMusic.setVolume(mMuteAll ? 0.f : mMusicSlider.value / 2.f);
    }
    if (event.key.code == sf::Keyboard::Escape)
      mScreen = MenuScreen::MAIN;
  }

  updateSlider(mMusicSlider, mouse, mouseDown);
  updateSlider(mSFXSlider, mouse, mouseDown);
}
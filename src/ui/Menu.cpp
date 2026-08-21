#include "Menu.h"
#include "ShopData.h"
#include "TextureManager.h"
#include "Utils.h"
#include <cmath>
#include <cstdio>
#include <algorithm>


Menu::Menu() {
  // Font
  if (!mFont.loadFromFile(Font_Path))
    printf("FAILED font\n");

  // Background
  const auto &bgTex =
      TextureManager::getInstance().getTexture("assets/ui/menu/menu_bg.png");
  mBgSprite.setTexture(bgTex);
  mBgSprite.setScale((float)Win_W / bgTex.getSize().x,
                     (float)Win_H / bgTex.getSize().y);

  // Title
  const auto &titleTex =
      TextureManager::getInstance().getTexture("assets/ui/menu/title.png");
  mTitleSprite.setTexture(titleTex);
  mTitleSprite.setTextureRect(sf::IntRect(0, 0, 350, 40));
  mTitleSprite.setScale(3.f, 3.f);
  mTitleSprite.setOrigin(350 / 2.f, 40 / 2.f);
  mTitleSprite.setPosition(Win_W / 2.f, 120.f);
  mTitleAnim =
      std::make_unique<Animation>(mTitleSprite, titleTex, 350, 40, 4, 4,
                                  0.12f, // frame
                                  true   // Loop
      );

  // Main Buttons (Clean 5-button layout)
  float btnY = 225.f;
  float gap = 62.f;
  setupButton(mBtnNew, "assets/ui/menu/btn_newgame.png", "CAMPAIGN",
              Win_W / 2.f, btnY);
  setupButton(mBtnChallenges, "assets/ui/menu/btn_newgame.png", "CHALLENGES",
              Win_W / 2.f, btnY + gap);
  setupButton(mBtnLoad, "assets/ui/menu/btn_loadgame.png", "LOAD GAME",
              Win_W / 2.f, btnY + gap * 2);
  setupButton(mBtnSetting, "assets/ui/menu/btn_setting.png", "SETTINGS",
              Win_W / 2.f, btnY + gap * 3);
  setupButton(mBtnQuit, "assets/ui/menu/btn_quit.png", "QUIT", Win_W / 2.f,
              btnY + gap * 4);

  // Icon ? button (Info/Help) ở góc trên bên phải — dùng texture gỗ chuẩn
  setupButton(mBtnInfo, "assets/ui/menu/btn_info.png", "?", Win_W - 65.f, 65.f,
              "", 36);

  // Icon Shop button ở góc trên bên phải — nằm kế bên nút ? (Info)
  setupButton(mBtnShop, "assets/shop/btn_shop.png", "", Win_W - 145.f, 65.f,
              "assets/shop/btn_shop_hover.png", 26);

  setupLoadMenu();
  setupSettingsMenu();
  setupNewGameNamePopup();
  setupInfoMenu();
  setupShopMenu();
  setupChallengeMenu();
  setupDifficultyMenu();

  // Nhạc nền
  if (!mMusic.openFromFile("assets/sounds/menu/menu_music.ogg"))
    printf("FAILED music\n");
  mMusic.setLoop(true);
  mMusic.setVolume(50.f);
  mMusic.play();
}

Menu::~Menu() = default;

void Menu::setupButton(MenuButton &btn, const std::string &texPath,
                       const std::string &label, float x, float y,
                       const std::string &hoverTexPath, unsigned int charSize) {
  btn.setup(texPath, label, mFont, x, y, hoverTexPath, charSize);
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

  setupButton(mBtnBack, "assets/ui/menu/btn_back.png", "BACK", Win_W / 2.f,
              520.f, "assets/ui/menu/btn_back_hover.png");

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
  btn.update(mousePos, dt);
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

  if (mBtnBack.sprite.getGlobalBounds().contains(mouse)) {
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
  if (mScreen == MenuScreen::INFO) {
    handleInfoEvent(event, window, result);
    return;
  }
  if (mScreen == MenuScreen::SHOP) {
    handleShopEvent(event, window, result);
    return;
  }
  if (mScreen == MenuScreen::CHALLENGES) {
    handleChallengeEvent(event, window, result);
    return;
  }
  if (mScreen == MenuScreen::BOT_DIFFICULTY) {
    handleDifficultyEvent(event, window, result);
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
    } else if (mBtnChallenges.sprite.getGlobalBounds().contains(mouse)) {
      mScreen = MenuScreen::CHALLENGES;
    } else if (mBtnLoad.sprite.getGlobalBounds().contains(mouse)) {
      refreshSaveSlots();
      mScreen = MenuScreen::LOAD;
    } else if (mBtnQuit.sprite.getGlobalBounds().contains(mouse)) {
      result = MenuResult::QUIT;
    } else if (mBtnSetting.sprite.getGlobalBounds().contains(mouse)) {
      mScreen = MenuScreen::SETTINGS;
    } else if (mBtnInfo.sprite.getGlobalBounds().contains(mouse)) {
      mScreen = MenuScreen::INFO;
    } else if (mBtnShop.sprite.getGlobalBounds().contains(mouse)) {
      mScreen = MenuScreen::SHOP;
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
  if (mBtnBack.sprite.getGlobalBounds().contains(mouse)) {
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
      updateButton(mBtnBack, mouse, dt);
    }
    return;
  }
  if (mScreen == MenuScreen::SETTINGS) {
    updateButton(mBtnBackSettings, mouse, dt);
    return;
  }
  if (mScreen == MenuScreen::INFO) {
    updateButton(mBtnBackInfo, mouse, dt);
    return;
  }
  if (mScreen == MenuScreen::SHOP) {
    updateButton(mBtnBackShop, mouse, dt);
    for (int i = 0; i < 5; i++) {
      updateButton(mItemBuyButtons[i], mouse, dt);
    }
    return;
  }
  if (mScreen == MenuScreen::CHALLENGES) {
    updateButton(mBtnChallenge2P, mouse, dt);
    updateButton(mBtnChallengeBot, mouse, dt);
    updateButton(mBtnChallengeEndless, mouse, dt);
    updateButton(mBtnChallengeBack, mouse, dt);
    return;
  }

  if (mScreen == MenuScreen::BOT_DIFFICULTY) {
    updateButton(mBtnDifficultyEasy, mouse, dt);
    updateButton(mBtnDifficultyNormal, mouse, dt);
    updateButton(mBtnDifficultyHard, mouse, dt);
    updateButton(mBtnDifficultyBack, mouse, dt);
    return;
  }

  if (mTitleAnim)
    mTitleAnim->update(dt);
  updateButton(mBtnNew, mouse, dt);
  updateButton(mBtnChallenges, mouse, dt);
  updateButton(mBtnLoad, mouse, dt);
  updateButton(mBtnSetting, mouse, dt);
  updateButton(mBtnQuit, mouse, dt);
  updateButton(mBtnInfo, mouse, dt);
  updateButton(mBtnShop, mouse, dt);
}

void Menu::drawButton(sf::RenderWindow &w, MenuButton &btn) { btn.draw(w); }

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
  drawButton(window, mBtnBack);
}

void Menu::drawNewGameMenu(sf::RenderWindow &window) {
  window.draw(mBgSprite);

  if (mEnteringNewGameName) {
    drawNewGameNamePopup(window);
    return;
  }
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

  drawButton(window, mBtnBack);
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
  if (mScreen == MenuScreen::INFO) {
    drawInfoMenu(window);
    return;
  }
  if (mScreen == MenuScreen::SHOP) {
    drawShopMenu(window);
    return;
  }
  if (mScreen == MenuScreen::CHALLENGES) {
    drawChallengeMenu(window);
    return;
  }
  if (mScreen == MenuScreen::BOT_DIFFICULTY) {
    drawDifficultyMenu(window);
    return;
  }
  window.draw(mBgSprite);
  if (mTitleAnim)
    window.draw(mTitleSprite);
  else
    window.draw(mTitle);
  drawButton(window, mBtnNew);
  drawButton(window, mBtnChallenges);
  drawButton(window, mBtnLoad);
  drawButton(window, mBtnSetting);
  drawButton(window, mBtnQuit);
  drawButton(window, mBtnInfo);
  drawButton(window, mBtnShop);
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
  setupButton(mBtnBackSettings, "assets/ui/menu/btn_back.png", "BACK",
              Win_W / 2.f, 540.f, "assets/ui/menu/btn_back_hover.png");

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
  drawButton(window, mBtnBackSettings);
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
    if (mBtnBackSettings.sprite.getGlobalBounds().contains(mouse))
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

void Menu::setupInfoMenu() {
  mInfoTab = 0;

  // Title
  mInfoTitle.setFont(mFont);
  mInfoTitle.setString("GAME GUIDE & INFORMATION");
  mInfoTitle.setCharacterSize(28);
  mInfoTitle.setFillColor(sf::Color(255, 215, 0));
  mInfoTitle.setOutlineColor(sf::Color::Black);
  mInfoTitle.setOutlineThickness(3.f);
  sf::FloatRect tb = mInfoTitle.getLocalBounds();
  mInfoTitle.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
  mInfoTitle.setPosition(Win_W / 2.f, 212.f);

  // Tab Buttons (Headers)
  std::string tabNames[3] = {"[ 1. CONTROLS & SKILLS ]", "[ 2. GAMEPLAY & ERAS ]",
                             "[ 3. SHOP & POWER-UPS ]"};

  float tabPosX[3] = {Win_W / 2.f - 215.f, Win_W / 2.f, Win_W / 2.f + 215.f};

  for (int i = 0; i < 3; i++) {
    mInfoTabButtons[i].setFont(mFont);
    mInfoTabButtons[i].setString(tabNames[i]);
    mInfoTabButtons[i].setCharacterSize(15);
    mInfoTabButtons[i].setOutlineColor(sf::Color::Black);
    mInfoTabButtons[i].setOutlineThickness(2.f);

    sf::FloatRect bounds = mInfoTabButtons[i].getLocalBounds();
    mInfoTabButtons[i].setOrigin(bounds.left + bounds.width / 2.f,
                                 bounds.top + bounds.height / 2.f);
    mInfoTabButtons[i].setPosition(tabPosX[i], 245.f);
  }

  // Card Titles for each tab
  std::string leftTitles[3] = {"MOVEMENT & SHORTCUTS", "RULES & OBJECTIVES",
                               "ITEM SHOP UPGRADES"};
  std::string rightTitles[3] = {"ACTIVE COMBAT SKILLS", "5 HISTORICAL ERAS",
                                "IN-LANE POWER-UPS"};

  for (int i = 0; i < 3; i++) {
    mInfoCardTitleLeft[i].setFont(mFont);
    mInfoCardTitleLeft[i].setString(leftTitles[i]);
    mInfoCardTitleLeft[i].setCharacterSize(17);
    mInfoCardTitleLeft[i].setFillColor(sf::Color(100, 220, 255));
    mInfoCardTitleLeft[i].setOutlineColor(sf::Color::Black);
    mInfoCardTitleLeft[i].setOutlineThickness(2.f);
    sf::FloatRect lb = mInfoCardTitleLeft[i].getLocalBounds();
    mInfoCardTitleLeft[i].setOrigin(lb.left + lb.width / 2.f,
                                    lb.top + lb.height / 2.f);
    mInfoCardTitleLeft[i].setPosition(Win_W / 2.f - 170.f, 282.f);

    mInfoCardTitleRight[i].setFont(mFont);
    mInfoCardTitleRight[i].setString(rightTitles[i]);
    mInfoCardTitleRight[i].setCharacterSize(17);
    mInfoCardTitleRight[i].setFillColor(sf::Color(100, 220, 255));
    mInfoCardTitleRight[i].setOutlineColor(sf::Color::Black);
    mInfoCardTitleRight[i].setOutlineThickness(2.f);
    sf::FloatRect rb = mInfoCardTitleRight[i].getLocalBounds();
    mInfoCardTitleRight[i].setOrigin(rb.left + rb.width / 2.f,
                                     rb.top + rb.height / 2.f);
    mInfoCardTitleRight[i].setPosition(Win_W / 2.f + 170.f, 282.f);
  }

  // Helper lambda for setup left-aligned rows
  auto setupRows = [this](sf::Text texts[], const std::string lines[],
                          int count, float startX, float startY,
                          float lineGap = 25.5f, unsigned int charSize = 13) {
    for (int i = 0; i < count; i++) {
      texts[i].setFont(mFont);
      texts[i].setString(lines[i]);
      texts[i].setCharacterSize(charSize);
      texts[i].setFillColor(sf::Color(240, 240, 240));
      texts[i].setOutlineColor(sf::Color::Black);
      texts[i].setOutlineThickness(1.0f);
      texts[i].setOrigin(0.f, 0.f);
      texts[i].setPosition(startX, startY + i * lineGap);
    }
  };

  // Tab 0: Controls & Combat Skills
  std::string tab0LeftLines[] = {
      "P1 Controls: W/A/S/D | LShift: Sprint",
      "P1 Skills: E (Speed), Q (Radar), T (Freeze)",
      "P2 Controls: Arrow Keys | RShift: Sprint",
      "P2 Skills: [ / O (Speed), ] / K (Radar), \\ / L (Freeze)",
      "P: Pause Game | M: Menu | R: Restart",
      "F11 / Alt+Enter: Fullscreen | V: Hitbox"};
  std::string tab0RightLines[] = {
      "Skill 1: Speed Surge (+50% Spd, 5.0s)",
      "Skill 2: Coin Magnet (Pulls Coins, 6.0s)",
      "Skill 3: Time Freeze (Freeze All, 5.0s)",
      "Stamina: Drains on sprint, auto-regens",
      "Energy: Drains on move (Score scaling)",
      "2P Mode: Contest Power-Ups across 5 Eras!"};
  setupRows(mInfoTab0Left, tab0LeftLines, 6, Win_W / 2.f - 318.f, 306.f, 25.5f, 13);
  setupRows(mInfoTab0Right, tab0RightLines, 6, Win_W / 2.f + 22.f, 306.f, 25.5f, 13);

  // Tab 1: Gameplay & Eras
  std::string tab1LeftLines[] = {
      "Goal : Reach Time Elevator at top lane",
      "Time Limit : 45s (+8s / Shop Timer)",
      "Traffic Light : Red stops lane traffic",
      "Health : 3 Hearts (Game Over at 0 HP)",
      "Score : Advance + Coins + Wave Bonus",
      "Endless Chrono : Infinite era scaling"};
  std::string tab1RightLines[] = {
      "1. Prehistoric : Dinos & Stampede",
      "2. Ancient Egypt : Camels & Sandstorm",
      "3. Medieval : Horses & Arrow Rain",
      "4. Modern City : Cars & Rush Hour",
      "5. Cyberpunk : UFOs, Lasers & Void",
      "Each era features unique death scenes"};
  setupRows(mInfoTab1Left, tab1LeftLines, 6, Win_W / 2.f - 318.f, 306.f, 25.5f, 13);
  setupRows(mInfoTab1Right, tab1RightLines, 6, Win_W / 2.f + 22.f, 306.f, 25.5f, 13);

  // Tab 2: Shop & Power-Ups
  std::string tab2LeftLines[] = {
      "Energy Shield (250c) : Block 1 fatal hit",
      "Speed Boots (400c) : +15% Spd & 'E' Skill",
      "Time Extender (350c) : +8s & 'T' Skill",
      "Coin Radar (500c) : Pull Coins & 'Q' Skill",
      "Extra Heart (1500c) : +1 Life / Max HP",
      "3 Save Slots : Independent Coins & Gear"};
  std::string tab2RightLines[] = {
      "Magnet (Blue) : Pull all coins (8s)",
      "Time Stop (Clock) : Freeze traffic (4s)",
      "Speed Boost : +50% Speed Surge (6s)",
      "Shield : +1 Shield (Shop Synced)",
      "2X Coin : 2x Score & Gold Multiplier",
      "Coins : Collect in lanes to buy items"};
  setupRows(mInfoTab2Left, tab2LeftLines, 6, Win_W / 2.f - 318.f, 306.f, 25.5f, 13);
  setupRows(mInfoTab2Right, tab2RightLines, 6, Win_W / 2.f + 22.f, 306.f, 25.5f, 13);

  // Page Indicator Hint
  mInfoPageHint.setFont(mFont);
  mInfoPageHint.setCharacterSize(14);
  mInfoPageHint.setFillColor(sf::Color(255, 235, 150));
  mInfoPageHint.setOutlineColor(sf::Color::Black);
  mInfoPageHint.setOutlineThickness(1.f);
  mInfoPageHint.setPosition(Win_W / 2.f, 503.f);

  // Back Button
  setupButton(mBtnBackInfo, "assets/ui/menu/btn_back.png", "BACK", Win_W / 2.f,
              538.f, "assets/ui/menu/btn_back_hover.png");
  mBtnBackInfo.baseScale = 0.85f;
  mBtnBackInfo.hoverScale = 0.95f;
  mBtnBackInfo.curScale = 0.85f;
  mBtnBackInfo.sprite.setScale(0.85f, 0.85f);
}

void Menu::drawInfoMenu(sf::RenderWindow &window) {
  window.draw(mBgSprite);
  window.draw(mPanelSprite);
  window.draw(mInfoTitle);

  // Draw 3 Tab Buttons with highlight box on active tab
  for (int i = 0; i < 3; i++) {
    if (i == mInfoTab) {
      mInfoTabButtons[i].setFillColor(sf::Color(255, 220, 50));

      // Active tab highlight indicator frame
      sf::FloatRect bounds = mInfoTabButtons[i].getGlobalBounds();
      sf::RectangleShape tabBox(
          sf::Vector2f(bounds.width + 16.f, bounds.height + 8.f));
      tabBox.setOrigin((bounds.width + 16.f) / 2.f,
                       (bounds.height + 8.f) / 2.f);
      tabBox.setPosition(mInfoTabButtons[i].getPosition().x,
                         mInfoTabButtons[i].getPosition().y + 2.f);
      tabBox.setFillColor(sf::Color(255, 215, 0, 50));
      tabBox.setOutlineColor(sf::Color(255, 215, 0, 200));
      tabBox.setOutlineThickness(1.2f);
      window.draw(tabBox);
    } else {
      mInfoTabButtons[i].setFillColor(sf::Color(170, 170, 170));
    }
    window.draw(mInfoTabButtons[i]);
  }

  // Card Left container
  sf::RectangleShape cardLeft(sf::Vector2f(320.f, 230.f));
  cardLeft.setFillColor(sf::Color(15, 12, 10, 210));
  cardLeft.setOutlineColor(sf::Color(218, 165, 32, 220));
  cardLeft.setOutlineThickness(1.5f);
  cardLeft.setPosition(Win_W / 2.f - 330.f, 266.f);
  window.draw(cardLeft);

  // Card Right container
  sf::RectangleShape cardRight(sf::Vector2f(320.f, 230.f));
  cardRight.setFillColor(sf::Color(15, 12, 10, 210));
  cardRight.setOutlineColor(sf::Color(218, 165, 32, 220));
  cardRight.setOutlineThickness(1.5f);
  cardRight.setPosition(Win_W / 2.f + 10.f, 266.f);
  window.draw(cardRight);

  // Draw Card Titles for active tab
  window.draw(mInfoCardTitleLeft[mInfoTab]);
  window.draw(mInfoCardTitleRight[mInfoTab]);

  // Draw Tab specific content
  if (mInfoTab == 0) {
    for (int i = 0; i < 6; i++) {
      window.draw(mInfoTab0Left[i]);
      window.draw(mInfoTab0Right[i]);
    }
  } else if (mInfoTab == 1) {
    for (int i = 0; i < 6; i++) {
      window.draw(mInfoTab1Left[i]);
      window.draw(mInfoTab1Right[i]);
    }
  } else if (mInfoTab == 2) {
    for (int i = 0; i < 6; i++) {
      window.draw(mInfoTab2Left[i]);
      window.draw(mInfoTab2Right[i]);
    }
  }

  // Draw Page Hint
  std::string hintStr =
      "< Tab " + std::to_string(mInfoTab + 1) +
      " / 3 - Click tab or press Left/Right Arrow to switch >";
  mInfoPageHint.setString(hintStr);
  sf::FloatRect hb = mInfoPageHint.getLocalBounds();
  mInfoPageHint.setOrigin(hb.left + hb.width / 2.f, hb.top + hb.height / 2.f);
  mInfoPageHint.setPosition(Win_W / 2.f, 504.f);
  window.draw(mInfoPageHint);

  // Draw Back Button
  drawButton(window, mBtnBackInfo);
}

void Menu::handleInfoEvent(const sf::Event &event, sf::RenderWindow &window,
                           MenuResult &result) {
  if (event.type == sf::Event::MouseButtonPressed &&
      event.mouseButton.button == sf::Mouse::Left) {
    sf::Vector2f mouse = window.mapPixelToCoords(
        sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

    // Check tab click
    for (int i = 0; i < 3; i++) {
      sf::FloatRect tabBounds = mInfoTabButtons[i].getGlobalBounds();
      tabBounds.left -= 10.f;
      tabBounds.width += 20.f;
      tabBounds.top -= 5.f;
      tabBounds.height += 10.f;
      if (tabBounds.contains(mouse)) {
        mInfoTab = i;
        return;
      }
    }

    if (mBtnBackInfo.sprite.getGlobalBounds().contains(mouse)) {
      mScreen = MenuScreen::MAIN;
    }
  }

  if (event.type == sf::Event::KeyPressed) {
    if (event.key.code == sf::Keyboard::Escape) {
      mScreen = MenuScreen::MAIN;
    } else if (event.key.code == sf::Keyboard::Left ||
               event.key.code == sf::Keyboard::A) {
      mInfoTab = (mInfoTab + 2) % 3;
    } else if (event.key.code == sf::Keyboard::Right ||
               event.key.code == sf::Keyboard::D) {
      mInfoTab = (mInfoTab + 1) % 3;
    }
  }
}

void Menu::setupShopMenu() {
  mShopTitle.setFont(mFont);
  mShopTitle.setString("ITEM SHOP");
  mShopTitle.setCharacterSize(44);
  mShopTitle.setFillColor(sf::Color(255, 215, 0));
  mShopTitle.setOutlineColor(sf::Color::Black);
  mShopTitle.setOutlineThickness(3.f);
  sf::FloatRect tb = mShopTitle.getLocalBounds();
  mShopTitle.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
  mShopTitle.setPosition(Win_W / 2.f, 155.f);

  mShopCoinsText.setFont(mFont);
  mShopCoinsText.setCharacterSize(22);
  mShopCoinsText.setFillColor(sf::Color(255, 230, 100));
  mShopCoinsText.setOutlineColor(sf::Color::Black);
  mShopCoinsText.setOutlineThickness(2.f);

  std::string iconPaths[5] = {
      "assets/shop/item_shield.png", "assets/shop/item_speed.png",
      "assets/shop/item_time.png", "assets/shop/item_radar.png",
      "assets/ui/hud/heart.png"};

  std::string titles[5] = {"Energy Shield", "Speed Boots", "Time Extender",
                           "Coin Magnet Radar", "Extra Heart"};

  std::string descs[5] = {
      "Absorbs 1 deadly hit (Consumable)", "+15% Speed & Unlocks 'E' Skill",
      "+8s Level Time & Unlocks 'T' Skill", "Pulls all coins & Unlocks 'Q' Skill",
      "+1 Extra Life / Restore Health"};

  int prices[5] = {250, 400, 350, 500, 1500};

  for (int i = 0; i < 5; i++) {
    if (mItemTextures[i].loadFromFile(iconPaths[i])) {
      mItemSprites[i].setTexture(mItemTextures[i]);
      sf::Vector2u texSize = mItemTextures[i].getSize();
      if (texSize.x > 0 && texSize.y > 0) {
        float targetSize = 34.f;
        float scale = targetSize / std::max(texSize.x, texSize.y);
        mItemSprites[i].setScale(scale, scale);
      }
    }

    float itemY = 230.f + i * 54.f;

    mItemSprites[i].setPosition(Win_W / 2.f - 240.f, itemY - 5.f);

    mItemTitleTexts[i].setFont(mFont);
    mItemTitleTexts[i].setString(titles[i]);
    mItemTitleTexts[i].setCharacterSize(18);
    mItemTitleTexts[i].setFillColor(sf::Color(255, 215, 0));
    mItemTitleTexts[i].setPosition(Win_W / 2.f - 195.f, itemY - 5.f);

    mItemDescTexts[i].setFont(mFont);
    mItemDescTexts[i].setString(descs[i]);
    mItemDescTexts[i].setCharacterSize(13);
    mItemDescTexts[i].setFillColor(sf::Color(220, 220, 220));
    mItemDescTexts[i].setPosition(Win_W / 2.f - 195.f, itemY + 18.f);

    mItemPriceTexts[i].setFont(mFont);
    mItemPriceTexts[i].setString(std::to_string(prices[i]) + " Coins");
    mItemPriceTexts[i].setCharacterSize(15);
    mItemPriceTexts[i].setFillColor(sf::Color(100, 220, 255));
    mItemPriceTexts[i].setPosition(Win_W / 2.f + 95.f, itemY + 2.f);

    setupButton(mItemBuyButtons[i], "assets/ui/menu/btn_yes.png", "BUY",
                Win_W / 2.f + 205.f, itemY + 10.f,
                "assets/ui/menu/btn_yes_hover.png", 13);
    mItemBuyButtons[i].baseScale = 0.55f;
    mItemBuyButtons[i].hoverScale = 0.65f;
    mItemBuyButtons[i].curScale = 0.55f;
    mItemBuyButtons[i].sprite.setScale(0.55f, 0.55f);
    mItemBuyButtons[i].label.setScale(0.55f, 0.55f);
  }

  setupButton(mBtnBackShop, "assets/ui/menu/btn_back.png", "BACK", Win_W / 2.f,
              530.f, "assets/ui/menu/btn_back_hover.png");
}

void Menu::drawShopMenu(sf::RenderWindow &window, int currentHp, int maxHp) {
  window.draw(mBgSprite);
  window.draw(mPanelSprite);
  window.draw(mShopTitle);

  int activeSlot = ShopData::getActiveSlot();

  if (mIsTwoPlayerShop) {
    // 2-Player Tabs: PLAYER 1 (WASD) - Slot 4 | PLAYER 2 (ARROWS) - Slot 5
    float tabStartX = Win_W / 2.f - 140.f;
    float tabGap = 280.f;
    int pSlots[2] = {ShopData::SLOT_P1_2P, ShopData::SLOT_P2_2P};
    std::string pNames[2] = {"PLAYER 1 (WASD)", "PLAYER 2 (ARROWS)"};
    sf::Color pColors[2] = {sf::Color(0, 210, 255), sf::Color(255, 130, 255)};

    for (int i = 0; i < 2; i++) {
      float tx = tabStartX + i * tabGap;
      float ty = 188.f;
      bool isSelected = (pSlots[i] == activeSlot);

      sf::RectangleShape tabBox(sf::Vector2f(230.f, 30.f));
      tabBox.setOrigin(115.f, 15.f);
      tabBox.setPosition(tx, ty);

      if (isSelected) {
        tabBox.setFillColor(sf::Color(pColors[i].r, pColors[i].g, pColors[i].b, 90));
        tabBox.setOutlineColor(pColors[i]);
        tabBox.setOutlineThickness(2.2f);
      } else {
        tabBox.setFillColor(sf::Color(20, 25, 35, 150));
        tabBox.setOutlineColor(sf::Color(80, 90, 100, 160));
        tabBox.setOutlineThickness(1.2f);
      }
      window.draw(tabBox);

      sf::Text tabText;
      tabText.setFont(mFont);
      tabText.setString(pNames[i]);
      tabText.setCharacterSize(15);
      tabText.setFillColor(isSelected ? sf::Color(255, 255, 255) : sf::Color(170, 180, 190));
      tabText.setOutlineColor(sf::Color::Black);
      tabText.setOutlineThickness(1.5f);

      sf::FloatRect tb = tabText.getLocalBounds();
      tabText.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
      tabText.setPosition(tx, ty - 1.f);
      window.draw(tabText);
    }

    int coins = ShopData::getCoins(activeSlot);
    std::string pLabel = (activeSlot == ShopData::SLOT_P1_2P) ? "PLAYER 1 (WASD)" : "PLAYER 2 (ARROWS)";
    mShopCoinsText.setString("COINS: " + std::to_string(coins) + "  GOLD  [" + pLabel + "]");
    mShopCoinsText.setFillColor((activeSlot == ShopData::SLOT_P1_2P) ? sf::Color(100, 230, 255) : sf::Color(255, 170, 255));
  } else {
    // Campaign 3 Slots (SLOT 1 | SLOT 2 | SLOT 3)
    float tabStartX = Win_W / 2.f - 180.f;
    float tabGap = 180.f;

    for (int slot = 1; slot <= 3; slot++) {
      float tx = tabStartX + (slot - 1) * tabGap;
      float ty = 188.f;
      bool isSelected = (slot == activeSlot);

      sf::RectangleShape tabBox(sf::Vector2f(120.f, 28.f));
      tabBox.setOrigin(60.f, 14.f);
      tabBox.setPosition(tx, ty);

      if (isSelected) {
        tabBox.setFillColor(sf::Color(0, 160, 220, 110));
        tabBox.setOutlineColor(sf::Color(0, 230, 255, 240));
        tabBox.setOutlineThickness(2.0f);
      } else {
        tabBox.setFillColor(sf::Color(20, 25, 35, 140));
        tabBox.setOutlineColor(sf::Color(80, 90, 100, 160));
        tabBox.setOutlineThickness(1.2f);
      }
      window.draw(tabBox);

      sf::Text tabText;
      tabText.setFont(mFont);
      tabText.setString("SLOT " + std::to_string(slot));
      tabText.setCharacterSize(16);
      tabText.setFillColor(isSelected ? sf::Color(255, 220, 80) : sf::Color(170, 180, 190));
      tabText.setOutlineColor(sf::Color::Black);
      tabText.setOutlineThickness(1.5f);

      sf::FloatRect tb = tabText.getLocalBounds();
      tabText.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
      tabText.setPosition(tx, ty - 1.f);
      window.draw(tabText);
    }

    int coins = ShopData::getCoins(activeSlot);
    mShopCoinsText.setString("COINS: " + std::to_string(coins) + "  GOLD  (SLOT " + std::to_string(activeSlot) + ")");
    mShopCoinsText.setFillColor(sf::Color(255, 215, 0));
  }

  sf::FloatRect cb = mShopCoinsText.getLocalBounds();
  mShopCoinsText.setOrigin(cb.left + cb.width / 2.f, cb.top + cb.height / 2.f);
  mShopCoinsText.setPosition(Win_W / 2.f, 218.f);
  window.draw(mShopCoinsText);

  std::string itemIds[5] = {"shield", "speed", "time", "radar", "heart"};
  std::string titles[5] = {"Energy Shield", "Speed Boots", "Time Extender",
                           "Coin Magnet Radar", "Extra Heart"};
  int prices[5] = {250, 400, 350, 500, 1500};

  for (int i = 0; i < 5; i++) {
    float itemY = 245.f + i * 52.f;

    sf::Vector2u texSize = mItemTextures[i].getSize();
    float scale = mItemSprites[i].getScale().x;
    float renderW = (texSize.x > 0) ? (texSize.x * scale) : 34.f;
    float renderH = (texSize.y > 0) ? (texSize.y * scale) : 34.f;

    float iconBoxLeft = Win_W / 2.f - 245.f;
    float iconX = iconBoxLeft + (38.f - renderW) / 2.f;
    float iconY = itemY + (38.f - renderH) / 2.f - 2.f;

    mItemSprites[i].setPosition(iconX, iconY);
    mItemTitleTexts[i].setPosition(Win_W / 2.f - 195.f, itemY - 2.f);
    mItemDescTexts[i].setPosition(Win_W / 2.f - 195.f, itemY + 18.f);
    mItemBuyButtons[i].sprite.setPosition(Win_W / 2.f + 205.f, itemY + 10.f);

    bool isHeart = (itemIds[i] == "heart");
    bool isFullHp = (isHeart && currentHp >= maxHp);

    int count = ShopData::getItemCount(itemIds[i], activeSlot);
    std::string titleStr = titles[i];
    if (isHeart) {
      titleStr += " (" + std::to_string(currentHp) + "/" + std::to_string(maxHp) + " HP)";
    } else {
      titleStr += (count > 0 ? (" (x" + std::to_string(count) + ")") : " (x0)");
    }
    mItemTitleTexts[i].setString(titleStr);

    window.draw(mItemSprites[i]);
    window.draw(mItemTitleTexts[i]);
    window.draw(mItemDescTexts[i]);

    mItemPriceTexts[i].setString(std::to_string(prices[i]) + " Gold");
    mItemPriceTexts[i].setFillColor(sf::Color(255, 215, 0));

    int currentCoins = ShopData::getCoins(activeSlot);
    if (isFullHp) {
      mItemBuyButtons[i].label.setString("FULL HP");
      mItemBuyButtons[i].label.setFillColor(sf::Color(140, 255, 140));
    } else if (currentCoins < prices[i]) {
      mItemBuyButtons[i].label.setString("NO GOLD");
      mItemBuyButtons[i].label.setFillColor(sf::Color(255, 100, 100));
    } else {
      mItemBuyButtons[i].label.setString("BUY");
      mItemBuyButtons[i].label.setFillColor(sf::Color::White);
    }

    // Right-align price text so it sits cleanly to the left of the compact BUY button
    sf::FloatRect pb = mItemPriceTexts[i].getLocalBounds();
    mItemPriceTexts[i].setOrigin(pb.left + pb.width, pb.top + pb.height / 2.f);
    mItemPriceTexts[i].setPosition(Win_W / 2.f + 145.f, itemY + 10.f);

    sf::FloatRect b = mItemBuyButtons[i].label.getLocalBounds();
    mItemBuyButtons[i].label.setOrigin(b.left + b.width / 2.f,
                                       b.top + b.height / 2.f);
    mItemBuyButtons[i].label.setPosition(
        mItemBuyButtons[i].sprite.getPosition());

    window.draw(mItemPriceTexts[i]);
    drawButton(window, mItemBuyButtons[i]);
  }

  drawButton(window, mBtnBackShop);
}

void Menu::handleShopEvent(const sf::Event &event, sf::RenderWindow &window,
                           MenuResult &result, int* currentHpPtr, int maxHp) {
  if (event.type == sf::Event::KeyPressed &&
      (event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::S)) {
    mScreen = MenuScreen::MAIN;
    return;
  }

  if (event.type == sf::Event::MouseButtonPressed &&
      event.mouseButton.button == sf::Mouse::Left) {
    static sf::Clock buyCooldownClock;
    if (buyCooldownClock.getElapsedTime().asSeconds() < 0.22f) {
      return;
    }
    buyCooldownClock.restart();

    sf::Vector2f mouse = window.mapPixelToCoords(
        sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

    if (mBtnBackShop.sprite.getGlobalBounds().contains(mouse)) {
      mScreen = MenuScreen::MAIN;
      return;
    }

    int activeSlot = ShopData::getActiveSlot();

    if (mIsTwoPlayerShop) {
      // 2P Tabs
      float tabStartX = Win_W / 2.f - 140.f;
      float tabGap = 280.f;
      int pSlots[2] = {ShopData::SLOT_P1_2P, ShopData::SLOT_P2_2P};
      for (int i = 0; i < 2; i++) {
        float tx = tabStartX + i * tabGap;
        sf::FloatRect tabBounds(tx - 115.f, 173.f, 230.f, 30.f);
        if (tabBounds.contains(mouse)) {
          ShopData::setActiveSlot(pSlots[i]);
          return;
        }
      }
    } else {
      // Campaign 3 Slots
      float tabStartX = Win_W / 2.f - 180.f;
      float tabGap = 180.f;
      for (int slot = 1; slot <= 3; slot++) {
        float tx = tabStartX + (slot - 1) * tabGap;
        sf::FloatRect tabBounds(tx - 60.f, 172.f, 120.f, 30.f);
        if (tabBounds.contains(mouse)) {
          ShopData::setActiveSlot(slot);
          return;
        }
      }
    }

    activeSlot = ShopData::getActiveSlot();
    std::string itemIds[5] = {"shield", "speed", "time", "radar", "heart"};
    int prices[5] = {250, 400, 350, 500, 1500};

    for (int i = 0; i < 5; i++) {
      if (mItemBuyButtons[i].sprite.getGlobalBounds().contains(mouse)) {
        if (itemIds[i] == "heart") {
          int curHp = (currentHpPtr ? *currentHpPtr : 3);
          if (curHp >= maxHp) {
            return;
          }
          if (ShopData::getCoins(activeSlot) >= prices[i]) {
            if (ShopData::spendCoins(prices[i], activeSlot)) {
              if (currentHpPtr) {
                (*currentHpPtr)++;
                if (*currentHpPtr > maxHp) *currentHpPtr = maxHp;
              }
            }
          }
        } else {
          ShopData::buyItem(itemIds[i], prices[i], activeSlot);
        }
        return;
      }
    }
  }

  if (event.type == sf::Event::KeyPressed) {
    if (event.key.code == sf::Keyboard::Escape) {
      mScreen = MenuScreen::MAIN;
    }
  }
}

void Menu::setupChallengeMenu() {
  // Title
  mChallengeTitle.setFont(mFont);
  mChallengeTitle.setString("SPECIAL CHALLENGES");
  mChallengeTitle.setCharacterSize(38);
  mChallengeTitle.setFillColor(sf::Color(255, 215, 0));
  mChallengeTitle.setOutlineColor(sf::Color::Black);
  mChallengeTitle.setOutlineThickness(3.f);
  sf::FloatRect tb = mChallengeTitle.getLocalBounds();
  mChallengeTitle.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
  mChallengeTitle.setPosition(Win_W / 2.f, 215.f);

  // 3 Challenge Buttons (Clean, evenly spaced layout)
  float startY = 275.f;
  float cardGap = 72.f;

  setupButton(mBtnChallenge2P, "assets/ui/menu/btn_newgame.png", "2 PLAYERS VERSUS",
              Win_W / 2.f, startY, "", 23);
  setupButton(mBtnChallengeBot, "assets/ui/menu/btn_newgame.png", "VS BOT AI",
              Win_W / 2.f, startY + cardGap, "", 23);
  setupButton(mBtnChallengeEndless, "assets/ui/menu/btn_newgame.png", "ENDLESS MODE",
              Win_W / 2.f, startY + cardGap * 2, "", 23);
  setupButton(mBtnChallengeBack, "assets/ui/menu/btn_back.png", "BACK",
              Win_W / 2.f, 505.f, "assets/ui/menu/btn_back_hover.png", 22);
}

void Menu::drawChallengeMenu(sf::RenderWindow &window) {
  window.draw(mBgSprite);
  window.draw(mPanelSprite);
  window.draw(mChallengeTitle);

  drawButton(window, mBtnChallenge2P);
  drawButton(window, mBtnChallengeBot);
  drawButton(window, mBtnChallengeEndless);
  drawButton(window, mBtnChallengeBack);
}

void Menu::handleChallengeEvent(const sf::Event &event, sf::RenderWindow &window,
                                MenuResult &result) {
  if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
    mScreen = MenuScreen::MAIN;
    return;
  }

  if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
    sf::Vector2f mouse = window.mapPixelToCoords(
        sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

    if (mBtnChallenge2P.sprite.getGlobalBounds().contains(mouse)) {
      result = MenuResult::CHALLENGE_TWO_PLAYERS;
      mMusic.stop();
    } else if (mBtnChallengeBot.sprite.getGlobalBounds().contains(mouse)) {
      mScreen = MenuScreen::BOT_DIFFICULTY;
    } else if (mBtnChallengeEndless.sprite.getGlobalBounds().contains(mouse)) {
      result = MenuResult::CHALLENGE_ENDLESS;
      mMusic.stop();
    } else if (mBtnChallengeBack.sprite.getGlobalBounds().contains(mouse)) {
      mScreen = MenuScreen::MAIN;
    }
  }
}

void Menu::setupDifficultyMenu() {
  mDiffTitle.setFont(mFont);
  mDiffTitle.setString("SELECT BOT DIFFICULTY");
  mDiffTitle.setCharacterSize(26);
  mDiffTitle.setFillColor(sf::Color(255, 215, 0));
  mDiffTitle.setOutlineColor(sf::Color::Black);
  mDiffTitle.setOutlineThickness(2.f);

  sf::FloatRect tb = mDiffTitle.getLocalBounds();
  mDiffTitle.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
  mDiffTitle.setPosition(Win_W / 2.f, 220.f);

  // 3 Compact Buttons (160x50 px)
  setupButton(mBtnDifficultyEasy, "assets/ui/menu/btn_yes.png", "EASY",
              Win_W / 2.f - 180.f, 280.f, "assets/ui/menu/btn_yes_hover.png", 20);
  setupButton(mBtnDifficultyNormal, "assets/ui/menu/btn_yes.png", "NORMAL",
              Win_W / 2.f - 180.f, 355.f, "assets/ui/menu/btn_yes_hover.png", 20);
  setupButton(mBtnDifficultyHard, "assets/ui/menu/btn_yes.png", "HARD",
              Win_W / 2.f - 180.f, 430.f, "assets/ui/menu/btn_yes_hover.png", 20);
  setupButton(mBtnDifficultyBack, "assets/ui/menu/btn_back.png", "BACK",
              Win_W / 2.f, 510.f, "assets/ui/menu/btn_back_hover.png", 22);

  // Titles & Concise English Descriptions
  const char *titles[3] = {"EASY BOT", "NORMAL BOT", "HARD BOT"};
  const char *badges[3] = {"[BEGINNER]", "[RECOMMENDED]", "[CHALLENGE]"};
  const char *descs[3] = {
      "Slow reactions & occasional missteps",
      "Smart pathfinding, dodges most hazards",
      "Spatio-Temporal A* with instant micro-dodging reflexes"};
  sf::Color colors[3] = {sf::Color(120, 255, 140), sf::Color(255, 220, 80),
                         sf::Color(255, 90, 90)};

  for (int i = 0; i < 3; i++) {
    float y = 280.f + i * 75.f;

    mDiffCardTitles[i].setFont(mFont);
    mDiffCardTitles[i].setString(titles[i]);
    mDiffCardTitles[i].setCharacterSize(18);
    mDiffCardTitles[i].setFillColor(colors[i]);
    mDiffCardTitles[i].setOutlineColor(sf::Color::Black);
    mDiffCardTitles[i].setOutlineThickness(1.5f);
    mDiffCardTitles[i].setPosition(Win_W / 2.f - 75.f, y - 20.f);

    mDiffCardBadges[i].setFont(mFont);
    mDiffCardBadges[i].setString(badges[i]);
    mDiffCardBadges[i].setCharacterSize(14);
    mDiffCardBadges[i].setFillColor(colors[i]);
    mDiffCardBadges[i].setOutlineColor(sf::Color::Black);
    mDiffCardBadges[i].setOutlineThickness(1.0f);
    mDiffCardBadges[i].setPosition(Win_W / 2.f + 140.f, y - 18.f);

    mDiffCardDescs[i].setFont(mFont);
    mDiffCardDescs[i].setString(descs[i]);
    mDiffCardDescs[i].setCharacterSize(14);
    mDiffCardDescs[i].setFillColor(sf::Color(220, 220, 220));
    mDiffCardDescs[i].setOutlineColor(sf::Color::Black);
    mDiffCardDescs[i].setOutlineThickness(1.0f);
    mDiffCardDescs[i].setPosition(Win_W / 2.f - 75.f, y + 4.f);
  }
}

void Menu::drawDifficultyMenu(sf::RenderWindow &window) {
  window.draw(mBgSprite);
  window.draw(mPanelSprite);
  window.draw(mDiffTitle);

  for (int i = 0; i < 3; i++) {
    float y = 280.f + i * 75.f;
    sf::RectangleShape cardBox(sf::Vector2f(560.f, 64.f));
    cardBox.setOrigin(280.f, 32.f);
    cardBox.setPosition(Win_W / 2.f, y);
    cardBox.setFillColor(sf::Color(20, 22, 34, 210));
    cardBox.setOutlineColor(sf::Color(90, 100, 130, 200));
    cardBox.setOutlineThickness(1.5f);
    window.draw(cardBox);

    window.draw(mDiffCardTitles[i]);
    window.draw(mDiffCardBadges[i]);
    window.draw(mDiffCardDescs[i]);
  }

  drawButton(window, mBtnDifficultyEasy);
  drawButton(window, mBtnDifficultyNormal);
  drawButton(window, mBtnDifficultyHard);
  drawButton(window, mBtnDifficultyBack);
}

void Menu::handleDifficultyEvent(const sf::Event &event,
                                 sf::RenderWindow &window,
                                 MenuResult &result) {
  if (event.type == sf::Event::MouseButtonPressed &&
      event.mouseButton.button == sf::Mouse::Left) {
    sf::Vector2f mouse = window.mapPixelToCoords(
        sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

    sf::FloatRect easyRect(Win_W / 2.f - 280.f, 280.f - 32.f, 560.f, 64.f);
    sf::FloatRect normalRect(Win_W / 2.f - 280.f, 355.f - 32.f, 560.f, 64.f);
    sf::FloatRect hardRect(Win_W / 2.f - 280.f, 430.f - 32.f, 560.f, 64.f);

    if (mBtnDifficultyEasy.contains(mouse) || easyRect.contains(mouse)) {
      result = MenuResult::CHALLENGE_VS_BOT_EASY;
      mMusic.stop();
    } else if (mBtnDifficultyNormal.contains(mouse) || normalRect.contains(mouse)) {
      result = MenuResult::CHALLENGE_VS_BOT_NORMAL;
      mMusic.stop();
    } else if (mBtnDifficultyHard.contains(mouse) || hardRect.contains(mouse)) {
      result = MenuResult::CHALLENGE_VS_BOT_HARD;
      mMusic.stop();
    } else if (mBtnDifficultyBack.contains(mouse)) {
      mScreen = MenuScreen::CHALLENGES;
    }
  }

  if (event.type == sf::Event::KeyPressed) {
    if (event.key.code == sf::Keyboard::Escape) {
      mScreen = MenuScreen::CHALLENGES;
    }
  }
}
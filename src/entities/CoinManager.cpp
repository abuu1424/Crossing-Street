#include "CoinManager.h"
#include "ShopData.h"
#include "SoundManager.h"
#include <cmath>
#include <iostream>

CoinManager::CoinManager() {
  if (mCoinTexture.loadFromFile("assets/shop/coin.png")) {
    mTextureLoaded = true;
    mCoinSprite.setTexture(mCoinTexture);
    mCoinSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
    mCoinSprite.setOrigin(16.f, 16.f);

    float targetSize = 28.f;
    float scale = targetSize / 32.f;
    mCoinSprite.setScale(scale, scale);
  } else {
    std::cerr << "Failed to load assets/shop/coin.png\n";
  }

  if (mFont.loadFromFile(Font_Path)) {
    mFontLoaded = true;
  }

  mShadowShape.setRadius(12.f);
  mShadowShape.setOrigin(12.f, 6.f);
  mShadowShape.setScale(1.f, 0.4f);
  mShadowShape.setFillColor(sf::Color(0, 0, 0, 90));
}

void CoinManager::spawnForLevel(int level) {
  mCoins.clear();
  mFloatingTexts.clear();

  // Playable lane Y rows across screen (between top goal and bottom start)
  const float laneYPositions[] = {160.f, 245.f, 330.f, 415.f, 500.f, 585.f};
  const int numLanes = 6;

  // Distribute 2-3 coins horizontally per lane with varied offsets per level
  for (int l = 0; l < numLanes; l++) {
    float y = laneYPositions[l];
    int coinsInLane = (l % 2 == 0) ? 3 : 2;

    for (int c = 0; c < coinsInLane; c++) {
      float spacing = static_cast<float>(Win_W) / (coinsInLane + 1);
      float baseOffset = (level * 47 + l * 29) % 60 - 30.f;
      float x = spacing * (c + 1) + baseOffset;

      // Keep within screen padding
      x = std::max(90.f, std::min(x, static_cast<float>(Win_W) - 90.f));

      Coin coin;
      coin.position = sf::Vector2f(x, y);
      coin.animTimer = static_cast<float>(c * 0.15 + l * 0.1);
      coin.bobbingTimer = static_cast<float>(c * 0.8 + l * 0.5);
      coin.collected = false;
      mCoins.push_back(coin);
    }
  }
}

void CoinManager::update(float dt, const sf::FloatRect &playerHitbox,
                         SoundManager *sound) {
  for (auto &coin : mCoins) {
    if (coin.collected)
      continue;

    // Spinning animation (8 frames from 4x2 grid in 128x64 texture)
    coin.animTimer += dt;
    if (coin.animTimer >= 0.08f) {
      coin.animTimer = 0.f;
      coin.currentFrame = (coin.currentFrame + 1) % 8;
    }

    // Floating bobbing motion
    coin.bobbingTimer += dt * 3.5f;

    // Collision check with player
    float bobY = std::sin(coin.bobbingTimer) * 3.5f;
    sf::FloatRect coinHitbox(coin.position.x - 14.f,
                             coin.position.y + bobY - 14.f, 28.f, 28.f);

    if (playerHitbox.intersects(coinHitbox)) {
      coin.collected = true;

      // Award +25 Gold to player slot balance
      ShopData::addCoins(25);

      // Add floating feedback text "+25 Gold"
      FloatingCoinText ft;
      ft.text = "+25 Gold";
      ft.position = sf::Vector2f(coin.position.x, coin.position.y - 10.f);
      ft.alpha = 255.f;
      ft.lifetime = 0.85f;
      mFloatingTexts.push_back(ft);

      // Play pickup sound chime if available
      if (sound) {
        sound->playCoinSound();
      }
    }
  }

  // Update floating feedback texts
  for (auto it = mFloatingTexts.begin(); it != mFloatingTexts.end();) {
    it->lifetime -= dt;
    it->position.y -= 25.f * dt; // Rise up
    it->alpha = std::max(0.f, (it->lifetime / 0.85f) * 255.f);

    if (it->lifetime <= 0.f) {
      it = mFloatingTexts.erase(it);
    } else {
      ++it;
    }
  }
}

void CoinManager::draw(sf::RenderWindow &window) {
  if (!mTextureLoaded)
    return;

  for (const auto &coin : mCoins) {
    if (coin.collected)
      continue;

    float bobY = std::sin(coin.bobbingTimer) * 3.5f;

    // Draw ground shadow
    mShadowShape.setPosition(coin.position.x, coin.position.y + 12.f);
    window.draw(mShadowShape);

    // Draw coin sprite (32x32 frames in 4x2 grid)
    int col = coin.currentFrame % 4;
    int row = coin.currentFrame / 4;
    mCoinSprite.setTextureRect(sf::IntRect(col * 32, row * 32, 32, 32));
    mCoinSprite.setPosition(coin.position.x, coin.position.y + bobY);
    window.draw(mCoinSprite);
  }

  // Draw floating feedback texts
  if (mFontLoaded) {
    sf::Text text;
    text.setFont(mFont);
    text.setCharacterSize(16);
    text.setStyle(sf::Text::Bold);
    text.setOutlineThickness(1.8f);

    for (const auto &ft : mFloatingTexts) {
      text.setString(ft.text);
      text.setFillColor(
          sf::Color(255, 230, 40, static_cast<sf::Uint8>(ft.alpha)));
      text.setOutlineColor(
          sf::Color(20, 15, 0, static_cast<sf::Uint8>(ft.alpha)));

      sf::FloatRect bounds = text.getLocalBounds();
      text.setOrigin(bounds.left + bounds.width / 2.f,
                     bounds.top + bounds.height / 2.f);
      text.setPosition(ft.position);
      window.draw(text);
    }
  }
}

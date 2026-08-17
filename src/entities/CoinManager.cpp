#include "CoinManager.h"
#include "ShopData.h"
#include "SoundManager.h"
#include <cmath>
#include <iostream>
#include <algorithm>


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
                         SoundManager *sound, bool magnetActive,
                         const sf::Vector2f& playerPos, int scoreMultiplier) {
  for (auto &coin : mCoins) {
    if (coin.collected)
      continue;

    // Magnet Effect: Hút coin về phía Player (Coin Magnet Radar)
    if (magnetActive) {
      float dx = playerPos.x - coin.position.x;
      float dy = playerPos.y - coin.position.y;
      float distSq = dx * dx + dy * dy;
      if (distSq <= 700.f * 700.f && distSq > 4.f) {
        float dist = std::sqrt(distSq);
        float pullSpeed = 560.f * dt;
        coin.position.x += (dx / dist) * pullSpeed;
        coin.position.y += (dy / dist) * pullSpeed;
      }
    }

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

      // Award +25 Gold to player slot balance (multiplied by scoreMultiplier)
      int earnedGold = 25 * scoreMultiplier;
      ShopData::addCoins(earnedGold);

      // Add floating feedback text
      FloatingCoinText ft;
      ft.text = (scoreMultiplier > 1) ? ("+" + std::to_string(earnedGold) + " Gold (2X!)")
                                      : ("+" + std::to_string(earnedGold) + " Gold");
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

  // Update floating text lifespans
  for (auto it = mFloatingTexts.begin(); it != mFloatingTexts.end();) {
    it->lifetime -= dt;
    it->position.y -= dt * 35.f; // Float upward
    it->alpha = (it->lifetime / 0.85f) * 255.f;

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

  // Draw shadows and animated coin sprites
  for (const auto &coin : mCoins) {
    if (coin.collected)
      continue;

    float bobY = std::sin(coin.bobbingTimer) * 3.5f;

    // Shadow on ground
    mShadowShape.setPosition(coin.position.x, coin.position.y + 12.f);
    window.draw(mShadowShape);

    // Frame clipping: 8 frames across a 4x2 sprite sheet (32x32 per frame)
    int frameX = (coin.currentFrame % 4) * 32;
    int frameY = (coin.currentFrame / 4) * 32;
    mCoinSprite.setTextureRect(sf::IntRect(frameX, frameY, 32, 32));
    mCoinSprite.setPosition(coin.position.x, coin.position.y + bobY);

    window.draw(mCoinSprite);
  }

  // Draw floating feedback texts
  if (mFontLoaded) {
    sf::Text text;
    text.setFont(mFont);
    text.setCharacterSize(16);
    text.setStyle(sf::Text::Bold);

    for (const auto &ft : mFloatingTexts) {
      text.setString(ft.text);
      sf::Uint8 a = static_cast<sf::Uint8>(std::max(0.f, std::min(255.f, ft.alpha)));
      text.setFillColor(sf::Color(255, 215, 0, a));
      text.setOutlineColor(sf::Color(0, 0, 0, a));
      text.setOutlineThickness(1.5f);

      sf::FloatRect b = text.getLocalBounds();
      text.setOrigin(b.width / 2.f, b.height / 2.f);
      text.setPosition(ft.position);

      window.draw(text);
    }
  }
}

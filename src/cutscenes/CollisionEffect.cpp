#include "CollisionEffect.h"
#include <cstdio>
#include <cmath>

CollisionEffect::CollisionEffect(const std::string &texturePath,
                                 sf::Vector2f position, int cols, int rows,
                                 float frameTime, float scale)
    : mActive(false), mIsShieldEffect(false) {
  if (mTexture.loadFromFile(texturePath)) {
    mActive = true;
    if (cols <= 0) cols = 4;
    int fw = static_cast<int>(mTexture.getSize().x) / cols;
    if (rows <= 0 && fw > 0) {
      rows = static_cast<int>(mTexture.getSize().y) / fw;
    }
    if (rows <= 0) rows = 1;

    int fh = static_cast<int>(mTexture.getSize().y) / rows;

    mAnim = std::make_unique<Animation>(mSprite, mTexture, fw, fh, cols, rows,
                                        frameTime, false);

    mSprite.setOrigin(static_cast<float>(fw) / 2.f,
                      static_cast<float>(fh) / 2.f);
    mSprite.setScale(scale, scale);
    mSprite.setPosition(position);
  } else {
    printf("FAILED to load collision texture: %s\n", texturePath.c_str());
  }
}

CollisionEffect::CollisionEffect(sf::Vector2f position)
    : mActive(true), mIsShieldEffect(true), mPosition(position), mTimer(0.f), mDuration(0.35f) {}

void CollisionEffect::update(float dt) {
  if (!mActive)
    return;

  if (mIsShieldEffect) {
    mTimer += dt;
    if (mTimer >= mDuration) {
      mActive = false;
    }
    return;
  }

  if (mAnim) {
    mAnim->update(dt);
    if (mAnim->isFinished()) {
      mActive = false;
    }
  }
}

void CollisionEffect::draw(sf::RenderWindow &window) {
  if (!mActive)
    return;

  if (mIsShieldEffect) {
    float progress = mTimer / mDuration;
    float alpha = (1.0f - progress) * 255.f;
    float radius = 22.f + progress * 38.f;

    // Expanding Cyan Shockwave Ring
    sf::CircleShape ring(radius);
    ring.setOrigin(radius, radius);
    ring.setPosition(mPosition);
    ring.setFillColor(sf::Color::Transparent);
    ring.setOutlineColor(sf::Color(0, 220, 255, static_cast<sf::Uint8>(alpha)));
    ring.setOutlineThickness(3.0f * (1.0f - progress));
    window.draw(ring);

    // Core Flash
    sf::CircleShape core(16.f * (1.0f - progress));
    core.setOrigin(core.getRadius(), core.getRadius());
    core.setPosition(mPosition);
    core.setFillColor(sf::Color(200, 245, 255, static_cast<sf::Uint8>(alpha * 0.85f)));
    window.draw(core);

    // 6 Energy Sparks
    for (int i = 0; i < 6; ++i) {
      float angle = i * (2.f * 3.14159f / 6.f) + progress * 2.5f;
      float dist = radius + 4.f;
      float px = mPosition.x + std::cos(angle) * dist;
      float py = mPosition.y + std::sin(angle) * dist;

      sf::CircleShape spark(3.5f * (1.0f - progress));
      spark.setOrigin(spark.getRadius(), spark.getRadius());
      spark.setPosition(px, py);
      spark.setFillColor(sf::Color(100, 230, 255, static_cast<sf::Uint8>(alpha)));
      window.draw(spark);
    }
    return;
  }

  window.draw(mSprite);
}

bool CollisionEffect::isFinished() const {
  return !mActive || (!mIsShieldEffect && mAnim && mAnim->isFinished());
}

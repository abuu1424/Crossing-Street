#include "CollisionEffect.h"
#include <cstdio>

CollisionEffect::CollisionEffect(const std::string &texturePath,
                                 sf::Vector2f position, int cols, int rows,
                                 float frameTime, float scale)
    : mActive(false) {
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

void CollisionEffect::update(float dt) {
  if (!mActive || !mAnim)
    return;
  mAnim->update(dt);
  if (mAnim->isFinished()) {
    mActive = false;
  }
}

void CollisionEffect::draw(sf::RenderWindow &window) {
  if (mActive) {
    window.draw(mSprite);
  }
}

bool CollisionEffect::isFinished() const {
  return !mActive || (mAnim && mAnim->isFinished());
}

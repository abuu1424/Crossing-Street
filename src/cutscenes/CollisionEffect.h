#pragma once
#include "Animation.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

class CollisionEffect {
private:
  sf::Texture mTexture;
  sf::Sprite mSprite;
  std::unique_ptr<Animation> mAnim;
  bool mActive;

  bool mIsShieldEffect = false;
  sf::Vector2f mPosition;
  float mTimer = 0.f;
  float mDuration = 0.35f;

public:
  CollisionEffect(const std::string &texturePath, sf::Vector2f position,
                  int cols = 4, int rows = 0, float frameTime = 0.07f, float scale = 2.5f);

  // Constructor cho Shield Shatter Effect (Nổ khiên năng lượng, KHÔNG hiện death sprite)
  explicit CollisionEffect(sf::Vector2f position);

  void update(float dt);
  void draw(sf::RenderWindow &window);
  bool isFinished() const;
};

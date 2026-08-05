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

public:
  CollisionEffect(const std::string &texturePath, sf::Vector2f position,
                  int cols = 4, int rows = 0, float frameTime = 0.07f, float scale = 2.5f);

  void update(float dt);
  void draw(sf::RenderWindow &window);
  bool isFinished() const;
};

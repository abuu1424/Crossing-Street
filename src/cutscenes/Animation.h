#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Animation {
    sf::Sprite&              mSprite;
    std::vector<sf::IntRect> mFrames;
    float                    mFrameTime;
    float                    mElapsed;
    int                      mCurrentFrame;
    bool                     mLoop;

public:
    Animation(sf::Sprite& sprite,
              const sf::Texture& texture,
              int frameW, int frameH,
              int cols, int rows,
              float frameTime = 0.12f,
              bool loop = true);

    void update(float dt);
    void reset();
    bool isFinished() const;
};
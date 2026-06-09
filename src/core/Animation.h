#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Animation {
    sf::Sprite&              mSprite;
    sf::Texture              mTexture;
    std::vector<sf::IntRect> mFrames;
    float                    mFrameTime;
    float                    mElapsed;
    int                      mCurrentFrame;
    bool                     mLoop;

public:
    // Constructor cũ — tự load texture từ file
    Animation(sf::Sprite& sprite,
              const std::string& texturePath,
              int frameW, int frameH,
              int cols, int rows,
              float frameTime = 0.12f,
              bool loop = true);

    // Constructor mới — nhận texture từ ngoài vào
    Animation(sf::Sprite& sprite,
              sf::Texture& texture,
              int frameW, int frameH,
              int cols, int rows,
              float frameTime = 0.12f,
              bool loop = true);

    void update(float dt);
    void reset();
    bool isFinished() const;
};
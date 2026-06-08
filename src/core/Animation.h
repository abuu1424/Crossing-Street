
#ifndef CROSSINGGAME_ANIMATION_H
#define CROSSINGGAME_ANIMATION_H

#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Animation
{
    sf::Sprite& mSprite;
    sf::Texture mTexture;
    std::vector<sf::IntRect> mFrames;
    float mFrameTime;
    float mElapsed;
    int mCurrentFrame;
    bool mLoop;
public:
    // frameW, frameH: kích thước 1 frame
    // cols, rows: số cột, số hàng
    // frameTime: giây mỗi frame (dung FRAME_TIME tu Utils.h)
    Animation(sf::Sprite& sprite,
    const std::string& texturePath,
    int frameW, int frameH,
    int cols, int rows,
    float frameTime = 0.12f,
    bool loop = true);
    void update(float dt);
    void reset();
    bool isFinished() const;
};
#endif //CROSSINGGAME_ANIMATION_H

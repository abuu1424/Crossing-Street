#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
class Animation {
	sf::Sprite& mSprite;
	sf::Texture mTexture;
	std::vector<sf::IntRect> mFrames;
	float mFrameTime;
	float mElapsed;
	int mCurrentFrame;
	bool mLoop;
public:
	// frameW, frameH: kich thuoc 1 frame (pixel)
	// cols, rows: so cot va hang trong spritesheet
	// frameTime: giay moi frame (dung FRAME_TIME tu Utils.h)
	Animation(sf::Sprite& sprite,
		const std::string& texturePath,
		int frameW, int frameH,
		int cols, int rows,
		float frameTime = 0.12f,
		bool loop = true);
	void update(float dt); // goi moi frame trong game loop
	void reset();
	bool isFinished() const; // dung cho animation khong loop (vi du chet)
};
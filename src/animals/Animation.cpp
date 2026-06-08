#include "Animation.h"
Animation::Animation(sf::Sprite& sprite, const std::string& path,
	int fw, int fh, int cols, int rows,
	float frameTime, bool loop)
	: mSprite(sprite), mFrameTime(frameTime),
	mElapsed(0), mCurrentFrame(0), mLoop(loop)
{
	mTexture.loadFromFile(path);
	mSprite.setTexture(mTexture);
	// Tao danh sach frame theo thu tu trai->phai, tren->duoi
	for (int row = 0; row < rows; row++)
		for (int col = 0; col < cols; col++)
			mFrames.push_back(sf::IntRect(col * fw, row * fh, fw, fh));
	mSprite.setTextureRect(mFrames[0]);
}
void Animation::update(float dt) {
	mElapsed += dt;
	if (mElapsed >= mFrameTime) {
		mElapsed = 0;
		if (!isFinished()) {
			mCurrentFrame++;
			if (mCurrentFrame >= (int)mFrames.size()) {
				if (mLoop) mCurrentFrame = 0;
				else mCurrentFrame = (int)mFrames.size() - 1;
			}
			mSprite.setTextureRect(mFrames[mCurrentFrame]);
		}
	}
}
void Animation::reset() {
	mCurrentFrame = 0;
	mElapsed = 0;
	mSprite.setTextureRect(mFrames[0]);
}
bool Animation::isFinished() const {
	return !mLoop && mCurrentFrame >= (int)mFrames.size() - 1;
}
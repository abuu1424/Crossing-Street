#include "CPLANE.h"
CPLANE::CPLANE(float speed, float direction) : CANIMAL(speed, direction) {}
CPLANE::~CPLANE() {
	delete mAnim;
}


bool CPLANE::loadSprite(const std::string& path, float x, float y) {
	if (!mTexture.loadFromFile(path)) {
		printf("FAILED : %s\n", path.c_str());
		return false;
	}
	delete mAnim;
	mAnim = new Animation(mSprite, mTexture, 64, 64, 2, 2, Frame_Time);
	mSprite.setScale(3.f, 3.f);
	mSprite.setPosition(x, y);
	return true;
}

void CPLANE::update(float dt) {
	if (mAnim) mAnim->update(dt);
}
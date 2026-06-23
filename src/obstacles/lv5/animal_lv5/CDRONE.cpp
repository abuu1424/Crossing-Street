#include "CDRONE.h"
CDRONE::CDRONE(float speed, float direction) : CANIMAL(speed, direction) {}
CDRONE::~CDRONE() {
	delete mAnim;
}


bool CDRONE::loadSprite(const std::string& path, float x, float y) {
	if (!mTexture.loadFromFile(path)) {
		printf("FAILED : %s\n", path.c_str());
		return false;
	}
	delete mAnim;
	mAnim = new Animation(mSprite, mTexture, 32, 32, 2, 2, Frame_Time);
	mSprite.setScale(3.f, 3.f);
	mSprite.setPosition(x, y);
	return true;
}

void CDRONE::update(float dt) {
	if (mAnim) mAnim->update(dt);
}
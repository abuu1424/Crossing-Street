#pragma once
#include "CANIMAL.h"
#include "Animation.h"

class CEAGLE : public CANIMAL {
private:
	Animation* mAnim = nullptr;
public:
	CEAGLE(float speed = 150.f, float direction = -1.f);
	~CEAGLE();
	bool loadSprite(const std::string& path, float x, float y);
	void update(float dt) override;
};

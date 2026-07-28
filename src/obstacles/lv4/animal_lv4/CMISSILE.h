#pragma once
#include "CANIMAL.h"
#include "Animation.h"

class CMISSILE : public CANIMAL {
private:
	Animation* mAnim = nullptr;
public:
	CMISSILE(float speed = 150.f, float direction = -1.f);
	~CMISSILE();
	bool loadSprite(const std::string& path, float x, float y);
	void update(float dt) override;

};

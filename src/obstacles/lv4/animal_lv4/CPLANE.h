#pragma once
#include "CANIMAL.h"
#include "Animation.h"

class CPLANE : public CANIMAL {
private:
	Animation* mAnim = nullptr;
public:
	CPLANE(float speed = 150.f, float direction = -1.f);
	~CPLANE();
	bool loadSprite(const std::string& path, float x, float y);
	void update(float dt) override;

};

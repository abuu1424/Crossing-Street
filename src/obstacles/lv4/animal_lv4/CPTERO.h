#pragma once
#include "CANIMAL.h"
#include "Animation.h"

class CPTERO : public CANIMAL {
private:
	Animation* mAnim = nullptr;
public:
	CPTERO(float speed = 150.f, float direction = -1.f);
	~CPTERO();
	bool loadSprite(const std::string& path, float x, float y);
	void update(float dt) override;

};

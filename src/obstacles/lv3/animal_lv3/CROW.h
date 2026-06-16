#pragma once
#include "CANIMAL.h"
#include "Animation.h"
class CROW : public CANIMAL {
private:
	Animation* mAnim = nullptr;
public:
	CROW(float speed = 150.f, float direction = -1.f);
	~CROW();
	bool loadSprite(const std::string& path, float x, float y);
	void update(float dt) override;
};

#pragma once
#include "CANIMAL.h"
#include "Animation.h"

class CUFO : public CANIMAL {
private:
	Animation* mAnim = nullptr;
public:
	CUFO(float speed = 150.f, float direction = -1.f);
	~CUFO();
	bool loadSprite(const std::string& path, float x, float y);
	void update(float dt) override;

};
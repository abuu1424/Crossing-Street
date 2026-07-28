#pragma once
#include "CANIMAL.h"
#include "Animation.h"

class CFLYCAR : public CANIMAL {
private:
	Animation* mAnim = nullptr;
public:
	CFLYCAR(float speed = 150.f, float direction = -1.f);
	~CUFO();
	bool loadSprite(const std::string& path, float x, float y);
	void update(float dt) override;

};
#pragma once
#include "CANIMAL.h"
#include "Animation.h"

class CDRONE : public CANIMAL {
private:
	Animation* mAnim = nullptr;
public:
	CDRONE(float speed = 150.f, float direction = -1.f);
	~CDRONE();
	bool loadSprite(const std::string& path, float x, float y);
	void update(float dt) override;

};

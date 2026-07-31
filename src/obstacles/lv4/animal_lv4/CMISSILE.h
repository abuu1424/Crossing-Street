#pragma once
#include "CANIMAL.h"
#include "Animation.h"

class CMISSILE : public CANIMAL {
public:
	CMISSILE(float speed = 150.f, float direction = -1.f);
	~CMISSILE() override = default;
	bool loadSprite(const std::string& path, float x, float y) override;
};

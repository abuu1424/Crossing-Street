#pragma once
#include "CANIMAL.h"
#include "Animation.h"

class CEAGLE : public CANIMAL {
public:
	CEAGLE(float speed = 150.f, float direction = -1.f);
	~CEAGLE() override = default;
	bool loadSprite(const std::string& path, float x, float y) override;
};

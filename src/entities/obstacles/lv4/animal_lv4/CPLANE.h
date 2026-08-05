#pragma once
#include "CANIMAL.h"
#include "Animation.h"

class CPLANE : public CANIMAL {
public:
	CPLANE(float speed = 150.f, float direction = -1.f);
	~CPLANE() override = default;
	bool loadSprite(const std::string& path, float x, float y) override;
};

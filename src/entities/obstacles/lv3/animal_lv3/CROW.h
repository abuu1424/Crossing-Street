#pragma once
#include "CANIMAL.h"
#include "Animation.h"
class CROW : public CANIMAL {
public:
	CROW(float speed = 150.f, float direction = -1.f);
	~CROW() override = default;
	bool loadSprite(const std::string& path, float x, float y) override;
};

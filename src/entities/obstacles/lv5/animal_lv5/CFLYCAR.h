#pragma once
#include "CANIMAL.h"
#include "Animation.h"

class CFLYCAR : public CANIMAL {
public:
	CFLYCAR(float speed = 150.f, float direction = -1.f);
	~CFLYCAR() override = default;
	bool loadSprite(const std::string& path, float x, float y) override;
};
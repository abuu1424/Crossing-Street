#pragma once
#include "CANIMAL.h"
#include "Animation.h"

class CUFO : public CANIMAL {
public:
	CUFO(float speed = 150.f, float direction = -1.f);
	~CUFO() override = default;
	bool loadSprite(const std::string& path, float x, float y) override;
};
#pragma once
#include "CANIMAL.h"
#include "Animation.h"

class CDRONE : public CANIMAL {
public:
	CDRONE(float speed = 150.f, float direction = -1.f);
	~CDRONE() override = default;
	bool loadSprite(const std::string& path, float x, float y) override;
};

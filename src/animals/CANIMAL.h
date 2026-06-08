#pragma once
#include <SFML/Graphics.hpp>
#include "Utils.h"
class CANIMAL {
protected:
	sf::Sprite mSprite;
	float mSpeed;
	int mDirection; // 1 = phai, -1 = trai
	bool mIsAlive;
public:
	CANIMAL(float speed = BASE_SPEED, int direction = 1);
	virtual ~CANIMAL() = default;
};
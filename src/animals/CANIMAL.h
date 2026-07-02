#pragma once
#include <SFML/Graphics.hpp>
#include "Animation.h"
#include "Utils.h"

class CANIMAL {
protected:
	sf::Texture mTexture;
	sf::Sprite mSprite;
	float mSpeed;
	float mDirection; // 1 = phai, -1 = trai
public:
	CANIMAL(float speed = 150.f, float direction = 1.f);
	virtual ~CANIMAL() = default;
	virtual void Move(float dt); 
    virtual void Draw(sf::RenderWindow& w);
    virtual void update(float dt);
	virtual bool loadSprite(const std::string& texturePath, float x, float y);

	sf::Vector2f getPosition() const { return mSprite.getPosition(); }

    sf::FloatRect getBounds() const { return mSprite.getGlobalBounds(); }
};
#pragma once
#include "CANIMAL.h"
#include "Animation.h"

class CEAGLE : public CANIMAL {
    Animation mAnim;

public:
    CEAGLE(float direction = 1.f);

    void update(float dt) override;
    void Draw(sf::RenderWindow& w) override;
};

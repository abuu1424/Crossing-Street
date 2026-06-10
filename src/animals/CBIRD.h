#pragma once
#include "CANIMAL.h"
#include "Animation.h"

class CBIRD : public CANIMAL {
    Animation mAnim; 

public:
    CBIRD(float direction = 1.f);

    void update(float dt) override;
    void Draw(sf::RenderWindow& w) override;
};

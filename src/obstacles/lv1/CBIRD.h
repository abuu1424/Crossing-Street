#pragma once
#include "../../animals/CANIMAL.h"
#include "Animation.h"

class CBIRD : public CANIMAL {
private:
    Animation mAnim;
public:
    CBIRD(float direction = 1.f);

    void update(float dt) override;
    void Draw(sf::RenderWindow& w) override;
};

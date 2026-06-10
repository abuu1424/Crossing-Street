#ifndef CROSSINGGAME_CCAR_H
#define CROSSINGGAME_CCAR_H

#include "CVEHICLE.h"
#include "Animation.h"

class CCAR : public CVEHICLE {
    Animation mCarAnim;

public:
    CCAR(float direction = 1.f);
    void update(float dt) override;
    void Draw(sf::RenderWindow& w) override;
};

#endif
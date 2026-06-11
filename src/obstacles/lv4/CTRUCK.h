#ifndef CROSSINGGAME_CTRUCK_H
#define CROSSINGGAME_CTRUCK_H

#include "../../vehicle/CVEHICLE.h"
#include "Animation.h"

class CTRUCK : public CVEHICLE {
private:
    Animation mTruckAnim;

public:
    CTRUCK(float direction = 1.f);

    void update(float dt) override;
    void Draw(sf::RenderWindow& w) override;
};

#endif
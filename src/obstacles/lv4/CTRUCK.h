#pragma once
#include "CVEHICLE.h"

class CTRUCK : public CVEHICLE {
public:
    CTRUCK(float speed = 150.f, float direction = 1.f);
    bool loadSprite(const std::string& path, float x, float y) override;
    void update(float dt) override;
};
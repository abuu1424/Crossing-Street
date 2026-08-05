#pragma once
#include "CVEHICLE.h"

class CSPHINX : public CVEHICLE {
public:
    CSPHINX(float speed = 170.f, float direction = 1.f);
    bool loadSprite(const std::string& path, float x, float y) override;
    void update(float dt) override;
};
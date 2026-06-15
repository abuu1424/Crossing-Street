#pragma once
#include "CVEHICLE.h"

class CDINOSOUR : public CVEHICLE {
public:
    CDINOSOUR(float speed = 150.f, float direction = 1.f);
    bool loadSprite(const std::string& path, float x, float y) override;
    void update(float dt) override;
};
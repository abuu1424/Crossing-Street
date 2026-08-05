#pragma once
#include "CANIMAL.h"
#include "Animation.h"

class CBIRD : public CANIMAL {
public:
    CBIRD(float speed = 150.f, float direction = 1.f);
    ~CBIRD() override = default;
    bool loadSprite(const std::string& path, float x, float y) override;
};

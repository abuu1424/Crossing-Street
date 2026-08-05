#pragma once
#include "CANIMAL.h"
#include "Animation.h"
class CARROW : public CANIMAL {
public:
    CARROW(float speed = 150.f, float direction = -1.f);
    ~CARROW() override = default;
    bool loadSprite(const std::string& path, float x, float y) override;
};

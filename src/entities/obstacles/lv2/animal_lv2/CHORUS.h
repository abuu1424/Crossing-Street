#pragma once
#include "CANIMAL.h"
#include "Animation.h"

class CHORUS : public CANIMAL {
public:
    CHORUS(float speed = 170.f, float direction = -1.f);
    ~CHORUS() override = default;
    bool loadSprite(const std::string& path, float x, float y) override;
};

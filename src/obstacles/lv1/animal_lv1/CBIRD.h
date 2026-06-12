#pragma once
#include "CANIMAL.h"
#include "Animation.h"

class CBIRD : public CANIMAL {
private:
    Animation * mAnim = nullptr;
public:
    CBIRD(float speed = 150.f, float direction = 1.f);
    ~CBIRD();
    bool loadSprite(const std::string& path, float x, float y);
    void update(float dt) override;
};

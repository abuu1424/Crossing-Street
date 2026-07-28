#pragma once
#include "CANIMAL.h"
#include "Animation.h"
class CARROW : public CANIMAL {
private:
    Animation* mAnim = nullptr;
public:
    CARROW(float speed = 150.f, float direction = -1.f);
    ~CARROW();
    bool loadSprite(const std::string& path, float x, float y);
    void update(float dt) override;
};

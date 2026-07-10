#pragma once
#include "CANIMAL.h"
#include "Animation.h"

class CHORUS : public CANIMAL {
private:
    Animation* mAnim = nullptr;
public:
    CHORUS(float speed = 170.f, float direction = -1.f);
    ~CHORUS();
    bool loadSprite(const std::string& path, float x, float y);
    void update(float dt) override;
};

#pragma once
#include "CTRAFFICLIGHT.h"

class CTRAFFIC_LV1 : public CTRAFFICLIGHT {
public:
    CTRAFFIC_LV1(std::vector<CVEHICLE*>& vehicles);
    bool loadSprite(const std::string& path, float x, float y) override;
};
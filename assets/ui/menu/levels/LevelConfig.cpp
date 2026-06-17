#include "LevelConfig.h"

LevelConfig getLevel(int level)
{
    switch(level)
    {
    case 1:
        return getLevel1();

    default:
        return getLevel1();
    }
}

LevelConfig getLevel1() {
    return {
        1,
        "assets/background/lv1_bg/bg_lv1.png",
        "assets/sprites/lv1_sp/traffic/torch.png",
        "assets/sprites/lv1_sp/traffic/torch1.png",
        "assets/sounds/lv1_sd/music_bg1.ogg",
        10, 160,
        // Làn xe
        {
                { 150.f,  1.f, 175.f, "assets/sprites/lv1_sp/dino/dino1.png", 3, 300.f },
                { 200.f, -1.f, 255.f, "assets/sprites/lv1_sp/dino/dino2.png", 4, 250.f },
                { 120.f,  1.f, 335.f, "assets/sprites/lv1_sp/dino/dino3.png", 3, 350.f },
                { 180.f, -1.f, 415.f, "assets/sprites/lv1_sp/dino/dino1.png", 4, 280.f },
                { 140.f,  1.f, 495.f, "assets/sprites/lv1_sp/dino/dino2.png", 3, 320.f },
            },
            // Động vật bay
            {
                { 150.f,  1.f, 90.f, "assets/sprites/lv1_sp/bird/bay.png", 2, 400.f },
                { 200.f, -1.f, 90.f, "assets/sprites/lv1_sp/bird/bay.png", 2, 350.f },
            }
    };
}
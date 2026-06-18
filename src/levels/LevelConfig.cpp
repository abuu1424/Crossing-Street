#include "LevelConfig.h"

LevelConfig getLevel1() {
    return {
        1,
        "assets/background/lv1_bg/bg_lv1.png",
        "assets/sounds/lv1_sd/music_bg1.ogg",
        "assets/sprites/lv1_sp/traffic/torch.png",
        "assets/sprites/lv1_sp/traffic/torch1.png",
        10, 140,
        {
                { ObstacleType::DINOSAUR, 150.f,  1.f, 215.f, "assets/sprites/lv1_sp/dino/dino1.png", 3, 300.f },

                { ObstacleType::DINOSAUR, 200.f, -1.f, 310.f, "assets/sprites/lv1_sp/dino/dino2.png", 4, 250.f },
                { ObstacleType::MAMMOTH,   90.f,  1.f, 310.f, "assets/sprites/lv1_sp/mammoth/MAMUT.png", 2, 450.f },

                { ObstacleType::DINOSAUR, 120.f, -1.f, 480.f, "assets/sprites/lv1_sp/dino/dino3.png", 3, 350.f },
                { ObstacleType::MAMMOTH,   80.f,  1.f, 480.f, "assets/sprites/lv1_sp/mammoth/MAMUT.png", 2, 480.f },
            },
            {
                { AnimalType::BIRD, 150.f,  1.f, 90.f, "assets/sprites/lv1_sp/bird/bay.png", 2, 400.f },
                { AnimalType::BIRD, 200.f, -1.f, 90.f, "assets/sprites/lv1_sp/bird/bay.png", 2, 350.f },
            }
    };
}

LevelConfig getLevel(int level) {
    switch (level) {
    case 1: return getLevel1();
    default: return getLevel1();
    }
}
#include "LevelConfig.h"

LevelConfig getLevel1()
{
    return {
        1,
        "assets/background/lv1_bg/bg_lv1.png",
        "assets/sounds/lv1_sd/music_bg1.ogg",
        "assets/sprites/lv1_sp/player/player.png",
        "assets/ui/hud/tiensu.png",
        "assets/sprites/lv1_sp/traffic/torch.png",
        "assets/sprites/lv1_sp/traffic/torch1.png",
        80,
        550,
        {

            {ObstacleType::DINOSAUR, 150.f, 1.f, 180.f, "assets/sprites/lv1_sp/dino/dino1.png", 3, 420.f},
            {ObstacleType::DINOSAUR, 200.f, -1.f, 315.f, "assets/sprites/lv1_sp/dino/dino2_trai.png", 3, 430.f},
            {ObstacleType::MAMMOTH, 90.f, 1.f, 270.f, "assets/sprites/lv1_sp/mammoth/MAMUT.png", 2, 620.f},
            {ObstacleType::DINOSAUR, 120.f, -1.f, 405.f, "assets/sprites/lv1_sp/dino/dino3_trai.png", 3, 430.f},
            {ObstacleType::MAMMOTH, 80.f, 1.f, 80.f, "assets/sprites/lv1_sp/mammoth/MAMUT.png", 2, 620.f},
        },
        {
            {AnimalType::BIRD, 280.f, 1.f, 50.f, "assets/sprites/lv1_sp/bird/bay.png", 2, 520.f},
            {AnimalType::BIRD, 250.f, -1.f, 50.f, "assets/sprites/lv1_sp/bird/baytrai.png", 2, 520.f},
        }};
}

LevelConfig getLevel2()
{
    return {
        2,
        "assets/background/lv2_bg/bg_lv2.png",
        "assets/sounds/lv2_sd/music_bg2.ogg",
        "assets/sprites/lv2_sp/player/player.png",
        "assets/ui/hud/codai.png",
        "assets/sprites/lv2_sp/traffic/torch.png",
        "assets/sprites/lv2_sp/traffic/torch1.png",
        80,
        550,
        {},
        {{AnimalType::EAGLE, 180.f, 1, 115.f, "assets/sprites/lv2_sp/bird/kenkenaicap.png", 2, 520.f}}};
};

LevelConfig getLevel(int level)
{
    switch (level)
    {
    case 1:
        return getLevel1();
    case 2:
        return getLevel2();
    default:
        return getLevel1();
    }
}
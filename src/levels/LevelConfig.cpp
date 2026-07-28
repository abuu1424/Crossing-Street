#include "LevelConfig.h"

LevelConfig getLevel1() {
  return {1,
          "assets/background/lv1_bg/bg_lv1.png",
          "assets/sounds/lv1_sd/music_bg1.ogg",
          "assets/sprites/lv1_sp/player/player.png",
          "assets/ui/hud/tiensu.png",
          "assets/sprites/lv1_sp/traffic/torch.png",
          "assets/sprites/lv1_sp/traffic/torch1.png",
          80,
          550,
          {

              {ObstacleType::DINOSAUR, 150.f, 1.f, 180.f,
               "assets/sprites/lv1_sp/dino/dino1.png", 3, 420.f},
              {ObstacleType::DINOSAUR, 200.f, -1.f, 315.f,
               "assets/sprites/lv1_sp/dino/dino2_trai.png", 3, 430.f},
              {ObstacleType::MAMMOTH, 90.f, 1.f, 270.f,
               "assets/sprites/lv1_sp/mammoth/MAMUT.png", 2, 620.f},
              {ObstacleType::DINOSAUR, 120.f, -1.f, 405.f,
               "assets/sprites/lv1_sp/dino/dino3_trai.png", 3, 430.f},
              {ObstacleType::MAMMOTH, 80.f, 1.f, 80.f,
               "assets/sprites/lv1_sp/mammoth/MAMUT.png", 2, 620.f},
          },
          {
              {AnimalType::BIRD, 280.f, 1.f, 50.f,
               "assets/sprites/lv1_sp/bird/bay.png", 2, 520.f},
              {AnimalType::BIRD, 250.f, -1.f, 50.f,
               "assets/sprites/lv1_sp/bird/baytrai.png", 2, 520.f},
          }};
}

LevelConfig getLevel2() {
  return {2,
          "assets/background/lv2_bg/bg_lv2.png",
          "assets/sounds/lv2_sd/music_bg2.ogg",
          "assets/sprites/lv2_sp/player/player_codai.png",
          "assets/ui/hud/codai.png",
          "assets/sprites/lv2_sp/traffic/brazier1.png",
          "assets/sprites/lv2_sp/traffic/brazier.png",
          75,
          80,
          {{ObstacleType::SPHINX, 170.f, 1, 300.f,
            "assets/sprites/lv2_sp/animal/sphinx.png", 3, 420.f},
           {ObstacleType::CAMEL, 170.f, -1, 180.f,
            "assets/sprites/lv2_sp/animal/lacda.png", 3, 430.f},
           {ObstacleType::SPHINX, 170.f, 1, 185.f,
            "assets/sprites/lv2_sp/animal/sphinx.png", 2, 620.f},
           {ObstacleType::CAMEL, 170.f, 1, 385,
            "assets/sprites/lv2_sp/animal/lacda_trai.png", 3, 630.f}

          },
          {
              {AnimalType::EAGLE, 180.f, 1, 170.f,
               "assets/sprites/lv2_sp/bird/kenkenaicap.png", 2, 520.f},
              {AnimalType::HORUS, 180.f, 1, 180.f,
               "assets/sprites/lv2_sp/bird/chim2_trai.png", 2, 520.f},
              {AnimalType::HORUS, 180.f, -1, 400.f,
               "assets/sprites/lv2_sp/bird/chim2.png", 2, 520.f},

          }};
};

LevelConfig getLevel3()
{
    return {
        3,
        "assets/background/lv3_bg/bg_lv3.png",
        "assets/sounds/lv2_sd/music_bg2.ogg",
        "assets/sprites/lv3_sp/player/player_lv3.png",
        "assets/ui/hud/trungco.png",
        "assets/sprites/lv2_sp/traffic/brazier1.png",
        "assets/sprites/lv2_sp/traffic/brazier.png",
        75,
        80,
        {
            {ObstacleType::RICKSHAW, 210.f, -1.f, 320.f, "assets/sprites/lv3_sp/vehicle/xe_keo.png", 3, 264.f},
            {ObstacleType::WOOD,     190.f, -1.f, 280.f, "assets/sprites/lv3_sp/vehicle/khucgo.png", 3, 429.f},
            {ObstacleType::HORSE,    240.f, -1.f, 400.f, "assets/sprites/lv3_sp/vehicle/xe_ngua.png", 2, 595.f}
        },
        {
            {AnimalType::ARROW, 270.f,  1.f, 350.f, "assets/sprites/lv3_sp/animal/muiten.png", 3, 180.f},
            {AnimalType::ROW,   230.f,  1.f, 340.f, "assets/sprites/lv3_sp/animal/qua.png",    3, 346.f},
            {AnimalType::ARROW, 280.f,  1.f, 360.f, "assets/sprites/lv3_sp/animal/muiten.png", 3, 511.f}
        }
    };
}
LevelConfig getLevel(int level)
{
    switch (level)
    {
    case 1:
        return getLevel1();
    case 2:
        return getLevel2();
    case 3:
        return getLevel3();
    default:
        return getLevel1();
    }
}
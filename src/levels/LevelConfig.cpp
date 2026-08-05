#include "LevelConfig.h"

LevelConfig getLevel1() {
  return {1,
          "assets/background/lv1_bg/bg_lv1.png",
          "assets/sounds/lv1_sd/music_bg1.ogg",
          "assets/sprites/lv1_sp/player/player.png",
          "assets/ui/hud/tiensu.png",
          "assets/sprites/lv1_sp/traffic/torch.png",
          "assets/sprites/lv1_sp/traffic/torch1.png",
          "assets/sprites/lv1_sp/collision/collision_1.png",
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
          "assets/sprites/lv2_sp/collision/collision_2.png",
          75,
          80,
          {{ObstacleType::SPHINX, 180.f, 1, 300.f,
            "assets/sprites/lv2_sp/animal/sphinx.png", 3, 420.f},
           {ObstacleType::CAMEL, 180.f, -1, 180.f,
            "assets/sprites/lv2_sp/animal/lacda.png", 3, 430.f},
           {ObstacleType::SPHINX, 180.f, 1, 185.f,
            "assets/sprites/lv2_sp/animal/sphinx.png", 2, 620.f},
           {ObstacleType::CAMEL, 180.f, 1, 385,
            "assets/sprites/lv2_sp/animal/lacda_trai.png", 3, 630.f}},
          {
              {AnimalType::EAGLE, 280.f, 1, 170.f,
               "assets/sprites/lv2_sp/bird/kenkenaicap.png", 2, 520.f},
              {AnimalType::HORUS, 280.f, 1, 180.f,
               "assets/sprites/lv2_sp/bird/chim2_trai.png", 2, 520.f},
              {AnimalType::HORUS, 280.f, -1, 400.f,
               "assets/sprites/lv2_sp/bird/chim2.png", 2, 520.f},
          }};
};

LevelConfig getLevel3() {
  return {3,
          "assets/background/lv3_bg/bg_lv3.png",
          "assets/sounds/lv3_sd/music_bg3.ogg",
          "assets/sprites/lv3_sp/player/player_lv3.png",
          "assets/ui/hud/trungco.png",
          "assets/sprites/lv3_sp/traffic_lv3/MAT_TRANG_TOI.png",
          "assets/sprites/lv3_sp/traffic_lv3/MAT_TRANG_SANG.png",
          "assets/sprites/lv3_sp/collision/collision_3.png",
          185,
          -20,
          {{ObstacleType::RICKSHAW, 210.f, 1.f, 150.f,
            "assets/sprites/lv3_sp/vehicle_lv3/xe_keo.png", 4, 640.f},
           {ObstacleType::WOOD, 190.f, 1.f, 215.f,
            "assets/sprites/lv3_sp/vehicle_lv3/khucgo.png", 3, 529.f},
           {ObstacleType::HORSE, 240.f, 1.f, 505.f,
            "assets/sprites/lv3_sp/vehicle_lv3/xe_ngua.png", 3, 595.f},
           {ObstacleType::RICKSHAW, 240.f, -1.f, 452.f,
            "assets/sprites/lv3_sp/vehicle_lv3/xe_keo_trai.png", 3, 595.f},
           {ObstacleType::HORSE, 240.f, -1.f, 270.f,
            "assets/sprites/lv3_sp/vehicle_lv3/xe_ngua_trai.png", 4, 620.f}},
          {{AnimalType::ARROW, 270.f, -1.f, 450.f,
            "assets/sprites/lv3_sp/animal_lv3/muiten_trai.png", 3, 180.f},
           {AnimalType::ROW, 230.f, 1.f, 380.f,
            "assets/sprites/lv3_sp/animal_lv3/qua.png", 3, 346.f},
           {AnimalType::ROW, 230.f, 1.f, 320.f,
            "assets/sprites/lv3_sp/animal_lv3/qua.png", 3, 346.f},
           {AnimalType::ROW, 230.f, -1.f, 115.f,
            "assets/sprites/lv3_sp/animal_lv3/qua_trai.png", 3, 346.f},
           {AnimalType::ARROW, 280.f, 1.f, 385.f,
            "assets/sprites/lv3_sp/animal_lv3/muiten.png", 3, 511.f}}};
}

LevelConfig getLevel4() {
  return {4,
          "assets/background/lv4_bg/bg_lv4.png",
          "assets/sounds/lv4_sd/music_bg4.ogg",
          "assets/sprites/lv4_sp/player/player_lv4.png",
          "assets/ui/hud/hiendai.png",
          "assets/sprites/lv4_sp/traffic_lv4/traffic_unlight.png",
          "assets/sprites/lv4_sp/traffic_lv4/traffic_light.png",
          "assets/sprites/lv4_sp/collision/collision_4.png",
          75,
          115,
          {{ObstacleType::CAR, 210.f, 1.f, 320.f,
            "assets/sprites/lv4_sp/vehicle_lv4/xe_hoi_phai.png", 3, 264.f},
           {ObstacleType::CAR, 210.f, -1.f, 445.f,
            "assets/sprites/lv4_sp/vehicle_lv4/xe_hoi.png", 3, 264.f},
           {ObstacleType::BUS, 190.f, 1.f, 280.f,
            "assets/sprites/lv4_sp/vehicle_lv4/xe_buyt_phai.png", 3, 429.f},
           {ObstacleType::MOTOR, 240.f, -1.f, 400.f,
            "assets/sprites/lv4_sp/vehicle_lv4/xe_may.png", 2, 595.f},
           {ObstacleType::MOTOR, 240.f, 1.f, 180.f,
            "assets/sprites/lv4_sp/vehicle_lv4/xe_may_phai.png", 3, 595.f}},
          {{AnimalType::PLANE, 270.f, 1.f, 350.f,
            "assets/sprites/lv4_sp/animal_lv4/may_bay_phai.png", 3, 380.f},
           {AnimalType::MISSILE, 230.f, 1.f, 220.f,
            "assets/sprites/lv4_sp/animal_lv4/ten_lua.png", 3, 346.f},
           {AnimalType::MISSILE, 230.f, -1.f, 500.f,
            "assets/sprites/lv4_sp/animal_lv4/ten_lua_trai.png", 3, 346.f},
           {AnimalType::PLANE, 280.f, -1.f, 260.f,
            "assets/sprites/lv4_sp/animal_lv4/may_bay.png", 3, 511.f}}};
}

LevelConfig getLevel5() {
  return {5,

          "assets/background/lv5_bg/bg_lv5.png",
          "assets/sounds/lv5_sd/music_bg5.ogg",
          "assets/sprites/lv5_sp/player/player_lv5.png",
          "assets/ui/hud/tuonglai.png",
          "assets/sprites/lv5_sp/traffic_lv5/STOP.png",
          "assets/sprites/lv5_sp/traffic_lv5/MOVE.png",
          "assets/sprites/lv5_sp/collision/collision_5.png",
          75,
          50,
          {{ObstacleType::ECAR, 310.f, 1.f, 505.f,
            "assets/sprites/lv5_sp/vehicle_lv5/xe_dien.png", 3, 540.f},
           {ObstacleType::ETRAIN, 290.f, 1.f, 140.f,
            "assets/sprites/lv5_sp/vehicle_lv5/tau_dien.png", 2, 629.f},
           {ObstacleType::ETRAIN, 290.f, -1.f, 250.f,
            "assets/sprites/lv5_sp/vehicle_lv5/tau_dien_phai.png", 3, 629.f},
           {ObstacleType::ECAR, 340.f, 1.f, 180.f,
            "assets/sprites/lv5_sp/vehicle_lv5/xe_dien.png", 3, 595.f},
           {ObstacleType::ECAR, 240.f, -1.f, 300.f,
            "assets/sprites/lv5_sp/vehicle_lv5/xe_dien_trai.png", 2, 595.f}},
          {
              {AnimalType::FLYCAR, 300.f, 1.f, 400.f,
               "assets/sprites/lv5_sp/animal_lv5/xe_bay.png", 3, 680.f},
              {AnimalType::FLYCAR, 300.f, -1.f, 250.f,
               "assets/sprites/lv5_sp/animal_lv5/xe_bay_trai.png", 3, 580.f},
              {AnimalType::UFO, 300.f, -1.f, 340.f,
               "assets/sprites/lv5_sp/animal_lv5/ufo_trai.png", 2, 706.f},
              {AnimalType::UFO, 300.f, 1.f, 440.f,
               "assets/sprites/lv5_sp/animal_lv5/ufo.png", 3, 780.f},
              {AnimalType::DRONE, 330.f, 1.f, 360.f,
               "assets/sprites/lv5_sp/animal_lv5/drone.png", 2, 750.f},
              {AnimalType::DRONE, 330.f, -1.f, 500.f,
               "assets/sprites/lv5_sp/animal_lv5/drone_trai.png", 3, 650.f},
          }};
}

LevelConfig getLevel(int level) {
  switch (level) {
  case 1:
    return getLevel1();
  case 2:
    return getLevel2();
  case 3:
    return getLevel3();
  case 4:
    return getLevel4();
  case 5:
    return getLevel5();
  default:
    return getLevel1();
  }
}
#pragma once
#include <SFML/Graphics.hpp>
// Window
inline constexpr int Win_W = 1280;
inline constexpr int Win_H = 720;
inline constexpr int FPS = 60;

// Game
inline constexpr int Max_Level = 5;
inline constexpr int Num_Lanes = 6;
inline constexpr int Lane_H = 80;
inline constexpr int Safe_Zone_H = 80;

// Player
inline constexpr float Player_Speed = 200.f;
inline constexpr float Player_W = 48.f;
inline constexpr float Player_H = 64.f;
// Animal
inline constexpr float ANIMAL_W = 80.f;
inline constexpr float ANIMAL_H = 64.f;
inline constexpr float BASE_SPEED = 150.f;
inline constexpr float FRAME_TIME = 0.12f;
// Animation
inline constexpr float Frame_Time = 0.12f;

// File path
inline constexpr const char *Save_Path = "saves/save.txt";
inline constexpr const char *Score_Path = "saves/highscore.txt";
inline constexpr const char *Font_Path =
    "assets/font/pixel_operator/PixelOperator-Bold.ttf";

// Era
enum class Era {
  Prehistoric = 0,
  Ancient = 1,
  Medieval = 2,
  Modern = 3,
  Future = 4
};

// TRAFFIC LIGHT
inline constexpr float Green_Time = 5.f; // giây đèn xanh
inline constexpr float Red_Time = 3.f;   // giây đèn đỏ

// TIME SET
inline constexpr float Level_Time_Limit = 45.f;

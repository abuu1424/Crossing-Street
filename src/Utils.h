#pragma once

//Window
const int W = static_cast<int>(sf::VideoMode::getDesktopMode().width);
const int H = static_cast<int>(sf::VideoMode::getDesktopMode().height);
const int FPS = 60;

//Game
const int Max_Level = 5;
const int Num_Lanes = 6;
const int Lane_H = 80;
const int Safe_Zone_H = 80;

//Player
const float Player_Speed = 200.f;
const float Player_W = 48.f;
const float Player_H = 64.f;

//Animation
const float Frame_Time = 0.12f;

//File path
const char Save_Path[] = "saves/save.txt";
const char Score_Path[] = "saves/highscore.txt";
const char Front_Path[] = "assets/front/font.ttf";

//Era
enum class Era
{
    Prehistoric = 0,
    Ancient = 1,
    Medieval = 2,
    Modern = 3,
    Future = 4
};

#pragma once
#include <string>
#include <vector>

struct LaneConfig {
    float speed;
    float direction;
    float y;
    std::string spritePath;
    int   count;
    float spacing;
};

struct AnimalConfig {
    float speed;
    float direction;
    float y;
    std::string spritePath;
    int   count;
    float spacing;
};

struct LevelConfig {
    int         level;
    std::string backgroundPath;
    std::string trafficRedPath;
    std::string trafficGreenPath;
    std::string musicPath;
    float       trafficX;
    float       trafficY;
    std::vector<LaneConfig>   lanes;
    std::vector<AnimalConfig> animals;
};

// Khai báo 5 level
LevelConfig getLevel1();
LevelConfig getLevel(int level);
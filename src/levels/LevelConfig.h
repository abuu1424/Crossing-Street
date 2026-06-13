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
    float       trafficX;
    float       trafficY;
    std::vector<LaneConfig>   lanes;
    std::vector<AnimalConfig> animals;
};

// Khai báo 5 level
LevelConfig getLevel1();
LevelConfig getLevel2();
LevelConfig getLevel3();
LevelConfig getLevel4();
LevelConfig getLevel5();
LevelConfig getLevel(int level);
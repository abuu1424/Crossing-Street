#pragma once
#include <string>
#include <vector>

enum class ObstacleType { DINOSAUR, MAMMOTH, SPHINX, CAMEL, HORSE, RICKSHAW, WOOD };
enum class AnimalType   { BIRD, PTERO, EAGLE, HORUS, ROW, ARROW };

struct LaneConfig {
    ObstacleType type;
    float speed;
    float direction;
    float y;
    std::string spritePath;
    int   count;
    float spacing;
};

struct AnimalConfig {
    AnimalType type;
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
    std::string musicPath;
    std::string playerSpritePath;
    std::string hudBarPath;
    std::string trafficRedPath;
    std::string trafficGreenPath;
    float trafficX;
    float trafficY;
    std::vector<LaneConfig>   lanes;
    std::vector<AnimalConfig> animals;
};

LevelConfig getLevel1();
LevelConfig getLevel2();
LevelConfig getLevel3();

LevelConfig getLevel(int level);
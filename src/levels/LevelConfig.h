#pragma once
#include <string>
#include <vector>

enum class ObstacleType { DINOSAUR, MAMMOTH, CHARIOT, HORSE, CAR, TRUCK, FLYCAR, ROBOT };
enum class AnimalType   { BIRD, PTERO, EAGLE, CROW, PIGEON, DRONE };

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
    std::string trafficRedPath;
    std::string trafficGreenPath;
    float trafficX;
    float trafficY;
    std::vector<LaneConfig>   lanes;
    std::vector<AnimalConfig> animals;
};

LevelConfig getLevel1();

LevelConfig getLevel(int level);
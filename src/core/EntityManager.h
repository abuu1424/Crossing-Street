#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "LevelConfig.h"
#include "CVEHICLE.h"
#include "CANIMAL.h"
#include "CTRAFFIC_LV1.h"

class EntityManager {
public:
    ~EntityManager();

    void spawnFromLevel(const LevelConfig& cfg);
    void update(float dt);
    void draw(sf::RenderWindow& window) const;

    void clear();

    const std::vector<CVEHICLE*>& obstacles() const { return mObstacles; }
    const std::vector<CANIMAL*>&  animals()   const { return mAnimals; }
    CTRAFFIC_LV1* traffic() const { return mTraffic; }

private:
    std::vector<CVEHICLE*> mObstacles;
    std::vector<CANIMAL*>  mAnimals;
    CTRAFFIC_LV1*          mTraffic = nullptr;

    static CVEHICLE* createObstacle(ObstacleType type, float speed, float direction);
    static CANIMAL*  createAnimal(AnimalType type, float speed, float direction);
};

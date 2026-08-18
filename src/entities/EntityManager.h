#pragma once
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "LevelConfig.h"
#include "CVEHICLE.h"
#include "CANIMAL.h"
#include "CTRAFFICLIGHT.h"
#include "CTRAFFIC_LV1.h"
#include "CTRAFFIC_LV2.h"

class EntityManager {
public:
    ~EntityManager();

    void spawnFromLevel(const LevelConfig& cfg);
    void update(float dt, float speedMultiplier = 1.0f);
    void draw(sf::RenderWindow& window) const;

    void clear();

    const std::vector<std::unique_ptr<CVEHICLE>>& obstacles() const { return mObstacles; }
    const std::vector<std::unique_ptr<CANIMAL>>&  animals()   const { return mAnimals; }
    CTRAFFICLIGHT* traffic() const { return mTraffic.get(); }

    std::vector<sf::FloatRect> getPredictedHitboxes(float lookaheadTime) const;

private:
    std::vector<std::unique_ptr<CVEHICLE>> mObstacles;
    std::vector<std::unique_ptr<CANIMAL>>  mAnimals;
    std::unique_ptr<CTRAFFICLIGHT>         mTraffic;

    static std::unique_ptr<CVEHICLE> createObstacle(ObstacleType type, float speed, float direction);
    static std::unique_ptr<CANIMAL>  createAnimal(AnimalType type, float speed, float direction);
};
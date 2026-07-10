#include "EntityManager.h"
#include <cstdio>

// Obstacle
#include "CDINOSOUR.h"
#include "CMAMMOTH.h"
#include "CCAMEL.h"
#include "CHORSE.h"
#include "CCAR.h"
#include "CTRUCK.h"
#include "CSPHINX.h"

// Animal
#include "CBIRD.h"
#include "CEAGLE.h"
#include "CHORUS.h"
#include "CPTERO.h"
#include "CROW.h"
#include "CDRONE.h"

#include "Utils.h"

EntityManager::~EntityManager() {
    clear();
}

void EntityManager::clear() {
    for (auto* o : mObstacles) delete o;
    for (auto* a : mAnimals)   delete a;
    delete mTraffic;

    mObstacles.clear();
    mAnimals.clear();
    mTraffic = nullptr;
}

CVEHICLE* EntityManager::createObstacle(ObstacleType type, float speed, float direction) {
    switch (type) {
    case ObstacleType::DINOSAUR: return new CDINOSOUR(speed, direction);
    case ObstacleType::MAMMOTH: return new CMAMMOTH(speed, direction);
    case ObstacleType::CAMEL: return new CCAMEL(speed, direction);
    case ObstacleType::HORSE: return new CHORSE(speed, direction);
    case ObstacleType::CAR: return new CCAR(speed, direction);
    case ObstacleType::TRUCK: return new CTRUCK(speed, direction);
    case ObstacleType::SPHINX: return new CSPHINX(speed, direction);
    case ObstacleType::FLYCAR:
    case ObstacleType::ROBOT:
        printf("[EntityManager] WARNING: chua co class cho ObstacleType %d, dung DINOSAUR tam thoi\n", (int)type);
        return new CDINOSOUR(speed, direction);
    }
    return new CDINOSOUR(speed, direction);
}

CANIMAL* EntityManager::createAnimal(AnimalType type, float speed, float direction) {
    switch (type) {
    case AnimalType::BIRD:  return new CBIRD(speed, direction);
    case AnimalType::EAGLE: return new CEAGLE(speed, direction);
    case AnimalType::HORUS: return new CHORUS(speed, direction);
    case AnimalType::PTERO: return new CPTERO(speed, direction);
    case AnimalType::CROW:  return new CROW(speed, direction);
    case AnimalType::DRONE: return new CDRONE(speed, direction);
    case AnimalType::PIGEON:
        printf("[EntityManager] WARNING: chua co class cho AnimalType::PIGEON, dung BIRD tam thoi\n");
        return new CBIRD(speed, direction);
    }
    return new CBIRD(speed, direction);
}

void EntityManager::spawnFromLevel(const LevelConfig& cfg) {
    clear();

    for (auto& lane : cfg.lanes) {
        for (int i = 0; i < lane.count; i++) {
            float x = lane.direction > 0
                ? i * lane.spacing
                : Win_W - i * lane.spacing;

            CVEHICLE* obj = createObstacle(lane.type, lane.speed, lane.direction);
            obj->loadSprite(lane.spritePath, x, lane.y);
            mObstacles.push_back(obj);
        }
    }

    for (auto& ani : cfg.animals) {
        for (int i = 0; i < ani.count; i++) {
            float x = ani.direction > 0
                ? i * ani.spacing
                : Win_W - i * ani.spacing;

            CANIMAL* obj = createAnimal(ani.type, ani.speed, ani.direction);
            obj->loadSprite(ani.spritePath, x, ani.y);
            mAnimals.push_back(obj);
        }
    }

    mTraffic = new CTRAFFIC_LV1(mObstacles);
    mTraffic->loadSprite(
        cfg.trafficRedPath, cfg.trafficGreenPath,
        cfg.trafficX, cfg.trafficY
    );
}

void EntityManager::update(float dt) {
    for (auto* obs : mObstacles) {
        obs->Move(dt);
        obs->update(dt);
    }
    for (auto* ani : mAnimals) {
        ani->Move(dt);
        ani->update(dt);
    }
    if (mTraffic) {
        mTraffic->update(dt);
    }
}

void EntityManager::draw(sf::RenderWindow& window) const {
    for (auto* obs : mObstacles) obs->Draw(window);
    for (auto* ani : mAnimals)   ani->Draw(window);
    if (mTraffic) mTraffic->Draw(window);
}

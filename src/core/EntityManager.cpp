#include "EntityManager.h"
#include <cstdio>

// Obstacle
#include "CBUS.h"
#include "CCAMEL.h"
#include "CCAR.h"
#include "CDINOSOUR.h"
#include "CECAR.h"
#include "CETRAIN.h"
#include "CHORSE.h"
#include "CMAMMOTH.h"
#include "CMOTOR.h"
#include "CRICKSHAW.h"
#include "CSPHINX.h"
#include "CWOOD.h"

// Animal
#include "CARROW.h"
#include "CBIRD.h"
#include "CDRONE.h"
#include "CEAGLE.h"
#include "CFLYCAR.h"
#include "CHORUS.h"
#include "CMISSILE.h"
#include "CPLANE.h"
#include "CROW.h"
#include "CUFO.h"

// Traffic
#include "CTRAFFIC_LV1.h"
#include "CTRAFFIC_LV2.h"
#include "CTRAFFIC_LV3.h"
#include "CTRAFFIC_LV4.h"
#include "CTRAFFIC_LV5.h"

#include "Utils.h"

EntityManager::~EntityManager() { clear(); }

void EntityManager::clear() {
  for (auto *o : mObstacles)
    delete o;
  for (auto *a : mAnimals)
    delete a;
  delete mTraffic;

  mObstacles.clear();
  mAnimals.clear();
  mTraffic = nullptr;
}

CVEHICLE *EntityManager::createObstacle(ObstacleType type, float speed,
                                        float direction) {
  switch (type) {
  case ObstacleType::DINOSAUR:
    return new CDINOSOUR(speed, direction);
  case ObstacleType::MAMMOTH:
    return new CMAMMOTH(speed, direction);
  case ObstacleType::CAMEL:
    return new CCAMEL(speed, direction);
  case ObstacleType::SPHINX:
    return new CSPHINX(speed, direction);
  case ObstacleType::HORSE:
    return new CHORSE(speed, direction);
  case ObstacleType::RICKSHAW:
    return new CRICKSHAW(speed, direction);
  case ObstacleType::WOOD:
    return new CWOOD(speed, direction);
  case ObstacleType::BUS:
    return new CBUS(speed, direction);
  case ObstacleType::CAR:
    return new CCAR(speed, direction);
  case ObstacleType::MOTOR:
    return new CMOTOR(speed, direction);
  case ObstacleType::ECAR:
    return new CECAR(speed, direction);
  case ObstacleType::ETRAIN:
    return new CETRAIN(speed, direction);
  }
  return new CDINOSOUR(speed, direction);
}

CANIMAL *EntityManager::createAnimal(AnimalType type, float speed,
                                     float direction) {
  switch (type) {
  case AnimalType::BIRD:
    return new CBIRD(speed, direction);
  case AnimalType::EAGLE:
    return new CEAGLE(speed, direction);
  case AnimalType::HORUS:
    return new CHORUS(speed, direction);
  case AnimalType::ROW:
    return new CROW(speed, direction);
  case AnimalType::ARROW:
    return new CARROW(speed, direction);
  case AnimalType::MISSILE:
    return new CMISSILE(speed, direction);
  case AnimalType::PLANE:
    return new CPLANE(speed, direction);
  case AnimalType::DRONE:
    return new CDRONE(speed, direction);
  case AnimalType::FLYCAR:
    return new CFLYCAR(speed, direction);
  case AnimalType::UFO:
    return new CUFO(speed, direction);
  }
  return new CBIRD(speed, direction);
}

void EntityManager::spawnFromLevel(const LevelConfig &cfg) {
  clear();

  for (auto &lane : cfg.lanes) {
    for (int i = 0; i < lane.count; i++) {
      float x =
          lane.direction > 0 ? i * lane.spacing : Win_W - i * lane.spacing;

      CVEHICLE *obj = createObstacle(lane.type, lane.speed, lane.direction);
      obj->loadSprite(lane.spritePath, x, lane.y);
      mObstacles.push_back(obj);
    }
  }

  for (auto &ani : cfg.animals) {
    for (int i = 0; i < ani.count; i++) {
      float x = ani.direction > 0 ? i * ani.spacing : Win_W - i * ani.spacing;

      CANIMAL *obj = createAnimal(ani.type, ani.speed, ani.direction);
      obj->loadSprite(ani.spritePath, x, ani.y);
      mAnimals.push_back(obj);
    }
  }

  if (cfg.level == 2)
    mTraffic = new CTRAFFIC_LV2(mObstacles);
  else if (cfg.level == 3)
    mTraffic = new CTRAFFIC_LV3(mObstacles);
  else if (cfg.level == 4)
    mTraffic = new CTRAFFIC_LV4(mObstacles);
  else if (cfg.level == 5)
    mTraffic = new CTRAFFIC_LV5(mObstacles);
  else
    mTraffic = new CTRAFFIC_LV1(mObstacles);
  mTraffic->loadSprite(cfg.trafficRedPath, cfg.trafficGreenPath, cfg.trafficX,
                       cfg.trafficY);
}

void EntityManager::update(float dt) {
  for (auto *obs : mObstacles) {
    obs->Move(dt);
    obs->update(dt);
  }
  for (auto *ani : mAnimals) {
    ani->Move(dt);
    ani->update(dt);
  }
  if (mTraffic) {
    mTraffic->update(dt);
  }
}

void EntityManager::draw(sf::RenderWindow &window) const {
  for (auto *obs : mObstacles)
    obs->Draw(window);
  for (auto *ani : mAnimals)
    ani->Draw(window);
  if (mTraffic)
    mTraffic->Draw(window);
}

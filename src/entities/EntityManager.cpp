#include "EntityManager.h"
#include <cstdio>
#include <memory>

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
  mTraffic.reset();
  mObstacles.clear();
  mAnimals.clear();
}

std::unique_ptr<CVEHICLE>
EntityManager::createObstacle(ObstacleType type, float speed, float direction) {
  switch (type) {
  case ObstacleType::DINOSAUR:
    return std::make_unique<CDINOSOUR>(speed, direction);
  case ObstacleType::MAMMOTH:
    return std::make_unique<CMAMMOTH>(speed, direction);
  case ObstacleType::CAMEL:
    return std::make_unique<CCAMEL>(speed, direction);
  case ObstacleType::SPHINX:
    return std::make_unique<CSPHINX>(speed, direction);
  case ObstacleType::HORSE:
    return std::make_unique<CHORSE>(speed, direction);
  case ObstacleType::RICKSHAW:
    return std::make_unique<CRICKSHAW>(speed, direction);
  case ObstacleType::WOOD:
    return std::make_unique<CWOOD>(speed, direction);
  case ObstacleType::BUS:
    return std::make_unique<CBUS>(speed, direction);
  case ObstacleType::CAR:
    return std::make_unique<CCAR>(speed, direction);
  case ObstacleType::MOTOR:
    return std::make_unique<CMOTOR>(speed, direction);
  case ObstacleType::ECAR:
    return std::make_unique<CECAR>(speed, direction);
  case ObstacleType::ETRAIN:
    return std::make_unique<CETRAIN>(speed, direction);
  }
  return std::make_unique<CDINOSOUR>(speed, direction);
}

std::unique_ptr<CANIMAL>
EntityManager::createAnimal(AnimalType type, float speed, float direction) {
  switch (type) {
  case AnimalType::BIRD:
  case AnimalType::PTERO:
    return std::make_unique<CBIRD>(speed, direction);
  case AnimalType::EAGLE:
    return std::make_unique<CEAGLE>(speed, direction);
  case AnimalType::HORUS:
    return std::make_unique<CHORUS>(speed, direction);
  case AnimalType::ROW:
    return std::make_unique<CROW>(speed, direction);
  case AnimalType::ARROW:
    return std::make_unique<CARROW>(speed, direction);
  case AnimalType::MISSILE:
    return std::make_unique<CMISSILE>(speed, direction);
  case AnimalType::PLANE:
    return std::make_unique<CPLANE>(speed, direction);
  case AnimalType::DRONE:
    return std::make_unique<CDRONE>(speed, direction);
  case AnimalType::FLYCAR:
    return std::make_unique<CFLYCAR>(speed, direction);
  case AnimalType::UFO:
    return std::make_unique<CUFO>(speed, direction);
  }
  return std::make_unique<CBIRD>(speed, direction);
}

void EntityManager::spawnFromLevel(const LevelConfig &cfg) {
  clear();

  mObstacles.reserve(32);
  mAnimals.reserve(32);

  for (auto &lane : cfg.lanes) {
    float laneOffset = static_cast<float>(rand() % 160 - 80);
    for (int i = 0; i < lane.count; i++) {
      float jitter = static_cast<float>(rand() % 80 - 40);
      float x = lane.direction > 0
                    ? (i * lane.spacing + laneOffset + jitter)
                    : (Win_W - i * lane.spacing + laneOffset + jitter);

      auto obj = createObstacle(lane.type, lane.speed, lane.direction);
      obj->loadSprite(lane.spritePath, x, lane.y);
      mObstacles.push_back(std::move(obj));
    }
  }

  for (auto &ani : cfg.animals) {
    float animalOffset = static_cast<float>(rand() % 160 - 80);
    for (int i = 0; i < ani.count; i++) {
      float jitter = static_cast<float>(rand() % 80 - 40);
      float x = ani.direction > 0
                    ? (i * ani.spacing + animalOffset + jitter)
                    : (Win_W - i * ani.spacing + animalOffset + jitter);

      auto obj = createAnimal(ani.type, ani.speed, ani.direction);
      obj->loadSprite(ani.spritePath, x, ani.y);
      mAnimals.push_back(std::move(obj));
    }
  }

  if (cfg.level == 2)
    mTraffic = std::make_unique<CTRAFFIC_LV2>(mObstacles);
  else if (cfg.level == 3)
    mTraffic = std::make_unique<CTRAFFIC_LV3>(mObstacles);
  else if (cfg.level == 4)
    mTraffic = std::make_unique<CTRAFFIC_LV4>(mObstacles);
  else if (cfg.level == 5)
    mTraffic = std::make_unique<CTRAFFIC_LV5>(mObstacles);
  else
    mTraffic = std::make_unique<CTRAFFIC_LV1>(mObstacles);
  mTraffic->loadSprite(cfg.trafficRedPath, cfg.trafficGreenPath, cfg.trafficX,
                       cfg.trafficY);
}

void EntityManager::update(float dt, float speedMultiplier) {
  float effectiveDt = dt * speedMultiplier;
  for (auto &obs : mObstacles) {
    obs->Move(effectiveDt);
    obs->update(effectiveDt);
  }
  for (auto &ani : mAnimals) {
    ani->Move(effectiveDt);
    ani->update(effectiveDt);
  }
  if (mTraffic) {
    mTraffic->update(effectiveDt);
  }
}

void EntityManager::draw(sf::RenderWindow &window) const {
  for (auto &obs : mObstacles)
    obs->Draw(window);
  for (auto &ani : mAnimals)
    ani->Draw(window);
  if (mTraffic)
    mTraffic->Draw(window);
}

std::vector<sf::FloatRect>
EntityManager::getPredictedHitboxes(float lookaheadTime) const {
  std::vector<sf::FloatRect> boxes;
  boxes.reserve((mObstacles.size() + mAnimals.size()) * 2);

  for (const auto &obs : mObstacles) {
    sf::FloatRect r = obs->getHitbox();
    // 1. Current position hitbox
    boxes.push_back(r);

    // 2. Continuous swept trajectory over [0, lookaheadTime]
    if (!obs->isStopped() && lookaheadTime > 0.f) {
      float vx = obs->getSpeed() * obs->getDirection();
      float shift = vx * lookaheadTime;
      sf::FloatRect swept = r;
      if (shift < 0.f) {
        swept.left += shift;
        swept.width += -shift;
      } else {
        swept.width += shift;
      }
      boxes.push_back(swept);
    }
  }

  for (const auto &ani : mAnimals) {
    sf::FloatRect r = ani->getHitbox();
    // 1. Current position hitbox
    boxes.push_back(r);

    // 2. Continuous swept trajectory over [0, lookaheadTime]
    if (lookaheadTime > 0.f) {
      float vx = ani->getSpeed() * ani->getDirection();
      float shift = vx * lookaheadTime;
      sf::FloatRect swept = r;
      if (shift < 0.f) {
        swept.left += shift;
        swept.width += -shift;
      } else {
        swept.width += shift;
      }
      boxes.push_back(swept);
    }
  }

  return boxes;
}

#include "BotAI.h"
#include "EntityManager.h"
#include "HazardManager.h"
#include "PowerUpManager.h"
#include "CPEOPLE.h"
#include "Utils.h"
#include <algorithm>
#include <cstdlib>
#include <cmath>

void BotAI::init(BotDifficulty diff, EntityManager* em, HazardManager* hm, PowerUpManager* pm) {
    mEntityManager  = em;
    mHazardManager  = hm;
    mPowerUpManager = pm;
    setDifficulty(diff);
    reset();
}

void BotAI::reset() {
    mCurrentPath.clear();
    mPathIndex = 0;
    mReplanTimer = 0.f;
}

GridNode BotAI::worldToGrid(sf::Vector2f pos) {
    float cx = pos.x + Player_W / 2.f;
    float cy = pos.y + Player_H / 2.f;
    return { std::clamp((int)(cy / CELL_SIZE), 0, (int)(Win_H / CELL_SIZE)),
             std::clamp((int)(cx / CELL_SIZE), 0, (int)(Win_W / CELL_SIZE)) };
}

sf::Vector2f BotAI::gridToWorld(GridNode n) {
    float targetX = n.col * CELL_SIZE + (CELL_SIZE - Player_W) / 2.f;
    float targetY = n.row * CELL_SIZE + (CELL_SIZE - Player_H) / 2.f;
    return { targetX, targetY };
}

std::vector<GridNode> BotAI::getNeighbors(GridNode n, bool allowWait) {
    std::vector<GridNode> neighbors = {
        { n.row - 1, n.col }, // UP (Primary Goal)
        { n.row, n.col - 1 }, // LEFT
        { n.row, n.col + 1 }, // RIGHT
        { n.row + 1, n.col }  // DOWN (Retreat / Dodge)
    };
    if (allowWait) {
        neighbors.push_back({ n.row, n.col }); // WAIT in place
    }
    return neighbors;
}

float BotAI::getPredictedNodeCost(GridNode n, float futureTime,
                                  const std::vector<sf::FloatRect>& dangerZones) const {
    float cost = 1.0f;
    sf::FloatRect cellRect(n.col * CELL_SIZE - 2.f, n.row * CELL_SIZE - 2.f, CELL_SIZE + 4.f, CELL_SIZE + 4.f);

    // 1. Spatio-Temporal Obstacle Trajectory Collision Check
    if (mEntityManager) {
        bool isTrafficRed = false;
        if (mEntityManager->traffic() && mEntityManager->traffic()->isRed()) {
            isTrafficRed = true;
        }

        // Vehicles
        for (const auto& obs : mEntityManager->obstacles()) {
            sf::FloatRect hb = obs->getHitbox();
            // If traffic light is red in level and car is stopped, it does not move
            float speed = (obs->isStopped() || isTrafficRed) ? 0.f : obs->getSpeed();
            float vx = speed * obs->getDirection();
            float shift = vx * futureTime;

            sf::FloatRect predictedBox = hb;
            predictedBox.left += shift;

            // Expand predicted box slightly for safety margin
            predictedBox.left -= 6.f;
            predictedBox.width += 12.f;

            if (predictedBox.intersects(cellRect)) {
                cost += mParams.dangerPenalty;
            }
        }

        // Animals
        for (const auto& ani : mEntityManager->animals()) {
            sf::FloatRect hb = ani->getHitbox();
            float vx = ani->getSpeed() * ani->getDirection();
            float shift = vx * futureTime;

            sf::FloatRect predictedBox = hb;
            predictedBox.left += shift;
            predictedBox.left -= 6.f;
            predictedBox.width += 12.f;

            if (predictedBox.intersects(cellRect)) {
                cost += mParams.dangerPenalty;
            }
        }
    }

    // 2. Active Hazards (Lasers, Stampede, Lightning)
    for (const auto& dz : dangerZones) {
        if (dz.intersects(cellRect)) {
            cost += mParams.dangerPenalty * 2.0f;
        }
    }

    // 3. Power-Up Attraction (Detour Incentive)
    if (mPowerUpManager && mPowerUpManager->hasItemNear(cellRect)) {
        cost -= (mParams.powerUpAttractionWeight * 25.f);
    }

    return std::max(0.05f, cost);
}

std::vector<GridNode> BotAI::runSpatioTemporalAStar(GridNode start, int goalRow, float botSpeed) const {
    const int maxCols = (int)(Win_W / CELL_SIZE);
    const int maxRows = (int)(Win_H / CELL_SIZE);

    float effectiveSpeed = std::max(80.f, botSpeed * mParams.speedFactor);
    float stepTime = CELL_SIZE / effectiveSpeed;

    std::vector<sf::FloatRect> dangerZones;
    if (mHazardManager && (mHazardManager->isHazardActive() || mHazardManager->isWarningActive())) {
        dangerZones = mHazardManager->getDangerZones();
    }

    // Min-priority queue based on fScore (gScore + heuristic)
    typedef std::pair<float, TimeGridNode> PQElement;
    std::priority_queue<PQElement, std::vector<PQElement>, std::greater<PQElement>> pq;

    std::unordered_map<TimeGridNode, float, TimeGridNodeHash> gScore;
    std::unordered_map<TimeGridNode, TimeGridNode, TimeGridNodeHash> cameFrom;

    TimeGridNode startNode = { start.row, start.col, 0 };
    gScore[startNode] = 0.f;

    auto heuristic = [&](const TimeGridNode& node) -> float {
        float rowDist = std::max(0.f, static_cast<float>(node.row - goalRow));
        return rowDist * 1.5f;
    };

    pq.push({ heuristic(startNode), startNode });

    TimeGridNode bestNode = startNode;
    float bestFScore = 999999.f;

    while (!pq.empty()) {
        auto [currentF, u] = pq.top();
        pq.pop();

        if (u.row <= goalRow) {
            bestNode = u;
            break;
        }

        if (currentF < bestFScore) {
            bestFScore = currentF;
            bestNode = u;
        }

        if (u.timeStep >= mParams.maxSearchDepth) {
            continue;
        }

        // Neighbors include UP, LEFT, RIGHT, DOWN and optional WAIT (stay in place)
        auto neighbors = getNeighbors({ u.row, u.col }, mParams.canWait);
        for (GridNode vGrid : neighbors) {
            if (vGrid.col < 0 || vGrid.col > maxCols || vGrid.row < 0 || vGrid.row > maxRows) {
                continue;
            }

            TimeGridNode v = { vGrid.row, vGrid.col, u.timeStep + 1 };
            float futureTime = v.timeStep * stepTime;

            float penaltyCost = getPredictedNodeCost(vGrid, futureTime, dangerZones);

            // Move bias
            float moveWeight = 1.0f;
            if (vGrid.row < u.row) {
                moveWeight = 0.85f;  // UP (Prioritize advancing)
            } else if (vGrid.row == u.row && vGrid.col == u.col) {
                moveWeight = 1.15f;  // WAIT (Safe pause for car gap)
            } else if (vGrid.row > u.row) {
                moveWeight = 1.60f;  // DOWN (Retreat penalty)
            } else {
                moveWeight = 1.05f;  // LEFT/RIGHT (Lateral)
            }

            float tentativeG = gScore[u] + (penaltyCost * moveWeight);

            if (!gScore.count(v) || tentativeG < gScore[v]) {
                gScore[v] = tentativeG;
                cameFrom[v] = u;
                float f = tentativeG + heuristic(v);
                pq.push({ f, v });
            }
        }
    }

    // Reconstruct Path from best/goal node back to start
    std::vector<GridNode> path;
    TimeGridNode curr = bestNode;
    while (!(curr == startNode)) {
        path.push_back({ curr.row, curr.col });
        if (!cameFrom.count(curr)) break;
        curr = cameFrom[curr];
    }
    path.push_back({ startNode.row, startNode.col });
    std::reverse(path.begin(), path.end());

    return path;
}

sf::Vector2f BotAI::calculateMicroDodge(const sf::Vector2f& botPos) const {
    if (!mEntityManager) return { 0.f, 0.f };

    sf::Vector2f dodgeVector(0.f, 0.f);
    sf::FloatRect threatBox(botPos.x - 30.f, botPos.y - 20.f, Player_W + 60.f, Player_H + 40.f);

    for (const auto& obs : mEntityManager->obstacles()) {
        sf::FloatRect ob = obs->getHitbox();
        if (ob.intersects(threatBox)) {
            float obsCenterX = ob.left + ob.width / 2.f;
            float botCenterX = botPos.x + Player_W / 2.f;
            // Push away laterally
            if (botCenterX < obsCenterX) {
                dodgeVector.x -= 140.f;
            } else {
                dodgeVector.x += 140.f;
            }
        }
    }

    return dodgeVector;
}

void BotAI::update(float dt, CPEOPLE& botEntity, int goalRow) {
    if (botEntity.isDead() || botEntity.isFinish()) {
        botEntity.setMoving(false);
        return;
    }

    float baseSpeed = botEntity.getSpeed();
    float effectiveSpeed = baseSpeed * mParams.speedFactor * botEntity.getPowerUpSpeedMultiplier();

    // 1. Emergency Reflex Check: if an obstacle is about to crash within 0.15s
    if (mParams.emergencyDodge && mEntityManager) {
        sf::FloatRect botBox = botEntity.getHitbox();
        sf::FloatRect threatBox(botBox.left - 14.f, botBox.top - 6.f, botBox.width + 28.f, botBox.height + 12.f);
        auto immediateObstacles = mEntityManager->getPredictedHitboxes(0.18f);
        for (const auto& obs : immediateObstacles) {
            if (obs.intersects(threatBox)) {
                mReplanTimer = 0.f; // Force immediate Spatio-Temporal A* replanning
                break;
            }
        }
    }

    // 2. Periodic Spatio-Temporal A* Path Replanning
    mReplanTimer -= dt;
    if (mReplanTimer <= 0.f) {
        mReplanTimer = mParams.replanInterval;
        GridNode start = worldToGrid(botEntity.getPosition());
        mCurrentPath = runSpatioTemporalAStar(start, goalRow, effectiveSpeed);
        mPathIndex = 1; // Step 0 is current cell, step 1 is next target

        // Mistake chance on EASY/NORMAL difficulty
        if (mCurrentPath.size() > 1 && mParams.mistakeChance > 0.f &&
            (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) < mParams.mistakeChance)) {
            auto neighbors = getNeighbors(start, false);
            if (!neighbors.empty()) {
                GridNode altStep = neighbors[rand() % neighbors.size()];
                std::vector<sf::FloatRect> dz = (mHazardManager && (mHazardManager->isHazardActive() || mHazardManager->isWarningActive()))
                                             ? mHazardManager->getDangerZones() : std::vector<sf::FloatRect>{};
                if (getPredictedNodeCost(altStep, 0.2f, dz) < mParams.dangerPenalty * 0.5f) {
                    mCurrentPath[1] = altStep;
                }
            }
        }
    }

    // 3. Movement Execution & Micro-Dodging
    if (mPathIndex < mCurrentPath.size()) {
        GridNode targetNode = mCurrentPath[mPathIndex];
        GridNode currentNode = worldToGrid(botEntity.getPosition());

        // Check if the next action is a "WAIT" action in the current cell
        if (targetNode == currentNode) {
            // Intelligent pause / waiting for traffic gap
            botEntity.setMoving(false);
            botEntity.setFacingRow(3); // Face UP towards crossing direction
            return;
        }

        sf::Vector2f target = gridToWorld(targetNode);
        sf::Vector2f cur = botEntity.getPosition();
        sf::Vector2f dir = target - cur;
        float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);

        if (dist < 6.f) {
            mPathIndex++;
            if (mPathIndex >= mCurrentPath.size()) {
                botEntity.setMoving(false);
            }
        } else {
            dir /= dist;
            sf::Vector2f moveVelocity = dir * effectiveSpeed;

            // Apply reactive micro-dodge repulsion if enabled
            if (mParams.emergencyDodge) {
                sf::Vector2f dodge = calculateMicroDodge(cur);
                moveVelocity += dodge;
            }

            botEntity.setPosition(cur.x + moveVelocity.x * dt, cur.y + moveVelocity.y * dt);
            botEntity.setMoving(true);

            // Update walk animation facing direction
            if (std::abs(dir.y) >= std::abs(dir.x)) {
                if (dir.y < 0) botEntity.setFacingRow(3); // UP
                else botEntity.setFacingRow(0);           // DOWN
            } else {
                if (dir.x < 0) botEntity.setFacingRow(1); // LEFT
                else botEntity.setFacingRow(2);           // RIGHT
            }
        }
    } else {
        botEntity.setMoving(false);
    }
}

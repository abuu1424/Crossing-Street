#include "BotAI.h"
#include "EntityManager.h"
#include "HazardManager.h"
#include "PowerUpManager.h"
#include "CPEOPLE.h"
#include "Utils.h"
#include <algorithm>
#include <cstdlib>

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
    // Offset by center of player entity
    float cx = pos.x + Player_W / 2.f;
    float cy = pos.y + Player_H / 2.f;
    return { std::clamp((int)(cy / CELL_SIZE), 0, (int)(Win_H / CELL_SIZE)),
             std::clamp((int)(cx / CELL_SIZE), 0, (int)(Win_W / CELL_SIZE)) };
}

sf::Vector2f BotAI::gridToWorld(GridNode n) {
    // Return top-left position for CPEOPLE entity centered in cell
    float targetX = n.col * CELL_SIZE + (CELL_SIZE - Player_W) / 2.f;
    float targetY = n.row * CELL_SIZE + (CELL_SIZE - Player_H) / 2.f;
    return { targetX, targetY };
}

std::vector<GridNode> BotAI::getNeighbors(GridNode n) {
    return {
        { n.row - 1, n.col }, // UP
        { n.row + 1, n.col }, // DOWN
        { n.row, n.col - 1 }, // LEFT
        { n.row, n.col + 1 }  // RIGHT
    };
}

float BotAI::getNodeCost(GridNode n,
                         const std::vector<sf::FloatRect>& predictedObstacles,
                         const std::vector<sf::FloatRect>& dangerZones) const {
    float cost = 1.0f;
    sf::FloatRect cellRect(n.col * CELL_SIZE, n.row * CELL_SIZE, CELL_SIZE, CELL_SIZE);

    // Dynamic vehicle / animal obstacles lookahead collision penalty
    for (const auto& hb : predictedObstacles) {
        if (hb.intersects(cellRect)) {
            cost += mParams.dangerPenalty;
        }
    }

    // Active hazard danger zones penalty
    for (const auto& dz : dangerZones) {
        if (dz.intersects(cellRect)) {
            cost += mParams.dangerPenalty * 1.8f;
        }
    }

    // Power-up attraction
    if (mPowerUpManager && mPowerUpManager->hasItemNear(cellRect)) {
        cost -= mParams.powerUpAttractionWeight;
    }

    return std::max(0.05f, cost);
}

std::vector<GridNode> BotAI::runDijkstra(GridNode start, int goalRow) const {
    const int maxCols = (int)(Win_W / CELL_SIZE);
    const int maxRows = (int)(Win_H / CELL_SIZE);

    // Pre-cache predicted hitboxes & hazard zones ONCE per Dijkstra execution for max performance
    std::vector<sf::FloatRect> predictedObstacles;
    if (mEntityManager) {
        predictedObstacles = mEntityManager->getPredictedHitboxes(mParams.predictionHorizon);
    }

    std::vector<sf::FloatRect> dangerZones;
    if (mHazardManager && (mHazardManager->isHazardActive() || mHazardManager->isWarningActive())) {
        dangerZones = mHazardManager->getDangerZones();
    }

    // Min-priority queue
    std::priority_queue<std::pair<float, GridNode>,
                        std::vector<std::pair<float, GridNode>>,
                        std::greater<std::pair<float, GridNode>>> pq;

    std::unordered_map<GridNode, float, GridNodeHash> dist;
    std::unordered_map<GridNode, GridNode, GridNodeHash> prev;

    dist[start] = 0.f;
    pq.push({ 0.f, start });
    GridNode goalNode = start;
    bool found = false;

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > dist[u]) continue;

        // Reached target goal row
        if (u.row <= goalRow) {
            goalNode = u;
            found = true;
            break;
        }

        for (GridNode v : getNeighbors(u)) {
            if (v.col < 0 || v.col > maxCols || v.row < 0 || v.row > maxRows) {
                continue;
            }

            float w = getNodeCost(v, predictedObstacles, dangerZones);
            // Slight bias towards forward movement (decreasing row)
            if (v.row < u.row) {
                w *= 0.95f;
            } else if (v.row > u.row) {
                w *= 1.15f;
            }

            float nd = dist[u] + w;
            if (!dist.count(v) || nd < dist[v]) {
                dist[v] = nd;
                prev[v] = u;
                pq.push({ nd, v });
            }
        }
    }

    if (!found) return {};

    std::vector<GridNode> path;
    for (GridNode cur = goalNode; !(cur == start); cur = prev[cur]) {
        path.push_back(cur);
    }
    path.push_back(start);
    std::reverse(path.begin(), path.end());
    return path;
}

void BotAI::update(float dt, CPEOPLE& botEntity, int goalRow) {
    if (botEntity.isDead() || botEntity.isFinish()) {
        botEntity.setMoving(false);
        return;
    }

    // 1. Emergency Reflex Check: if an obstacle is dangerously close or about to crash into bot
    if (mParams.emergencyDodge && mEntityManager) {
        sf::FloatRect botBox = botEntity.getHitbox();
        sf::FloatRect threatBox(botBox.left - 10.f, botBox.top - 4.f, botBox.width + 20.f, botBox.height + 8.f);
        auto immediateObstacles = mEntityManager->getPredictedHitboxes(0.15f);
        for (const auto& obs : immediateObstacles) {
            if (obs.intersects(threatBox)) {
                mReplanTimer = 0.f; // Force immediate replan to dodge away
                break;
            }
        }
    }

    // 2. Periodic Dijkstra Path Replanning
    mReplanTimer -= dt;
    if (mReplanTimer <= 0.f) {
        mReplanTimer = mParams.replanInterval;
        GridNode start = worldToGrid(botEntity.getPosition());
        mCurrentPath = runDijkstra(start, goalRow);
        mPathIndex = 1; // Step 0 is current cell, move to step 1

        // Mistake chance: low difficulty bot occasionally chooses an alternate safe step
        if (mCurrentPath.size() > 1 && mParams.mistakeChance > 0.f &&
            (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) < mParams.mistakeChance)) {
            auto neighbors = getNeighbors(start);
            if (!neighbors.empty()) {
                GridNode altStep = neighbors[rand() % neighbors.size()];
                std::vector<sf::FloatRect> obs = mEntityManager ? mEntityManager->getPredictedHitboxes(0.1f) : std::vector<sf::FloatRect>{};
                std::vector<sf::FloatRect> dz  = (mHazardManager && (mHazardManager->isHazardActive() || mHazardManager->isWarningActive())) ? mHazardManager->getDangerZones() : std::vector<sf::FloatRect>{};
                // Only take alternate step if it is safe (< penalty threshold)
                if (getNodeCost(altStep, obs, dz) < mParams.dangerPenalty * 0.5f) {
                    mCurrentPath[1] = altStep;
                }
            }
        }
    }

    // 3. Movement Execution with Speed Factor & Walk Direction Animation
    if (mPathIndex < mCurrentPath.size()) {
        sf::Vector2f target = gridToWorld(mCurrentPath[mPathIndex]);
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
            float baseSpeed = botEntity.getSpeed();
            float effectiveSpeed = baseSpeed * mParams.speedFactor * botEntity.getPowerUpSpeedMultiplier();
            botEntity.setPosition(cur.x + dir.x * effectiveSpeed * dt, cur.y + dir.y * effectiveSpeed * dt);
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

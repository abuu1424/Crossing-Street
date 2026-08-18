#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_map>
#include <queue>
#include <cmath>

class EntityManager;
class HazardManager;
class PowerUpManager;
class CPEOPLE;

enum class BotDifficulty { EASY, NORMAL, HARD };

struct BotDifficultyParams {
    float replanInterval;          // seconds between Dijkstra recalculations
    float predictionHorizon;       // seconds to project dynamic obstacle hitboxes
    float dangerPenalty;           // weight penalty for hazardous cells
    float speedFactor;             // speed multiplier relative to player base speed
    float mistakeChance;           // 0..1 probability of taking a non-optimal safe step
    bool  emergencyDodge;          // instant reaction reflex on imminent threat
    float powerUpAttractionWeight; // incentive to route toward power-ups
};

inline BotDifficultyParams getBotParams(BotDifficulty d) {
    switch (d) {
        case BotDifficulty::EASY:
            return {
                /*replanInterval*/          0.38f,
                /*predictionHorizon*/       0.20f,
                /*dangerPenalty*/           900.f,
                /*speedFactor*/             0.84f,  // -16% speed, forgiving for beginner players
                /*mistakeChance*/           0.20f,  // occasional hesitation / alternate safe route
                /*emergencyDodge*/          false,
                /*powerUpAttractionWeight*/ 0.3f
            };
        case BotDifficulty::NORMAL:
            return {
                /*replanInterval*/          0.18f,
                /*predictionHorizon*/       0.45f,
                /*dangerPenalty*/           3000.f,
                /*speedFactor*/             1.00f,  // 100% equal speed to player
                /*mistakeChance*/           0.04f,
                /*emergencyDodge*/          true,   // dodges oncoming close-range cars
                /*powerUpAttractionWeight*/ 0.85f   // collects nearby powerups
            };
        case BotDifficulty::HARD:
            return {
                /*replanInterval*/          0.08f,  // 12.5 Hz fast Dijkstra replanning
                /*predictionHorizon*/       0.70f,  // long lookahead with swept danger boxes
                /*dangerPenalty*/           8000.f, // strict hazard avoidance
                /*speedFactor*/             1.10f,  // +10% speed, swift & highly competitive
                /*mistakeChance*/           0.00f,  // 0% mistakes, optimal path execution
                /*emergencyDodge*/          true,   // instant reflex evasion
                /*powerUpAttractionWeight*/ 1.40f   // actively seeks Speed Boost / Shield
            };
    }
    return { 0.18f, 0.45f, 3000.f, 1.0f, 0.04f, true, 0.85f };
}

struct GridNode {
    int row, col;
    bool operator==(const GridNode& o) const { return row == o.row && col == o.col; }
    bool operator!=(const GridNode& o) const { return !(*this == o); }
    bool operator<(const GridNode& o) const {
        return row < o.row || (row == o.row && col < o.col);
    }
};

struct GridNodeHash {
    size_t operator()(const GridNode& n) const {
        return std::hash<int>()(n.row) * 73856093u ^ std::hash<int>()(n.col);
    }
};

class BotAI {
public:
    void init(BotDifficulty diff, EntityManager* em, HazardManager* hm, PowerUpManager* pm);
    void setDifficulty(BotDifficulty diff) { mParams = getBotParams(diff); mDifficulty = diff; }
    BotDifficulty getDifficulty() const { return mDifficulty; }
    void update(float dt, CPEOPLE& botEntity, int goalRow);
    void reset();

    // Node & Path utility functions
    static GridNode worldToGrid(sf::Vector2f pos);
    static sf::Vector2f gridToWorld(GridNode n);
    static std::vector<GridNode> getNeighbors(GridNode n);

    std::vector<GridNode> runDijkstra(GridNode start, int goalRow) const;

private:
    EntityManager*  mEntityManager  = nullptr;
    HazardManager*  mHazardManager  = nullptr;
    PowerUpManager* mPowerUpManager = nullptr;

    BotDifficulty       mDifficulty = BotDifficulty::NORMAL;
    BotDifficultyParams mParams;

    float mReplanTimer = 0.f;
    std::vector<GridNode> mCurrentPath;
    size_t mPathIndex = 0;

    static constexpr float CELL_SIZE = 48.f;

    float getNodeCost(GridNode n,
                      const std::vector<sf::FloatRect>& predictedObstacles,
                      const std::vector<sf::FloatRect>& dangerZones) const;
};

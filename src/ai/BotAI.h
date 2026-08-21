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
    float replanInterval;          // seconds between Spatio-Temporal A* recalculations
    float predictionHorizon;       // seconds to project dynamic obstacle trajectories
    float dangerPenalty;           // weight penalty for hazardous cells
    float speedFactor;             // speed multiplier relative to player base speed
    float mistakeChance;           // 0..1 probability of taking a non-optimal safe step
    bool  emergencyDodge;          // instant reaction reflex on imminent threat
    bool  canWait;                 // ability to intelligently pause/wait for traffic gaps
    float powerUpAttractionWeight; // incentive to route toward power-ups
    int   maxSearchDepth;          // max time steps in Spatio-Temporal A*
};

inline BotDifficultyParams getBotParams(BotDifficulty d) {
    switch (d) {
        case BotDifficulty::EASY:
            return {
                /*replanInterval*/          0.32f,
                /*predictionHorizon*/       0.35f,
                /*dangerPenalty*/           1200.f,
                /*speedFactor*/             0.85f,  // -15% speed, beginner-friendly
                /*mistakeChance*/           0.15f,  // occasional hesitation / slight misdirection
                /*emergencyDodge*/          false,
                /*canWait*/                 true,
                /*powerUpAttractionWeight*/ 0.35f,
                /*maxSearchDepth*/          14
            };
        case BotDifficulty::NORMAL:
            return {
                /*replanInterval*/          0.14f,
                /*predictionHorizon*/       0.60f,
                /*dangerPenalty*/           4500.f,
                /*speedFactor*/             1.00f,  // 100% equal speed to player
                /*mistakeChance*/           0.02f,
                /*emergencyDodge*/          true,   // dodges oncoming close-range cars
                /*canWait*/                 true,   // waits for safe traffic gaps
                /*powerUpAttractionWeight*/ 0.90f,  // collects nearby powerups
                /*maxSearchDepth*/          22
            };
        case BotDifficulty::HARD:
            return {
                /*replanInterval*/          0.06f,  // 16.6 Hz real-time Spatio-Temporal A* replanning
                /*predictionHorizon*/       0.90f,  // deep trajectory lookahead with lane traffic sync
                /*dangerPenalty*/           9500.f, // strict hazard avoidance
                /*speedFactor*/             1.10f,  // +10% speed, highly competitive
                /*mistakeChance*/           0.00f,  // 0% mistakes, flawless execution
                /*emergencyDodge*/          true,   // instant reflex micro-dodging
                /*canWait*/                 true,   // masterfully times lane crossings & waits
                /*powerUpAttractionWeight*/ 1.50f,  // actively steals Speed Boost / Shield
                /*maxSearchDepth*/          30
            };
    }
    return { 0.14f, 0.60f, 4500.f, 1.0f, 0.02f, true, true, 0.90f, 22 };
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

struct TimeGridNode {
    int row;
    int col;
    int timeStep;
    bool operator==(const TimeGridNode& o) const {
        return row == o.row && col == o.col && timeStep == o.timeStep;
    }
    bool operator!=(const TimeGridNode& o) const { return !(*this == o); }
    bool operator<(const TimeGridNode& o) const {
        if (row != o.row) return row < o.row;
        if (col != o.col) return col < o.col;
        return timeStep < o.timeStep;
    }
};

struct TimeGridNodeHash {
    size_t operator()(const TimeGridNode& n) const {
        size_t h1 = std::hash<int>()(n.row);
        size_t h2 = std::hash<int>()(n.col);
        size_t h3 = std::hash<int>()(n.timeStep);
        return h1 ^ (h2 << 1) ^ (h3 << 2);
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
    static std::vector<GridNode> getNeighbors(GridNode n, bool allowWait = false);

    std::vector<GridNode> runSpatioTemporalAStar(GridNode start, int goalRow, float botSpeed) const;

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

    float getPredictedNodeCost(GridNode n, float futureTime,
                               const std::vector<sf::FloatRect>& dangerZones) const;

    sf::Vector2f calculateMicroDodge(const sf::Vector2f& botPos) const;
};

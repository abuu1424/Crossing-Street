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
    float replanInterval;      // seconds between Dijkstra recalculations
    float predictionHorizon;   // seconds to project dynamic obstacle hitboxes
    float dangerPenalty;       // weight penalty for hazardous cells
    float mistakeChance;       // 0..1 probability of taking a non-optimal safe step
    float reactionDelay;       // reaction latency
};

inline BotDifficultyParams getBotParams(BotDifficulty d) {
    switch (d) {
        case BotDifficulty::EASY:
            return { /*replan*/0.5f, /*horizon*/0.25f, /*danger*/1500.f, /*mistake*/0.25f, /*delay*/0.20f };
        case BotDifficulty::NORMAL:
            return { /*replan*/0.3f, /*horizon*/0.35f, /*danger*/3000.f, /*mistake*/0.10f, /*delay*/0.10f };
        case BotDifficulty::HARD:
            return { /*replan*/0.12f, /*horizon*/0.5f, /*danger*/6000.f, /*mistake*/0.02f, /*delay*/0.02f };
    }
    return { 0.3f, 0.35f, 3000.f, 0.10f, 0.10f };
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
